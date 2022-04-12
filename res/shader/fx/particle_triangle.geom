#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 4) out;

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

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;
	float size = 1.f - in_data[0].normalized_age;

	gl_Position = u_ViewProjection * vec4(pos + vec3(-0.5f, -0.5f, 0.f) * size, 1.f);
	out_data.color = in_data[0].color;
	EmitVertex();
	gl_Position = u_ViewProjection * vec4(pos + vec3(0.5f, -0.5f, 0.f) * size, 1.f);
	out_data.color = in_data[0].color;
	EmitVertex();
	gl_Position = u_ViewProjection * vec4(pos + vec3(0.f, 0.5f, 0.f) * size, 1.f);
	out_data.color = in_data[0].color;
	EmitVertex();
	gl_Position = u_ViewProjection * vec4(pos + vec3(-0.5f, -0.5f, 0.f) * size, 1.f);
	out_data.color = in_data[0].color;
	EmitVertex();

	EndPrimitive();
}