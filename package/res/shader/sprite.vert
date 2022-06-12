#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out V2F
{
	vec2 uv;
} v2f;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.f);
	v2f.uv = a_TexCoord;
}