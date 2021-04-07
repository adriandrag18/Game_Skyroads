#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 v_normal;

// Uniform properties
uniform float time;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// TODO: output values to fragment shader
out vec3 frag_color;

void main()
{
	// TODO: send output to fragment shader
	frag_color = vec3(v_color.x, time/10, v_color.z);
	vec3 new_position = vec3(v_position.x, v_position.y + time/10, v_position.z);
	//vec3 new_position = v_position;
	
	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}
