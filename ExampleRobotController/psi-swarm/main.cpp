/***********************************************************************
**  ██████╗ ███████╗██╗███████╗██╗    ██╗ █████╗ ██████╗ ███╗   ███╗  **
**  ██╔══██╗██╔════╝██║██╔════╝██║    ██║██╔══██╗██╔══██╗████╗ ████║  **
**  ██████╔╝███████╗██║███████╗██║ █╗ ██║███████║██████╔╝██╔████╔██║  **
**  ██╔═══╝ ╚════██║██║╚════██║██║███╗██║██╔══██║██╔══██╗██║╚██╔╝██║  **
**  ██║     ███████║██║███████║╚███╔███╔╝██║  ██║██║  ██║██║ ╚═╝ ██║  **
**  ╚═╝     ╚══════╝╚═╝╚══════╝ ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝  **
************************************************************************
** Copyright 2016 University of York - See notice at end of file      **
***********************************************************************/

/// based on PsiSwarm C++ Blank Example Code - Version 0.8
/// James Hilder, Alan Millard, Alexander Horsfield, Homero Elizondo, Jon Timmis
/// Charlotte Arndt

/// Include main.h - this includes psiswarm.h all the other necessary core files
#include "main.h"

Psiswarm psi;

char * program_name = "ARDebug";
char * author_name  = "YRL";
char * version_name = "0.80";

Ticker tick_IR;

void move_forward(int distance);
void turn_right();
void turn_left();
void turn(float degree);

void attime();

void ARDebug_sendWatchdog();
void ARDebug_sendIR();
void ARDebug_sendState(string statename);
void ARDebug_sendBackgroundIR();
void ARDebug_sendMessage(string message);
void ARDebug_sendCostumData(string key, string value);

enum state {FORWARD, AVOID};

int currentState = FORWARD;
int previousState = FORWARD;


///User code loop:  This is where user code should go; it is run as an infinite loop
void user_code_loop()
{
   
    int irfront;
    
    irfront = (sensors.calculate_side_ir_value(0 ) + sensors.calculate_side_ir_value( 7)) /2;
    ARDebug_sendWatchdog();
    
    if (currentState == FORWARD)
    {
   
        if (irfront >350)
        {
            previousState = currentState;
            currentState = AVOID;          
        }
        
    
    }
    else
    {
       if (currentState == AVOID)
        {
            if (irfront <300)
            {
                previousState = currentState;
                currentState = FORWARD;        
            } 
        }
          
    } 
    
    //Debug: turn on and off movement
    if (1){
        
        if (currentState == FORWARD)
        {
            if (currentState != previousState)
            {
                previousState = currentState ;
                ARDebug_sendState("FORWARD");
            }
            motors.forward(0.8);
            
        }
        if (currentState == AVOID)
        {
             if (currentState != previousState)
             {
                ARDebug_sendState("AVOID");
                previousState = currentState ;
            }
             motors.set_left_motor_speed(-0.8);
             motors.set_right_motor_speed(0.8);
                
            
        }    
        
    }
    
    //ARDebug_sendMessage("this is a costume message");
    //ARDebug_sendCostumData("costumeValue", "12.4");
      
    wait(0.1);
    
    
}

///Place user code here that should be run after initialisation but before the main loop
void user_code_setup()
{
    wait(1);
    display.clear_display();
    display.set_position(0,0);
    display.write_string("ARDebug");  
    tick_IR.attach(&attime, 0.1);
}

/// Code goes here to handle what should happen when the user switch is pressed
void handle_switch_event(char switch_state)
{
    /// Switch_state = 1 if up is pressed, 2 if down is pressed, 4 if left is pressed, 8 if right is pressed and 16 if the center button is pressed
    /// NB For maximum compatability it is recommended to minimise reliance on center button press
}

void handle_user_serial_message(char * message, char length, char interface)
{
    // This is where user code for handling a (non-system) serial message should go
    //
    // message = pointer to message char array
    // length = length of message
    // interface = 0 for PC serial connection, 1 for Bluetooth
    if(message[0] == 0x00){
        move_forward(5);
        bt.printf("F");
    }
    if(message[0] == 0x01){
      turn_right();
        bt.printf("R");
    }
   led.set_green_leds(7);
}



