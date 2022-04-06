#include "resourcecommon.h"

namespace Common_Mesh
{
	Mesh rect;
}
namespace Common_Mat
{
	Material test;
}

static const Vec2 RECT_DATA[] = {
	{ -0.5f, -0.5f },
	{ 0.5f, -0.5f },
	{ 0.5f, 0.5f },

	{ -0.5f, -0.5f },
	{ 0.5f, 0.5f },
	{ -0.5f, 0.5f },
};

void load_common_resources()
{
	using namespace Common_Mesh;
	using namespace Common_Mat;

	// Common_Mesh::rect
	rect.init();
	rect.add_buffer(0);
	rect.bind_buffer(0, 2);
	rect.buffer_data(0, sizeof(RECT_DATA), RECT_DATA);

	rect.draw_mode = GL_TRIANGLES;
	rect.draw_num = 6;

	// Common_Mat::test
	test.load_file("res/test.vert", "res/test.frag");
}