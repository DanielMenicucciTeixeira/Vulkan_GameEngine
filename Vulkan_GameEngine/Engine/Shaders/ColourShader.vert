#version 450

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec2 InTextureCoords;
layout (location = 2) in vec3 InNormal;

out vec3 Normal;
out vec2 TextureCoords;
out vec3 Colour;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


void main()
{
	//gl_Position = ModelMatrix * vec4(InPosition, 1.0f);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(InPosition, 1.0f);
	Colour = normalize(InPosition);
}