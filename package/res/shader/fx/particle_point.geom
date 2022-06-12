#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in Vertex_Data
{
	vec3 velocity;
	vec4 color;

	float scale;
	float spawn_time;
	float kill_time;
	float normalized_age;
} in_data[];
out Fragment_Data
{
	vec4 color;
} out_data;

uniform mat4 u_ViewProjection;

void emit(vec3 pos)
{
	gl_Position = u_ViewProjection * vec4(pos, 1.f);
	EmitVertex();
}

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;
	float size = (1.f - in_data[0].normalized_age) * in_data[0].scale;

	out_data.color = in_data[0].color;
	emit(pos + vec3(size, size, 0.f));
	emit(pos + vec3(-size, size, 0.f));
	emit(pos + vec3(size, -size, 0.f));
	emit(pos + vec3(-size, -size, 0.f));

	EndPrimitive();
}