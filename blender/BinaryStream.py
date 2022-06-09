import os
import struct

class BinaryStream:
	def create(self, path):
		self.file = open(path, "wb");

	def write_pack(self, format, *args):
		self.file.write(struct.pack(format, *args));

	def write_float(self, value):
		self.file.write(struct.pack("f", value));

	def write_u32(self, value):
		self.file.write(struct.pack("I", value));

	def write_i32(self, value):
		self.file.write(struct.pack("i", value));

	def write_string(self, value):
		self.write_u32(len(value));
		self.file.write(bytearray(value.encode()));

	def close(self):
		self.file.close();
