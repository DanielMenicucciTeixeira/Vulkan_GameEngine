#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec4 InColor;

layout(location = 0) out vec4 FragColor;

void main() {
    gl_Position = vec4(InPosition, 1.0);
    FragColor = InColor;
}