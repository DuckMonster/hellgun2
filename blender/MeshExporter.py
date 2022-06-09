from hellgun.BinaryStream import BinaryStream

def export_mesh(object, path):
	mesh_data = object.data;
	uv_layer = mesh_data.uv_layers.active.data;

	stream = BinaryStream();
	stream.create(path);

	# Write world transform
	for y in range(4):
		for x in range(4):
			stream.write_float(object.matrix_world[x][y]);

	# Write positions
	stream.write_u32(len(mesh_data.vertices));

	for vertex in mesh_data.vertices:
		pos = vertex.co;
		stream.write_pack("fff", pos.x, pos.y, pos.z);

	# Write UVs
	stream.write_u32(len(uv_layer));

	for uv in uv_layer:
		stream.write_pack("ff", uv.uv.x, uv.uv.y)

	# Write polygons
	stream.write_u32(len(mesh_data.polygons));

	for poly in mesh_data.polygons:
		stream.write_pack("fff", poly.normal.x, poly.normal.y, poly.normal.z);

		# Number of vertices in this polygon
		stream.write_u32(poly.loop_total);
		for loop in poly.loop_indices:
			# Write vertex index
			stream.write_u32(mesh_data.loops[loop].vertex_index);
			# Write UV index (which just happens to be the same as the loop index);
			stream.write_u32(loop);

	stream.close();