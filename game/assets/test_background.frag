uniform float u_time;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0);
    
    float gray = 0.5 + 0.5 * sin(u_time * 0.5 + uv.x * 5.0) * cos(u_time * 0.3 + uv.y * 5.0);
    
    vec3 color = vec3(gray);
    
    gl_FragColor = vec4(color, 1.0);
}

