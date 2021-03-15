#version 450

in vec3 Normal;
in vec2 TextureCoords;

uniform sampler2D TextureDifuse;
//uniform sampler2D TextureSpecular;

out vec4 FragColour;

void main()
{
	//FragColour = vec4(TextureCoords, 0, 0);
	FragColour = texture(TextureDifuse, TextureCoords);
}