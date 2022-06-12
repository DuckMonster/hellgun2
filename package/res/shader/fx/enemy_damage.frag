#version 330 core

in vec2 f_TexCoord;

uniform sampler2D u_Sampler;
uniform float u_NormalizedAge;

out vec4 o_Color;

void main()
{
	float tex_alpha = texture(u_Sampler, f_TexCoord).a;
	float age_alpha = 1.f - pow(u_NormalizedAge, 4.f);
	o_Color = vec4(1.f, 1.f, 1.f, tex_alpha * age_alpha);
}
