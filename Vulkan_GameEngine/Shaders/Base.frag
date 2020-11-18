#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D TextureSampler;
layout(location = 0) in vec4 FragColor;
layout(location = 1) in vec2 FragTextureCoordinate;

layout(location = 0) out vec4 OutColor;

void main() 
{
    OutColor = vec4(FragColor * vec4(texture(TextureSampler, FragTextureCoordinate).rgb, 1.0f));
    //OutColor = vec4(FragColor, 1.0);
}