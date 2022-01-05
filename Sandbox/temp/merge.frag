#version 330 core

precision lowp float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;

uniform sampler2D texture;
uniform sampler2D u_mergeTx;

uniform float u_level;

void main() {
	vec4 pixel = texture2D(texture, i_tex);

	vec4 lightPixel = texture2D(u_mergeTx, i_tex);
	
	vec4 light = vec4(1.0 - lightPixel.x , 1.0 - lightPixel.y, 1.0 - lightPixel.z, 1.0) + u_level;
	
	light = clamp(light, 0.0, 1.0);
	
	o_color = pixel * light;
}