#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 FragColor;

layout (location = 1) in vec3 TextureCoords;

layout (binding = 4) uniform samplerCube Skybox;

void main()
{             
  FragColor = texture(Skybox, TextureCoords);
}