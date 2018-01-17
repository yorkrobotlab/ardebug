# ARDebug
ARDebug is a tool for monitoring and debugging swarm robotics and multi-robot experiments in real time using augmented reality techniques. The software provides a GUI for displaying internal data reported wirelessly by each robot within the swarm or group, which includes a real-time video feed of the robots and their environment, augmented with graphical representations of the data. The software aims to reduce the time taken to develop and debug swarm robotics experiments and behaviours by giving the developer more immediate access to decision making varaibles, sensor readings, and other key data, as well as presenting it in a more rapidly readable format that standard console or log-based debugging output.

## Installation
Installation guide. Need to provide a built version? Instructions for downloading QTCreator?

## Features
Summary of core features, maybe screenshots

## Interfacing with ARDebug
In order to maximise performance and minimize latency ARDebug receives data from the robots in a simple string-literal format via UDP packets when using WiFi or a standard Bluetooth connection. The data-strings received should follow the standardised format outlined below, where each portion of the data string (referred to as a packet) is separated by a <i>space</i> character:

| Robot ID | Packet Type ID | Packet Data |
| -------- | -------------- | ----------- |

##### Robot ID
A positive, integer number which identifies the robot to the application. For convenience it is helpful for this ID number to match the ID number by which the tracking system knows the robot, but this is not strictly necessary.

##### Packet Types
The packet type ID is an integer number identifying the type of data contained within the data string. Possible values are:

| Packet Type ID | Packet Type | Packet Contents |
| -------------- | ----------- | --------------- |
| 0 | Watchdog | Notifies the application that the robot is still active. Contains a preferred name for the robot in the data field |
| 1 | State | Reports the robots current state, contained as a string in the data field |
| 2 | Position | Contains the x, y position of the robot within the experiment space, reported by the tracking system. The data field should contain the two numerical values separated by a space. |
| 3 | IR Sensor Data | Contains the robot's IR sensor readings as a series of space separated integers |
| 4 | IR Background Data | Contains the robot's background (passive) IR sensor readings as a series of space separated integers |
| 5 | Message | Used to send a string-based message to the application, which will be reported in the application console. The data field should contain a string, and may contain spaces. |
| 6 | Custom Data | Used to send any custom data value, in the form of a key/value pair. Data field should include the key string, followed by the value string, separated by a space. Neither may contain spaces. |

ode for communicating packets in this format via WiFi from a linux-based robot is provided in <i>ardebug/RobotAPI</i>, primarily for illustrative purposes. An example of how this can be integrated into an example robot controller is provided in <i>ardebug/ExampleRobotController</i>
