#version 330 core

layout(location = 0) out vec4 color;

uniform float u_time;

vec3 hueShift(vec3 color) {
	vec3 k = vec3(0.57735);
	float cosAngle = cos(u_time);
	float sinAngle = sin(u_time);

	vec3 A = color * cosAngle;
	vec3 B = cross(k, color) * sinAngle;
	vec3 C = k * dot(k, color) * (1.0 - cosAngle);

	return vec3(A);
}

in vec3 u_color;
void main()
{
	vec3 Color = hueShift(vec3(1.0, 0.0, 0.0));

	//color = vec4(Color, 1.0);
	color = vec4(u_color, 1.0);
};