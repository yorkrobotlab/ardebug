#!/usr/bin/python

import json
import time
import random
import math

import socket
from time import sleep

import sys

class Robot:

    def __init__(self, id, reportPose):
        self.id = id
        self.state = "WALKING"
        self.ir = [random.randint(0, 4095) for _ in range(8)]
        self.battery_voltage = random.uniform(4.0, 4.2)
        self.last_state_change = time.time() + random.uniform(0, 3)
        self.pose = {'x': random.uniform(0.05, 0.95), 'y': random.uniform(0.05, 0.95), 'orientation': random.uniform(0, 360)}
        self.reportPose = reportPose
        self.desired_heading = self.pose['orientation']
        self.last_turn = time.time()
        self.time_since_last_turn = 0

    def toJson(self):
        values = {
                    'id': self.id,
                    'state': self.state,
                    'ir': self.ir,
                    'battery_voltage': self.battery_voltage
               }
        
        if self.reportPose:
            values['pose'] = self.pose
            values['orientation'] = self.pose['orientation']
            values['desired_heading'] = self.desired_heading
            values['time_since_last_turn'] = self.time_since_last_turn
            
        return json.dumps(values)

hostName = ''
hostPort = 8888

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

            r.ir = [x + random.randint(-100, 100) for x in r.ir]

            r.ir = [0 if x < 0 else x for x in r.ir]

            if(r.battery_voltage > 0):
                r.battery_voltage -= random.uniform(0, 0.001)
                if(r.battery_voltage < 0):
                    r.battery_voltage = 0

            if reportPose:

                v = 0.01
                a = 10
                avoidance_radius = 0.1

                dx = v * math.cos(math.radians(r.pose['orientation']))
                dy = v * math.sin(math.radians(r.pose['orientation']))

                if r.state == "WALKING":

                    r.time_since_last_turn = time.time() - r.last_turn

                    if(r.time_since_last_turn > 0.5):

                        for s in robots:
                            if s.id != r.id:
                                if math.sqrt(math.pow(s.pose['x'] - r.pose['x'], 2) + math.pow(s.pose['y'] - r.pose['y'], 2)) < avoidance_radius:
                                    r.desired_heading = r.pose['orientation'] + 180
                                    r.state = "AVOIDING"
                                    break
                    
                        if (r.pose['x'] + dx > 0.95) or (r.pose['x'] + dx < 0.05) or (r.pose['y'] + dy > 0.95) or (r.pose['y'] + dy < 0.05):
                            r.desired_heading = r.pose['orientation'] + 180
                            r.state = "TURNING"

                    r.pose['x'] = r.pose['x'] + dx
                    r.pose['y'] = r.pose['y'] + dy    

                elif r.state == "TURNING" or r.state == "AVOIDING":
                    if r.desired_heading > 360:
                            r.desired_heading = r.desired_heading - 360

                    if abs(r.desired_heading - r.pose['orientation']) < a * 2:
                        r.last_turn = time.time()
                        r.state = "WALKING"
                    else:
                        r.pose['orientation'] = r.pose['orientation'] + a
                        if r.pose['orientation'] > 360:
                            r.pose['orientation'] = r.pose['orientation'] - 360

            print(r.toJson())

            try:
                s.send(r.toJson())
            except:
                s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                s.connect((hostName, hostPort))
                s.send(r.toJson())

        sleep(0.1)
