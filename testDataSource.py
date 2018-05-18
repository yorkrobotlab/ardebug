#!/usr/bin/python

import json
import time
import random

import socket
from time import sleep

import sys

class Robot:

    def __init__(self, id, reportPose):
        self.id = id
        self.state = "IDLE"
        self.ir = [random.randint(0, 4095) for _ in range(8)]
        self.battery_voltage = random.uniform(4.0, 4.2)
        self.last_state_change = time.time() + random.uniform(0, 3)
        self.pose = {'x': random.uniform(0.05, 0.95), 'y': random.uniform(0.05, 0.95), 'orientation': random.uniform(0, 360)}
        self.reportPose = reportPose

    def toJson(self):
        values = {
                    'id': self.id,
                    'state': self.state,
                    'ir': self.ir,
                    'battery_voltage': self.battery_voltage
               }
        
        if self.reportPose:
            values['pose'] = self.pose
            
        return json.dumps(values)

hostName = ''
hostPort = 8888

states = ["IDLE", "WALKING", "TURNING", "AVOIDING"]

if __name__=="__main__":
    reportPose = True
    if len(sys.argv) > 1:
        if sys.argv[1] == "aruco":
            reportPose = False
            
    robots = [Robot('robot_%d' % (i,), reportPose) for i in range(8)]

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect((hostName, hostPort))

    while True:
        for r in robots:

            if(time.time() - r.last_state_change > 5):
                r.state = random.choice(states)
                r.last_state_change = time.time()

            r.ir = [x + random.randint(-100, 100) for x in r.ir]

            r.ir = [0 if x < 0 else x for x in r.ir]

            if(r.battery_voltage > 0):
                r.battery_voltage -= random.uniform(0, 0.001)
                if(r.battery_voltage < 0):
                    r.battery_voltage = 0

            print(r.toJson())

            try:
                s.send(r.toJson())
            except:
                s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                s.connect((hostName, hostPort))
                s.send(r.toJson())

        sleep(0.1)
