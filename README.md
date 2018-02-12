# ARDebug
ARDebug is a tool for monitoring and debugging swarm robotics and multi-robot experiments in real time using augmented reality techniques. The software provides a GUI for displaying internal data reported wirelessly by each robot within the swarm or group. This data is used to augment a real-time video feed of the robots and their environment, using graphical and textual representations of the data. The software aims to reduce the time taken to develop and debug swarm robotics experiments and behaviours by giving the developer more immediate access to decision making varaibles, sensor readings, and other key data, when compared to standard console or log-based debugging techniques. The software relies on a tracking system to locate the robots within the image in order to correctly overlay the graphical elements, and a modular software architecture is used to allow for easy integration with a variety of tracking systems. The tracking systen used in this reference implementation makes use of [ARuCo](https://www.uco.es/investiga/grupos/ava/node/26) fiducial markers and the associated [OpenCV](https://docs.opencv.org/3.1.0/d5/dae/tutorial_aruco_detection.html) image processing library.

## Installation
Currently ARDebug requires compiling from source, and may require minor modifications to work with your tracking set up. Luckily compiling and building the application is made easy by the Qt toolchain. Simply download QtCreator from [here](https://www.qt.io/download), choosing the open source option. Clone this repository, or download it as a zip file and extract it into a directory, then use QtCreator to open the directory containing the code as a Qt project, and hit *Run* to launch.

Pre-built versions of the software are in the works.

## Features
The primary feature of ARDebug is providing acces to internal robot data. A number of types of data are supported:
- State
⋅⋅⋅ A string describing the robot's current state within its state machine / the current action the robot is performing
- IR Data
⋅⋅⋅ Readings from infrared sensors, including one channel for active IR readings and one for passive IR, which are then displayed comparitively. The orientations of the sensors on the robot can be set to allow for simultaneous graphical representation of a number of sensors.
- Messages
⋅⋅⋅ Textual messages to be displayed to the user in a console-like window. Can be used similarly to debugging 'print' statements, without the need for a terminal or ssh connected to the robot.
- Custom Data
⋅⋅⋅ The software can receive custom data from the robots, in a key-value pair format. The researcher or developer writing the robot code can decide on a key to use for a given element of data, and the software will track the most recent value reported against each key.

The second main feature of the software is taking this data and displaying it to the user in a nubmer of forms. This includes the augmented video view of the robots, which can be overalaid with all of the data mentioned above, as well as robot's names, IDs, position and orientation, and a track of their recent paths. Standard textual representations are alos available, including a history of state changes, and infrared sensor data displayed in both graph and numerical form.

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
