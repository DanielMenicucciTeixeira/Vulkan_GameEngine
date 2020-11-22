#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D TextureSampler;
layout(location = 1) in vec3 FragTextureCoordinate;

layout(location = 0) out vec4 OutColor;

void main() 
{
    OutColor = texture(TextureSampler, vec2(FragTextureCoordinate));
}