from hellgun.BinaryStream import BinaryStream

def export_level(collection, path):
	stream = BinaryStream();
	stream.create(path);

	# Write the number of objects in this level
	stream.write_u32(len(collection.objects));

	# Write object settings and name
	for object in collection.objects:
		stream.write_string(object.name);
		stream.write_pack("BB", object.hellgun_render, object.hellgun_collision);
		stream.write_string(object.hellgun_material);

	stream.close();