#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in float a_Alpha;
uniform mat4 u_ViewProjection;

out float f_Alpha;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 0.f, 1.f);
	f_Alpha = a_Alpha;
}