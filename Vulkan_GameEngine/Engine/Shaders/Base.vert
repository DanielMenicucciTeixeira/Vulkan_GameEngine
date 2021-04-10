#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformCameraObject 
{
    mat4 View;
    mat4 Projection;
} Camera;

layout(binding = 1) uniform UniformModelObject
{
    mat4 Matrix;
} Model;

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InTextureCoordinate;
layout(location = 2) in vec3 InNormal;

layout(location = 1) out vec3 FragTextureCoordinate;

void main() 
{
    mat4 projection = Camera.Projection;
    projection[0][0] = -abs(projection[0][0]);
    projection[1][1] = abs(projection[1][1]);
    mat4 identity = mat4(vec4(1, 0 , 0, 0), vec4(0, 1 , 0, 0), vec4(0, 0 , 1, 0), vec4(0, 0 , 0, 1));
    gl_Position = projection * Camera.View * Model.Matrix * vec4(InPosition, 1.0);

    //gl_Position = Camera.Projection * Camera.View * Model.Matrix * vec4(InPosition, 1.0);
    FragTextureCoordinate = InTextureCoordinate;
}