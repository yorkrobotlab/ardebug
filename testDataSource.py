#!/usr/bin/python

import json
import random

class Robot:

    def __init__(self, id):
        self.id = id
        self.state = "IDLE"
        self.ir = [0 for _ in range(8)]
        self.battery_voltage = random.uniform(4.0, 4.2)
    def toJson(self):
        return json.dumps({
                    'id': self.id,
                    'state': self.state,
                    'ir': self.ir,
                    'battery_voltage': "%.2f" % self.battery_voltage,
               })

robots = [Robot('robot_%d' % (i,)) for i in range(8)]
    
import socket
from time import sleep

hostName = ''
hostPort = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect((hostName, hostPort))

states = ["IDLE", "WALKING", "TURNING", "AVOIDING"]

while True:
    for r in robots:

        r.state = random.choice(states)
        r.ir = [random.randint(0,4095) for _ in range(8)]
        r.battery_voltage -= 0.001

        print(r.toJson())

        try:
            s.send(r.toJson())
        except:
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect((hostName, hostPort))
            s.send(r.toJson())

    sleep(0.1)
