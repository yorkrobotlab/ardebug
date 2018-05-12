#include <stdlib.h>

#include "basic_dispersion_controller.h"
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <argos3/core/utility/logging/argos_log.h>

#include <sstream>
#include <time.h>

static std::ofstream file;

#define LOG_FILE_INIT(FILENAME) if (file) file.open (FILENAME);
#define LOG_FILE(SMTHG) if (file.is_open()) file << SMTHG;
#define LOG_FILE_CLOSE() if (file.is_open()) file.close();

using namespace argos;

CBasicDispersionController::CBasicDispersionController():
        control_step(0),
        walk_count(0),
        random_turn_count(0),
        battery_level(86),
        left_wheel_speed(0),
        right_wheel_speed(0),
        wheels_actuator(NULL),
        proximity_sensor(NULL),
        ardebug(NULL) {}

CBasicDispersionController::~CBasicDispersionController()
{

}

/* Init
 * Initialise the controller and the debug data reporting
 */
void CBasicDispersionController::Init(TConfigurationNode& t_node)
{
    // Sensors
    proximity_sensor = GetSensor<CCI_EPuckProximitySensor>("epuck_proximity");
    light_sensor = GetSensor<CCI_EPuckLightSensor>("epuck_light");

    // Actuators
    wheels_actuator = GetActuator<CCI_EPuckWheelsActuator>("epuck_wheels");
    base_leds_actuator = GetActuator<CCI_EPuckBaseLEDsActuator>("epuck_base_leds");

    // Initialise ARDebug system with a fallback server IP and robot ID
    ardebug = new DebugNetwork();
    ardebug->init(8888, "192.168.1.101", 99);

    // Initialise rand
    srand(time(NULL));
}

/* Destroy
 * Tear down the controller and debug data handler to free memory.
 */
void CBasicDispersionController::Destroy()
{
    ardebug->destroy();
    delete ardebug;
}

/* ControlStep
 * The behaviour to execute each control step.
 */
void CBasicDispersionController::ControlStep()
{
    // Initialise threshold and max values
    CRadians m_cGoStraightAngleThreshold(0.05);
    Real m_fProximitySensorThreshold(0.01);
    Real MaxSpeed(10);

    // Increment control step
    control_step++;

    // Read the IR sensors
    const CCI_EPuckProximitySensor::TReadings& proximity_sensor_readings = proximity_sensor->GetReadings();

    // Read the IR sensor background values
    const CCI_EPuckLightSensor::TReadings& light_sensor_readings = light_sensor->GetReadings();

    // Create diffusion vector
    CVector2 diffusionVector;
    diffusionVector.Set(0.0f, 0.0f);

    // Average out the IR sensor readings
    for(size_t i = 0; i <  proximity_sensor_readings.size(); ++i)
    {
        if (proximity_sensor_readings[i].Value > 300) {
            diffusionVector += CVector2(proximity_sensor_readings[i].Value, proximity_sensor_readings[i].Angle);
        }
    }

    diffusionVector /= proximity_sensor_readings.size();

    /* If the angle of the vector is small enough and the closest obstacle
       is far enough, ignore the vector and go straight, otherwise turn to avoid it */
    if(diffusionVector.Angle().GetAbsoluteValue() < m_cGoStraightAngleThreshold.GetValue() && diffusionVector.Length() < m_fProximitySensorThreshold ||
       diffusionVector.Length() < 0.05) {
        // Walk forward for 100 control steps if uninterrupted
        if (walk_count < 100) {
            // Set state to WALKING
            state = "WALKING";

            // Walk forward
            wheels_actuator->SetLinearVelocity(3, 3);

            // Increment walk count
            walk_count++;
        } else if (walk_count == 100) {
            // At the 100th step generate a random number of steps to turn for
            random_turn_count = rand() % 50;

            // Increment the count
            walk_count++;
        } else {
            // Set state to TURNING
            state = "TURNING";

            // Turn on the spot
            wheels_actuator->SetLinearVelocity(0, 6);

            // Increment walk count
            walk_count++;

            // Reset walk count once random number is reached
            if (walk_count > 100 + random_turn_count) {
                walk_count = 0;
            }
        }
    } else {
        // Calculate heading vector away from the greatest concentration of IR readings
        CVector2 headingVector = -diffusionVector.Normalize() * MaxSpeed;

        // Move in heading direction
        WheelSpeedsFromHeadingVector(headingVector);

        // Set state to AVOIDING
        state = "AVOIDING";

        // Reset walk count
        walk_count = 0;
    }

    std::ostringstream packet;

    packet << "{";

    packet << "\"id\":\"robot_" << ardebug->getRobotID() << "\",";
    packet << "\"control_step\":" << control_step << ",";
    packet << "\"state\":\"" << state << "\",";
    packet << "\"reflected_ir\":[";

    for(int i = 0; i < proximity_sensor_readings.size(); i++)
    {
        packet << proximity_sensor_readings[i].Value;
        packet << (i < proximity_sensor_readings.size() - 1 ? "," : "]");
    }

    packet << ",";
    packet << "\"ambient_ir\":[";

    for(int i = 0; i < light_sensor_readings.size(); i++)
    {
        packet << light_sensor_readings[i].Value;
        packet << (i < light_sensor_readings.size() - 1 ? "," : "]");
    }

    packet << ",";
    packet << "\"walk_timer\":" << walk_count;
    packet << "}";
    std::cout << packet.str() << std::endl;

    ardebug->sendData(packet.str());
}

