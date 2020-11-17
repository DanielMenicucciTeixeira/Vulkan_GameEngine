#version 450
#extension GL_ARB_separate_shader_objects : enable
//#extension GL_KHR_vulkan_glsl : enable

layout(set = 0, binding = 0) uniform UniformBufferObject 
{
    mat4 Model;
    mat4 View;
    mat4 Projecjtion;
} UBO;

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec4 InColor;

layout(location = 0) out vec4 FragColor;

void main() {
    gl_Position = UBO.Projecjtion * UBO.View * UBO.Model * vec4(InPosition, 1.0);
    FragColor = InColor;
}