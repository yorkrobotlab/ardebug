#ifndef BASIC_DISPERSION_CONTROLLER_H
#define BASIC_DISPERSION_CONTROLLER_H

#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_light_sensor.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_base_leds_actuator.h>

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/vector2.h>

#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "debug_network.h"

#define NO_TURN 0
#define SOFT_TURN 1
#define HARD_TURN 2

namespace argos
{
    class CBasicDispersionController : public CCI_Controller
    {
    public:

        CBasicDispersionController();
        virtual ~CBasicDispersionController();

        virtual void Init(TConfigurationNode& t_node);
        virtual void ControlStep();
        virtual void Reset() {};
        virtual void Destroy();

        void WheelSpeedsFromHeadingVector(CVector2 &m_cHeadingVector);

    private:

        SInt32 left_wheel_speed;
        SInt32 right_wheel_speed;

        // Sensors
        CCI_EPuckProximitySensor* proximity_sensor;
        CCI_EPuckLightSensor* light_sensor;

        // Actuators
        CCI_EPuckWheelsActuator* wheels_actuator;
        CCI_EPuckBaseLEDsActuator* base_leds_actuator;

        int control_step;
        int walk_count;
        int random_turn_count;
        int battery_level;

        // Networking object for sending data to SwarmDebug system
        DebugNetwork* swarm_debug;
    };
};

#endif /* BASIC_DISPERSION_CONTROLLER_H */