#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_SpawnTime;
layout(location = 3) in float a_KillTime;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform float u_EmitterAge;

out Vertex_Data
{
	vec4 color;
	float normalized_age;
} out_data;

void main()
{
	gl_Position = u_Model * vec4(a_Position, 1.f);

	out_data.color = a_Color;
	out_data.normalized_age = (u_EmitterAge - a_SpawnTime) / (a_KillTime - a_SpawnTime);
}