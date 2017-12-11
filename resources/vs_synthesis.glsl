#version 330
layout(location = 0) in vec3 position_modelspace;
layout(location = 1) in vec3 normal_modelspace;

out vec3 position_worldspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace;
out vec3 normal_cameraspace;

uniform vec3 lightPosition_worldspace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 camera;

void main()
{
//    gl_Position = MVP * vec4(position_modelspace, 1.0);
    gl_Position = camera * model * vec4(position_modelspace, 1.0);

    position_worldspace = ( model * vec4(position_modelspace, 1.0) ).xyz;

    vec3 position_cameraspace = ( view * model * vec4(position_modelspace,1)).xyz;
    eyeDirection_cameraspace = vec3(0,0,0) - position_cameraspace;
    vec3 lightPosition_cameraspace = ( view * vec4(lightPosition_worldspace,1)).xyz;
    lightDirection_cameraspace = lightPosition_cameraspace - position_cameraspace;
    normal_cameraspace = ( transpose(inverse(view * model)) * vec4(normal_modelspace,0)).xyz;

//    normal_cameraspace = ( view * model * vec4(normal_modelspace,0)).xyz;
}
