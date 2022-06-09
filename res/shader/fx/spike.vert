#version 330 core

in vec3 a_Position;
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform float u_Scale;
uniform float u_NormalizedAge;

void main()
{
	vec3 position = a_Position;

	// Apply thinning and elongating based on age
	position.y *= (1.f - u_NormalizedAge); 
	position.x *= (1.f + 1.f * u_NormalizedAge);
	position *= u_Scale;

	gl_Position = u_ViewProjection * u_Model * vec4(position, 1.f);
}