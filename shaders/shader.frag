#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;

uniform vec3 color;
uniform int normalColoring;

in vec3 normalOutput;
in vec3 posOutput;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

	vec3 norm = normalize(normalOutput);

	if(normalColoring != 0){
        fragColor = vec4((norm + 1) / 2, 1.0);
    }
	else{
		// Use the color passed in. An alpha of 1.0f means it is not transparent.
		fragColor = vec4(color, sampleExtraOutput);
	}
}