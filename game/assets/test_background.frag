uniform float u_time;         // Timpul, pentru animația de fond
uniform vec2 lightPosition;   // Poziția pătratului, transmisă din C++
uniform float lightRadius;    // Raza luminii
uniform vec3 lightColor;      // Culoarea luminii

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0); // Normalizarea coordonatelor

    // Animația de fond bazată pe timp
    float gray = 0.5 + 0.5 * sin(u_time * 0.5 + uv.x * 5.0) * cos(u_time * 0.3 + uv.y * 5.0);
    vec3 baseColor = vec3(gray); // Culoarea de bază a fundalului

    // Calculul distanței față de lumină
    vec2 fragPos = gl_FragCoord.xy;
    float distance = length(fragPos - lightPosition);

    // Intensitatea luminii bazată pe distanță
    float intensity = 1.0 - smoothstep(lightRadius * 0.8, lightRadius, distance);

    // Aplicarea efectului de iluminare
    vec3 finalColor = mix(vec3(0.0), baseColor, intensity); // Întuneric în afara razei de lumină

    gl_FragColor = vec4(finalColor, 1.0);
}
