#!/usr/bin/python

import json
from random import randint

class Robot:
    def __init__(self, id = 'robot_5'):
        self.id = id
        self.pose = { 'x': randint(20, 500)*0.002, 'y': randint(20, 400)*0.0025, 'orientation': 10 }
        self.state = 'Wandering'
        self.batteryLevel = 12.6
        self.internalState = {'alpha': 12, 'nextTarget': 'somewhere', 'ir': [0.3, 0.6, 0.1, 0.05]}

    def toJson(self):
        return json.dumps({
                    'id': self.id,
                    'state': 'Wandering',
                    'batteryLevel': self.batteryLevel,
                    'internalState': self.internalState,
            'someString' : 'Some new String',
            'pose': self.pose
               })

robots = [Robot('robot_%d' % (i,)) for i in range(10)]
    
import socket
from math import cos, sin
from time import sleep

hostName = ''
hostPort = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect((hostName, hostPort))

while True:
    for r in robots:
        try:
            s.send(r.toJson())
        except:
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect((hostName, hostPort))
            s.send(r.toJson())

    sleep(0.1)
        
