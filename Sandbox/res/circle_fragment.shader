varying vec2 uv;

void main() {
    float distance = 1.0 - length(uv);

    if (distance > 0.0)
        distance = 1.0;

    gl_FragColor = vec4(1.0, 0.0, 0.0, 0.45) * distance;
}