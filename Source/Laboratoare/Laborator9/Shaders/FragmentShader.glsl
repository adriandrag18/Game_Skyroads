#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float time;
uniform int is_globe;
uniform int to_mix;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	if (is_globe == 1) {
		vec2 t = vec2(texcoord.x - time / 4, texcoord.y);
		vec4 color = texture2D(texture_1, t);
		out_color = color;
	}
	else if (to_mix == 1) {
		vec4 color1 = texture2D(texture_1, texcoord);
		vec4 color2 = texture2D(texture_2, texcoord);

		out_color = mix(color1, color2, 0.5f);
	}
	else {
		vec4 color = texture2D(texture_1, texcoord);
		out_color = color;
	}
	if (out_color.a < 0.3f)
		discard;
}