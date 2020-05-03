#!/usr/bin/env python3

import json
import signal
import socket
import sys
import time
import epuck1


class ARDebugPipuck:

	def __init__(self, robot_id, epuck, server_hostname, server_port):
		self.id = robot_id
		self.epuck = epuck
		self.server_hostname = server_hostname
		self.server_port = server_port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.socket.connect((self.server_hostname, self.server_port))
		self.state = "STOPPED"
		self.ir_ambient = [0] * 8
		self.ir_reflected = [0] * 8
		self.battery_voltage = 0.0
		self.epuck.enable_ir_sensors(True)

	def update_battery_voltage(self):
		with open("/sys/class/hwmon/hwmon1/device/in4_input", "r") as battery_file_handle:
			battery_value = battery_file_handle.read()
		# Battery calculation taken from Jacob's ROS node
		self.battery_voltage = (float(battery_value) + 79.10) / 503.1

	def update_ir(self):
		for i in range(8):
			self.ir_ambient[i] = self.epuck.get_ir_ambient(i)
			self.ir_reflected[i] = self.epuck.get_ir_reflected(i)

	def to_json(self):
		values = {
			'id': self.id,
			'state': self.state,
			'ir_ambient': self.ir_ambient,
			'ir_reflected': self.ir_reflected,
			'battery_voltage': self.battery_voltage
		}
		return json.dumps(values)

	def print_data(self):
		print(self.to_json())

	def send_data(self):
		json_data = self.to_json().encode()
		try:
			self.socket.send(json_data)
		except socket.error:
			self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			self.socket.connect((self.server_hostname, self.server_port))
			self.socket.send(json_data)


# Signal handler for Ctrl+C
def signal_handler(sig, frame):
	print("Exiting...")
	epuck.enable_ir_sensors(False)  # Stop IR
	epuck.set_motor_speeds(0, 0)  # Stop motors
	epuck.set_inner_leds(False, False)  # Turn off LEDs
	epuck.set_outer_leds_byte(0)  # Turn off LEDs
	sys.exit(0)


if __name__ == "__main__":
	epuck = epuck1.Epuck1()
	signal.signal(signal.SIGINT, signal_handler)
	robot_id = socket.gethostname()
	robot = ARDebugPipuck(robot_id, epuck, '192.168.2.1', 8888)

	while True:
		robot.update_ir()
		robot.update_battery_voltage()
		robot.print_data()
		robot.send_data()
		time.sleep(0.1)
