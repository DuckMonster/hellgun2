#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Velocity;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Scale;
layout(location = 4) in float a_SpawnTime;
layout(location = 5) in float a_KillTime;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform float u_SystemAge;

out Vertex_Data
{
	vec3 velocity;
	vec4 color;

	float scale;
	float spawn_time;
	float kill_time;
	float normalized_age;
} out_data;

void main()
{
	gl_Position = u_Model * vec4(a_Position, 1.f);

	out_data.velocity = a_Velocity;
	out_data.color = a_Color;
	out_data.scale = a_Scale;

	out_data.spawn_time = a_SpawnTime;
	out_data.kill_time = a_KillTime;
	out_data.normalized_age = (u_SystemAge - a_SpawnTime) / (a_KillTime - a_SpawnTime);
}