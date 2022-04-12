#version 330 core
#define TAU (6.28318530718f)

layout(points) in;
layout(line_strip, max_vertices = 11) out;

in Vertex_Data
{
	vec4 color;
	float normalized_age;
} in_data[];
out Vertex_Data
{
	vec4 color;
	float normalized_age;
} out_data;

uniform mat4 u_ViewProjection;

void emit(vec2 position)
{
	gl_Position = u_ViewProjection * vec4(position, 0.f, 1.f);
	EmitVertex();
}

void main()
{
	out_data.color = in_data[0].color;
	vec2 pos = gl_in[0].gl_Position.xy;
	float size = 1.f - in_data[0].normalized_age;

	float angle = in_data[0].normalized_age;
	const float angle_step = TAU / 10;
	for(int i = 0; i < 10; i += 2)
	{
		emit(pos + vec2(sin(angle + angle_step * i), cos(angle + angle_step * i)) * size);
		emit(pos + vec2(sin(angle + angle_step * (i + 1)), cos(angle + angle_step * (i + 1))) * size * 0.6f);
	}

	emit(pos + vec2(sin(angle), cos(angle)) * size);

	EndPrimitive();
}