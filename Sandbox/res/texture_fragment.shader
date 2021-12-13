varying vec2 uv;

uniform sampler2D texture;

void main() {
	vec4 pixel = texture2D(texture, uv.xy);
	
	gl_FragColor = pixel;
};