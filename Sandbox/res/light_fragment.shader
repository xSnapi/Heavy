varying vec2 st;

uniform sampler2D texture;

uniform vec2  u_resolution;
uniform vec2  u_positions[99];
uniform float u_radius[99];
uniform float u_backLight;
uniform int   u_size;

vec3 Circle(vec2 position, vec3 color, float radius) {
	float circle = sqrt(pow(position.x, 2.0) + pow(position.y, 2.0));
	circle = smoothstep(radius, radius + 0.15, 1.0 - circle);

	return (color - vec3(u_backLight)) * circle;
}

void main() {
	vec2 uv = gl_FragCoord.xy / u_resolution;

	vec4 pixel = texture2D(texture, st.xy);

	vec3 canvas = vec3(u_backLight);
	

	for (int i = 0; i < u_size; i++) {
		vec3 circle = Circle(uv - (u_positions[i] / u_resolution), vec3(1.0, 1.0, 1.0), 1.0 - (u_radius[i] / u_resolution.x));

		canvas += circle;
	}

	gl_FragColor = pixel * vec4(canvas, 1.0);
};