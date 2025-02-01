#version 120

uniform vec2 resolution;

// Atributele de la SFML
attribute vec2 position;
attribute vec4 color;

varying vec4 vColor;
varying vec2 vPos;

void main() {
    // Transformare din coordonate pixeli în NDC
    vec2 pos = position;
    pos.x = (pos.x / resolution.x) * 2.0 - 1.0;
    pos.y = 1.0 - (pos.y / resolution.y) * 2.0;
    gl_Position = vec4(pos, 0.0, 1.0);
    
    vColor = color;
    vPos = position;
}

