#version 120

uniform float time;
uniform vec2 resolution;

varying vec4 vColor;
varying vec2 vPos;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

float fbm(vec2 st) {
    float value = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise(st);
        st *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}

void main() {
    vec2 st = vPos / resolution;
    
    vec2 noiseCoords = st * 3.0;
    noiseCoords += vec2(time * 0.2, time * 0.15);
    
    float n = fbm(noiseCoords);
    
    float gradient = smoothstep(0.0, 1.0, st.y);
    
    vec3 stoneDark = vec3(0.3, 0.3, 0.35);
    vec3 stoneLight = vec3(0.5, 0.5, 0.55);
    
    vec3 col = mix(stoneDark, stoneLight, n * gradient);
    col *= 0.9 + 0.1 * sin(time + st.y * 10.0);
    
    gl_FragColor = vec4(col, 1.0);
}

