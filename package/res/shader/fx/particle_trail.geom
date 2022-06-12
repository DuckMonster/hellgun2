#version 330 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in Vertex_Data
{
	vec3 velocity;
	vec4 color;

	float scale;
	float spawn_time;
	float kill_time;
	float normalized_age;
} in_data[];
out Fragment_Data
{
	vec4 color;
} out_data;

uniform mat4 u_ViewProjection;

void emit(vec2 position)
{
	gl_Position = u_ViewProjection * vec4(position, 0.f, 1.f);
	EmitVertex();
}

void main()
{

	vec2 direction = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy);
	vec2 perpendicular = vec2(direction.y, -direction.x);

	// This vertex
	float size = pow(1.f - in_data[0].normalized_age, 2.f) * in_data[0].scale;
	vec2 pos = gl_in[0].gl_Position.xy;
	out_data.color = in_data[0].color;

	emit(pos - perpendicular * size);
	emit(pos + perpendicular * size);

	// Other vertex
	size = pow(1.f - in_data[1].normalized_age, 2.f) * in_data[1].scale;
	pos = gl_in[1].gl_Position.xy;
	out_data.color = in_data[1].color;

	emit(pos - perpendicular * size);
	emit(pos + perpendicular * size);

	EndPrimitive();
}