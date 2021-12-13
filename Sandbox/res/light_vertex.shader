varying vec2 st;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    st = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_FrontColor = gl_Color;
}