//distance in cm
void move_forward(int distance)
{
    float time ;
    time = (float) distance /15.2;
    motors.forward( 0.6);
    wait(time);
    motors.brake();
    wait(0.1);
    
}
    
void turn(float degree)
{
    float time ;
    if (degree >=0)
    {
        time = degree /170.0;
        motors.set_left_motor_speed(0.6);
        motors.set_right_motor_speed(-0.6);
        
    
    }
    else
    {
        time = degree /-172.0;
        motors.set_left_motor_speed(-0.6);
        motors.set_right_motor_speed(0.6);
        
        
    }
    wait(time);
    motors.brake();
    wait(0.1);
}


void turn_right()
{
    turn(90.0);
    
    }
    
void turn_left(){
    turn(-90.0);
    
    
    }





/// The main routine: it is recommended to leave this function alone and add user code to the above functions
int main()
{
    psi.init();             ///psi.init() in psiswarm.cpp sets up the robot
    user_code_setup();      ///run user code setup block
    user_code_running = 1;  ///nb. user code can be paused by external commands sent from PC\BT interfaces
    while(1) {
        user_code_loop();   ///run user code
    }
}


void attime()
{
    sensors.store_ir_values(); 
    //sensors.store_background_raw_ir_values();
    ARDebug_sendIR();
    ARDebug_sendBackgroundIR(); 
}

//ARDebug messages

//Watchdog message
void ARDebug_sendWatchdog()
{
     char str[100];
     //[Robotid] [MessageID] [Robotname]
     sprintf(str, "%i %i %s\n", (int) robot_id ,0 , "PsiSwarm");
     bt.printf(str);
}

//Sends the IR values of the robot
void ARDebug_sendIR()
{
    char str[100];
    //[Robotid] [MessageID] 8x[sensorvalue]
    sprintf(str, "%i %i %i %i %i %i %i %i %i %i\n", 
           (int) robot_id, 3, sensors.get_illuminated_raw_ir_value(0),            
            sensors.get_illuminated_raw_ir_value(1),
            sensors.get_illuminated_raw_ir_value(2),
            sensors.get_illuminated_raw_ir_value(3),
            sensors.get_illuminated_raw_ir_value(4),
            sensors.get_illuminated_raw_ir_value(5),
            sensors.get_illuminated_raw_ir_value(6),
            sensors.get_illuminated_raw_ir_value(7));
    bt.printf(str);
}

void ARDebug_sendBackgroundIR()
{
    char str[100];
    //[Robotid] [MessageID] 8x[sensorvalue]
    sprintf(str, "%i %i %i %i %i %i %i %i %i %i\n", 
            (int) robot_id, 4, sensors.get_background_raw_ir_value(0),            
            sensors.get_background_raw_ir_value(1),
            sensors.get_background_raw_ir_value(2),
            sensors.get_background_raw_ir_value(3),
            sensors.get_background_raw_ir_value(4),
            sensors.get_background_raw_ir_value(5),
            sensors.get_background_raw_ir_value(6),
            sensors.get_background_raw_ir_value(7));
    bt.printf(str);
}

void ARDebug_sendState(string statename)
{
    char str[100];
    //[Robotid] [MessageID] [Statename]
    sprintf(str, "%i %i %s\n", (int) robot_id, 1, statename.c_str());
    bt.printf(str);
}

void ARDebug_sendMessage(string message)
{
    char str[100];
    //[Robotid] [MessageID] [Message]
    sprintf(str, "%i %i %s\n", (int) robot_id, 5, message.c_str());
    bt.printf(str);   
}

void ARDebug_sendCostumData(string key, string value)
{
    char str[100];
     //[Robotid] [MessageID] [Key] [Value]
    sprintf(str, "%i %i %s %s\n", (int) robot_id,6 , key.c_str() , value.c_str());
    bt.printf(str);   
}

/***********************************************************************
** Copyright 2016 University of York                                  **
**                                                                    **
** Licensed under the Apache License, Version 2.0 (the "License")     **
** You may not use this file except in compliance with the License.   **
** You may obtain a copy of the License at                            **
** http://www.apache.org/licenses/LICENSE-2.0   Unless required by    **
** applicable law or agreed to in writing, software distributed under **
** under the License is distributed on an "AS IS" BASIS WITHOUT       **  
** WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   ** 
** See the License for the specific language governing permissions    ** 
** and limitations under the License.                                 **
***********************************************************************/
