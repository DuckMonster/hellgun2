#include "resourcecommon.h"

namespace Common_Mesh
{
	Mesh rect;
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

	// Common_Mesh::rect
	rect.init();
	rect.add_buffer(0);
	rect.bind_attribute(0, 0, 2);
	rect.buffer_data(0, sizeof(RECT_DATA), RECT_DATA);

	rect.draw_mode = GL_TRIANGLES;
	rect.draw_num = 6;
}