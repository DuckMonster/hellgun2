#version 330 core

const vec3 LIGHT_DIR = normalize(vec3(-1.5, -1.5, -2.0));
const vec3 COLOR = vec3(0.5, 0.35, 0.25);

in V2F
{
	vec3 normal;
} v2f;

out vec4 o_Color;

void main()
{
	float diffuse = -dot(v2f.normal, LIGHT_DIR);
	o_Color = vec4(COLOR * diffuse, 1.0);
}