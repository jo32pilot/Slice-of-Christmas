#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normalOutput;
out vec3 posOutput;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vec4 posVec = vec4(position, 1.0);
    posOutput = vec3(model * posVec);
    normalOutput = mat3(transpose(inverse(model))) * normal;
}