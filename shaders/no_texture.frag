#version 330 core

in vec3 normalOutput;
in vec3 posOutput;

uniform vec3 color;

out vec4 fragColor;

void main()
{
    
    fragColor = vec4(color, 1);

}