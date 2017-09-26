#version 330
layout(location = 0) in vec3 position;
out vec4 vColor;

uniform vec4 color;
uniform mat4 camera;
uniform mat4 model;

void main()
{
  gl_Position = camera * model * vec4(position, 1.0);
  vColor = color;
}
