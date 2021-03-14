#version 450

in vec3 Normal;
in vec2 TextureCoords;
in vec3 Colour;

out vec4 FragColour;

void main()
{
	FragColour = vec4(Colour, 1.0f);
}