#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties
uniform sampler2D texture1;
uniform vec3 light_direction;
uniform vec3 light_position1;
uniform vec3 light_position2;
uniform vec3 light_color1;
uniform vec3 light_color2;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int is_spot;
uniform float cut_off_angle;
uniform float light_intensity1;
uniform float light_intensity2;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

vec3 calculateLight(int is_spot, vec3 light_position, vec3 culoareLumina, float light_intensity) {
	float emisive_light = 0;
	vec3 color = vec3(0);
	vec3 N = normalize(world_normal);

	vec3 L = normalize(light_position - world_position);
	vec3 R = reflect(-L, N);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	float diffuse_light = material_kd * light_intensity * max(dot(N, L), 0);

	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * light_intensity *
			pow(max(dot(N, H), 0), material_shininess);
	}

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
		color = (emisive_light + culoareLumina * (light_att_factor * (diffuse_light + specular_light)));
	}
	else {
		float factorAtenuare = 1 / (d * d + 1);
		color = (culoareLumina * (factorAtenuare * (diffuse_light + specular_light)));
	}

	return color;
}

void main()
{
	vec3 color1 = calculateLight(0, light_position1, light_color1, light_intensity1);
	vec3 color2 = calculateLight(1, light_position2, light_color2, light_intensity2);
	float ambient_light = material_kd * (light_intensity1+light_intensity2)/2;
	vec4 color = texture2D(texture1, texcoord);
	
	if (color.a < 0.3f)
		discard;
	else {
		vec4 c = vec4(object_color * ambient_light + color1 + color2, 1);
		out_color = mix(color, c, 0.35f);
	}
}