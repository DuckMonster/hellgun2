#include "obj.h"
#include "core/parse/file_stream.h"

void Obj_File::load(const String& path)
{
	Array<Vec3> positions;
	Array<Vec2> uvs;
	Array<Vec3> normals;

	File_Stream file;
	file.open(path);

	while(!file.eof())
	{
		// Position
		if (file.skip_substr("v ", 2))
		{
			Vec3& position = positions.add_uninitialized();
			file.scan("%f %f %f", &position.x, &position.y, &position.z);
		}

		// UV
		else if (file.skip_substr("vt ", 3))
		{
			Vec2& uv = uvs.add_uninitialized();
			file.scan("%f %f", &uv.x, &uv.y);
		}

		// Normal
		else if (file.skip_substr("vn ", 3))
		{
			Vec3& normal = normals.add_uninitialized();
			file.scan("%f %f %f", &normal.x, &normal.y, &normal.z);
		}

		// Face
		else if (file.skip_substr("f ", 2))
		{
			struct Face_Indicies
			{
				u32 position;
				u32 uv;
				u32 normal;

				Face_Indicies(int position, int uv, int normal)
					:position(position), uv(uv), normal(normal) {}
			};

			Array<Face_Indicies> face_indicies;
			face_indicies.reserve(4);

			while(true)
			{
				// Scan faces until we fail
				int pos_idx, uv_idx, norm_idx;
				int result = file.scan("%d/%d/%d ", &pos_idx, &uv_idx, &norm_idx);

				if (result != 3)
					break;

				// Indicies are 1-based :|
				face_indicies.emplace(pos_idx - 1, uv_idx - 1, norm_idx - 1);
			}

			// Okay! Turn the vertex indicies into actual vertices
			auto push_vertex = [&](u32 idx) {
				Face_Indicies& indicies = face_indicies[idx];
				vertices.emplace(
					positions[indicies.position],
					uvs[indicies.uv],
					normals[indicies.normal]
				);
			};

			// Triangulation algorithm
			// Essentially GL_TRIANGLE_FAN
			for(u32 idx = 2; idx < face_indicies.count(); ++idx)
			{
				push_vertex(0);
				push_vertex(idx - 1);
				push_vertex(idx);
			}
		}

		file.skip_to_next_line();
	}
}