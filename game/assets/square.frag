#version 120

// Input uniforms from the application
uniform float u_health;      // Current health value
uniform float u_maxHealth;   // Maximum possible health
uniform float u_time;        // Time for animation effects

void main() {
    // Define color states
    vec3 fullHealthColor = vec3(0.0, 0.0, 1.0);  // Blue at full health
    vec3 lowHealthColor = vec3(1.0, 0.0, 0.0);   // Red at low health
    
    // Calculate health percentage for color interpolation
    float healthPercent = u_health / u_maxHealth;
    
    // Smoothly transition between colors based on health
    vec3 color = mix(lowHealthColor, fullHealthColor, healthPercent);
    
    // Add warning effect when health is critical (below 30%)
    if (healthPercent < 0.3) {
        float pulse = 0.7 + 0.3 * sin(u_time * 5.0);  // Pulsing effect
        color *= pulse;
    }
    
    gl_FragColor = vec4(color, 1.0);
}
