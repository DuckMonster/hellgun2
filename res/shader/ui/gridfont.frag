#version 330 core

uniform sampler2D u_Sampler;
uniform vec4 u_Foreground;
uniform vec4 u_Background;

in vec2 f_TexCoord;
out vec4 o_Color;

void main()
{
	vec4 clr = mix(u_Background, u_Foreground, texture(u_Sampler, f_TexCoord).r);
	o_Color = clr;
}