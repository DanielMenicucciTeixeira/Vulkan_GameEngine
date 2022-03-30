#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 InPosition;
layout (location = 2) in vec3 InNormal;


layout (location = 1) out vec3 TextureCoords;

layout(binding = 0, std140) uniform UniformCamera 
{
    mat4 View;
    mat4 Projection;
} Camera;


void main()
{
    TextureCoords = InPosition;

    //gl_Position = Camera.Projection *  Camera.View * vec4(InPosition, 1.0);
    vec4 position = Camera.Projection * mat4(mat3(Camera.View)) * vec4(InPosition, 1.0);
    gl_Position = position.xyww;
} 