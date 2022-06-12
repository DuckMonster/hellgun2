#version 330 core

in V2F
{
	vec2 uv;
} v2f;

uniform sampler2D u_Sampler;

out vec4 o_Color;

void main()
{
	vec4 clr = texture(u_Sampler, v2f.uv);
	if (clr.a < 0.5) discard; 

	o_Color = clr;
}