#version 330 core

uniform vec3 color;
uniform int normalColoring;
uniform sampler2D ourTexture;

in vec3 normalOutput;
in vec3 posOutput;
in vec2 TexCoord;

out vec4 fragColor;

void main()
{

	vec3 norm = normalize(normalOutput);

	if(normalColoring != 0){
        fragColor = vec4((norm + 1) / 2, 1.0);
    }
	else{
		//fragColor = vec4(color, 1.0f);
		fragColor = texture(ourTexture, TexCoord);

	}
}