#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 1) in vec2 TextureCoords;
//layout (location = 3) in vec3 FragPosition;
layout (binding = 2) uniform sampler2D Texture;


layout(location = 0) out vec4 FragColour;

void main()
{
	vec2 textureCoords = vec2(TextureCoords.x, TextureCoords.y);
	FragColour = texture(Texture, textureCoords);
}