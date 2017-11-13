#version 330
layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 vertNormal;

out vec3 fragVert;
out vec3 fragNormal;

uniform mat4 camera;
uniform mat4 model;

void main()
{
    fragNormal = normalize(transpose(inverse(mat3(model))) * -vertNormal);
    fragVert = vec3(model * vec4(vert, 1));;
    gl_Position = camera * model * vec4(vert, 1);
}
