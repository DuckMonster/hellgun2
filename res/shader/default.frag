#version 330 core

in V2F
{
	vec3 normal;
} v2f;

out vec4 o_Color;

void main()
{
	o_Color = vec4(v2f.normal, 1.f);
}