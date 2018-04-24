#!/usr/bin/python

import json

class Robot:
    def __init__(self):
        self.id = 'robot_5'
        self.pose = { 'x': 0.1, 'y': 0.2, 'orientation': 10 }
        self.state = 'Wandering'
        self.batteryLevel = 12.6
        self.internalState = {'alpha': 12, 'nextTarget': 'somewhere', 'ir': [0.3, 0.6, 0.1, 0.05]}

    def toJson(self):
        return json.dumps({
                    'id': self.id,
                    'state': 'Wandering',
                    'batteryLevel': self.batteryLevel,
                    'internalState': self.internalState,
            'someString' : 'Some new String'
               })

testRobot = Robot()
print(testRobot.toJson())

import socket
from math import cos, sin
from time import sleep

hostName = ''
hostPort = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect((hostName, hostPort))

while True:
    
    testRobot.pose['orientation'] += 1
    if testRobot.pose['orientation'] > 360:
        testRobot.pose['orientation'] -= 360
    testRobot.pose['x'] = 0.5 + 0.4*sin(testRobot.pose['orientation']*(3.14159/180))
    testRobot.pose['y'] = 0.5 - 0.4*cos(testRobot.pose['orientation']*(3.14159/180))

    try:
        s.send(testRobot.toJson())
    except:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect((hostName, hostPort))
        s.send(testRobot.toJson())

    sleep(0.1)
        
