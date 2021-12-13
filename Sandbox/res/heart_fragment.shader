varying vec2 uv;

void main() {
    vec3 col = vec3(0.0);

    uv.x *= 0.7;
    uv.y += sqrt(abs(uv.x)) * 0.5;
    float d = length(uv);

    float c = smoothstep(0.6, 0.56, d);

    col = vec3(c);

    if (c < 1.0)
        discard;

    gl_FragColor = vec4(1.0, 0.0, 0.0, 0.75) * vec4(col, 1.0);
}
