#version 150

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;



void main() {
    // Pass the tex coord straight through to the fragment shader    
    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(vert, 1);
}