/* WheelSpeedsFromHeadingVector
 * Sets the robot's wheel speeds in order to turn it towards a certain vector.
 */
void CBasicDispersionController::WheelSpeedsFromHeadingVector(CVector2 &headingVector)
{
    CRadians m_cNoTurnOnAngleThreshold(0.2);
    CRadians m_cSoftTurnOnAngleThreshold(0.6);
    Real MaxSpeed(10);

    /* Get the heading angle */
    CRadians cHeadingAngle = headingVector.Angle().SignedNormalize();

    /* Get the length of the heading vector */
    Real fHeadingLength = headingVector.Length();
    /* Clamp the speed so that it's not greater than MaxSpeed */
    Real fBaseAngularWheelSpeed = Min<Real>(fHeadingLength, MaxSpeed);

    int TurningMechanism;

    /* Turning state switching conditions */
    if(cHeadingAngle.GetAbsoluteValue() <= m_cNoTurnOnAngleThreshold.GetValue())
    {
        /* No Turn, heading angle very small */
        TurningMechanism = NO_TURN;
        //std::cout << " cHeadingAngle " << cHeadingAngle << " fBaseAngularWheelSpeed " << fBaseAngularWheelSpeed << " NO_TURN " << std::endl;
    }
    else if(cHeadingAngle.GetAbsoluteValue() > m_cNoTurnOnAngleThreshold.GetValue() &&
            cHeadingAngle.GetAbsoluteValue() <= m_cSoftTurnOnAngleThreshold.GetValue())
    {
        /* Soft Turn, heading angle in between the two cases */
        TurningMechanism = SOFT_TURN;
        //std::cout << " cHeadingAngle " << cHeadingAngle << " fBaseAngularWheelSpeed " << fBaseAngularWheelSpeed << " SOFT_TURN " << std::endl;
    }
    else if(cHeadingAngle.GetAbsoluteValue() > m_cSoftTurnOnAngleThreshold.GetValue()) // m_sWheelTurningParams.SoftTurnOnAngleThreshold
    {
        /* Hard Turn, heading angle very large */
        TurningMechanism = HARD_TURN;
        //std::cout << " cHeadingAngle " << cHeadingAngle << " fBaseAngularWheelSpeed " << fBaseAngularWheelSpeed << " HARD_TURN " << std::endl;
    }


    /* Wheel speeds based on current turning state */
    Real fSpeed1, fSpeed2;
    switch(TurningMechanism)
    {
    case NO_TURN:
    {
        /* Just go straight */
        fSpeed1 = fBaseAngularWheelSpeed;
        fSpeed2 = fBaseAngularWheelSpeed;
        break;
    }

    case SOFT_TURN:
    {
        /* Both wheels go straight, but one is faster than the other */ //HardTurnOnAngleThreshold
        Real fSpeedFactor = (m_cSoftTurnOnAngleThreshold.GetValue() - cHeadingAngle.GetAbsoluteValue()) /
                m_cSoftTurnOnAngleThreshold.GetValue();
        fSpeed1 = fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
        fSpeed2 = fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);


        /*
         * to make sure the speeds do not exceed the max speed
         *
         */
        if (fSpeed1 > MaxSpeed)
            fSpeed1 = MaxSpeed;

        if (fSpeed2 > MaxSpeed)
            fSpeed2 = MaxSpeed;

        if (fSpeed1 < -MaxSpeed)
            fSpeed1 = -MaxSpeed;

        if (fSpeed2 < -MaxSpeed)
            fSpeed2 = -MaxSpeed;


        break;
    }

    case HARD_TURN:
    {
        /* Opposite wheel speeds */
        fSpeed1 = -MaxSpeed;
        fSpeed2 =  MaxSpeed;
        break;
    }
    }

    /* Apply the calculated speeds to the appropriate wheels */
    if(cHeadingAngle > CRadians::ZERO)
    {
        /* Turn Left */
        wheels_actuator->SetLinearVelocity(fSpeed1, fSpeed2);
    }
    else
    {
        /* Turn Right */
        wheels_actuator->SetLinearVelocity(fSpeed2, fSpeed1);
    }
}

REGISTER_CONTROLLER(CBasicDispersionController, "basic_dispersion_controller");
