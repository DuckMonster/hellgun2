#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec2 f_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 0.f, 1.f);
	f_TexCoord = a_TexCoord;
}