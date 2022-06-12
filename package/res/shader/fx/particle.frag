#version 330 core

in Fragment_Data
{
	vec4 color;
} in_data;

out vec4 o_Color;

void main()
{
	o_Color = in_data.color;
}