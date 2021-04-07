#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int is_spot;
uniform float cut_off_angle;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	float emisive_light = 0;
	float intensitateLumina = 2;
	vec3 color = vec3(0);
	vec3 culoareLumina = vec3(1);
	vec3 N = normalize(world_normal);

	vec3 L = normalize(light_position - world_position);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * intensitateLumina *
		max(dot(N, L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * intensitateLumina *
			pow(max(dot(N, H), 0), material_shininess);
	}

	// TODO: compute light
	float d = distance(light_position, world_position);

	if (is_spot == 1) {
		float cut_off = radians(cut_off_angle);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);
		float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor = pow(linear_att, 2);
		if (spot_light < spot_light_limit) {
			diffuse_light = 0;
			specular_light = 0;
		}
		color = object_color * (emisive_light + culoareLumina *
			(ambient_light + light_att_factor * (diffuse_light + specular_light)));
	}
	else {
		float factorAtenuare = 1 / (d * d + 1);
		color = object_color * (emisive_light + culoareLumina *
			(ambient_light + factorAtenuare * (diffuse_light + specular_light)));
	}
	// TODO: write pixel out color
	out_color = vec4(color, 1);
}