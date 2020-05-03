import smbus

# Default e-puck I2C bus and firmware address
I2C_CHANNEL = 4
EPUCK_I2C_ADDR = 0x1f

# Register addresses
OUTER_LEDS = 0
INNER_LEDS = 1
LEFT_MOTOR_SPEED = 2
RIGHT_MOTOR_SPEED = 3
LEFT_MOTOR_STEPS = 4
RIGHT_MOTOR_STEPS = 5
IR_CONTROL = 6
IR_REFLECTED_BASE = 7
IR0_REFLECTED = 7
IR1_REFLECTED = 8
IR2_REFLECTED = 9
IR3_REFLECTED = 10
IR4_REFLECTED = 11
IR5_REFLECTED = 12
IR6_REFLECTED = 13
IR7_REFLECTED = 14
IR_AMBIENT_BASE = 15
IR0_AMBIENT = 15
IR1_AMBIENT = 16
IR2_AMBIENT = 17
IR3_AMBIENT = 18
IR4_AMBIENT = 19
IR5_AMBIENT = 20
IR6_AMBIENT = 21
IR7_AMBIENT = 22


class Epuck1:

	def __init__(self, i2c_bus=I2C_CHANNEL, i2c_address=EPUCK_I2C_ADDR):
		self._bus = smbus.SMBus(i2c_bus)
		self._i2c_address = i2c_address

	def _write_data_8(self, address, data):
		self._bus.write_byte_data(self._i2c_address, address, data)

	def _write_data_16(self, address, data):
		self._bus.write_word_data(self._i2c_address, address, data)

	def _read_data_8(self, address):
		return self._bus.read_byte_data(self._i2c_address, address)

	def _read_data_16(self, address):
		return self._bus.read_word_data(self._i2c_address, address)

	def set_outer_leds_byte(self, leds):
		self._write_data_8(OUTER_LEDS, leds)

	def set_outer_leds(self, led0, led1, led2, led3, led4, led5, led6, led7):
		data = 0x00
		if led0:
			data += 0x01
		if led1:
			data += 0x02
		if led2:
			data += 0x04
		if led3:
			data += 0x08
		if led4:
			data += 0x10
		if led5:
			data += 0x20
		if led6:
			data += 0x40
		if led7:
			data += 0x80
		self.set_outer_leds_byte(data)

	def set_inner_leds(self, front, body):
		data = 0x00
		if front:
			data += 0x01
		if body:
			data += 0x02
		self._write_data_8(INNER_LEDS, data)

	def set_left_motor_speed(self, speed):
		self._write_data_16(LEFT_MOTOR_SPEED, int(speed))

	def set_right_motor_speed(self, speed):
		self._write_data_16(RIGHT_MOTOR_SPEED, int(speed))

	def set_motor_speeds(self, speed_left, speed_right):
		self.set_left_motor_speed(speed_left)
		self.set_right_motor_speed(speed_right)

	def get_left_motor_speed(self):
		return self._read_data_16(LEFT_MOTOR_SPEED)

	def get_right_motor_speed(self):
		return self._read_data_16(RIGHT_MOTOR_SPEED)

	def get_motor_speeds(self):
		return self.get_left_motor_speed(), self.get_right_motor_speed()

	def get_left_motor_steps(self):
		return self._read_data_16(LEFT_MOTOR_STEPS)

	def get_right_motor_steps(self):
		return self._read_data_16(RIGHT_MOTOR_STEPS)

	def get_motor_steps(self):
		return self.get_left_motor_steps(), self.get_right_motor_steps()

	def enable_ir_sensors(self, enabled):
		if enabled:
			data = 0x01
		else:
			data = 0x00
		self._write_data_8(IR_CONTROL, data)

	def get_ir_reflected(self, sensor):
		return self._read_data_16(IR_REFLECTED_BASE + sensor)

	def get_ir_ambient(self, sensor):
		return self._read_data_16(IR_AMBIENT_BASE + sensor)
