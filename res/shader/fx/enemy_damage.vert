#version 330 core
in vec3 a_Position;
in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform float u_NormalizedAge;

out vec2 f_TexCoord;

void main()
{
	float scale = 1.2 + 0.5f * u_NormalizedAge;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position * scale, 1.f);
	f_TexCoord = a_TexCoord;
}