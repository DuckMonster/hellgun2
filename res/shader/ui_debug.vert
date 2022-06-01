#version 330 core

in vec2 a_Position;

uniform vec2 u_Min;
uniform vec2 u_Max;
uniform mat4 u_ViewProjection;

void main()
{
	vec2 size = u_Max - u_Min;
	vec2 pos = u_Min + a_Position * size;

	gl_Position = u_ViewProjection * vec4(pos, 0.f, 1.f);
}