#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformCameraObject 
{
    mat4 View;
    mat4 Projecjtion;
} Camera;

layout(binding = 1) uniform UniformModelObject
{
    mat4 Matrix;
} Model;

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InTextureCoordinate;

layout(location = 1) out vec3 FragTextureCoordinate;

void main() 
{
    gl_Position = Camera.Projecjtion * Camera.View * Model.Matrix * vec4(InPosition, 1.0);
    FragTextureCoordinate = InTextureCoordinate;
}