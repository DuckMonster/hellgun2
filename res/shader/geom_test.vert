#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_InitialVelocity;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_SpawnTime;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform float u_EmitterAge;

out Vertex_Data
{
	vec4 color;
} out_data;

void main()
{
	float age = u_EmitterAge - a_SpawnTime;
	vec3 position = a_Position + a_InitialVelocity * age + vec3(0.f, -10.f, 0.f) * age * age;

	gl_Position = u_Model * vec4(position, 1.f);
	out_data.color = a_Color;
}