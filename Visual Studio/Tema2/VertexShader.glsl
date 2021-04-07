#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform float time;
uniform int deform;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 world_position;
out vec3 world_normal;
out vec2 texcoord;

void main()
{
	vec3 position = v_position;
	if (deform == 1) {
		position += (0.2 + 0.5 * sin(v_position.x * time) * sin(v_position.y * 15 * time) * cos(v_position.z * 5 * time))* v_normal;
	}
	world_position = vec3(Model * vec4(position, 1));
	world_normal = normalize(vec3(Model * vec4(v_normal, 0)));
	texcoord = v_texture_coord;

	gl_Position = Projection * View * Model * vec4(position, 1.0);
}

