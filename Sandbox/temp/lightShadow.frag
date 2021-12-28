#version 330

precision lowp float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;
	
uniform sampler2D texture;
uniform sampler2D u_shadows;

void main() {

    vec4 pixel       = texture2D(texture, i_tex);
    vec4 shadowPixel = texture2D(u_shadows, i_tex);

    o_color = pixel * shadowPixel;
}