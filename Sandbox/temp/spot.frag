#version 330

precision lowp float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;

uniform sampler2D u_shadow;
uniform sampler2D texture;

uniform vec2  u_resolution;
uniform vec2  u_position;
uniform vec3  u_color;
uniform float u_radius;
uniform float u_attenuation;
uniform float u_lightPower;

float CreateCircle(vec2 position, float radius) {
	float circleShape = length(position);
	float circle      = smoothstep(radius, radius + u_attenuation, 1.0 - circleShape);
	return circle;
}

void main() {
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;

	float aspect = u_resolution.x / u_resolution.y;
	uv.x *= aspect;
	uv.y = 1.0 - uv.y;

	vec2  pos = u_position / u_resolution;
	float rad = 1.0 - (u_radius / u_resolution.x);
	
	vec4 pixel 		 = texture2D(texture, i_tex);
	vec4 shadowPixel = texture2D(u_shadow, i_tex);

	float circleShape = CreateCircle(uv - pos, rad);
	
	shadowPixel *= circleShape;

    vec4 circle = vec4(1.0 - (u_color * circleShape) * u_lightPower, 1.0) + shadowPixel * u_lightPower;

	o_color = pixel * circle;
}