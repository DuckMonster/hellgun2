#version 330 core

in Vertex_Data
{
	vec4 color;
	float normalized_age;
} in_data;

out vec4 o_Color;

void main()
{
	o_Color = in_data.color;
}