#version 330 core
in float f_Alpha;

out vec4 o_Color;

void main()
{
	o_Color = vec4(1.f, 0.f, 0.f, f_Alpha);
}