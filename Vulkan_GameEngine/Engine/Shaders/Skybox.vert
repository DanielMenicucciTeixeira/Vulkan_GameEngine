#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InTextureCoords;
layout (location = 2) in vec3 InNormal;


layout (location = 1) out vec3 TextureCoords;
layout (location = 2) out vec3 Normal;

layout(binding = 0, std140) uniform UniformCamera 
{
    mat4 View;
    mat4 Projection;
} Camera;


void main()
{
    TextureCoords = -InPosition;
    Normal = InNormal; 
    mat4 view = mat4
    (
        Camera.View[0][0], Camera.View[0][1], Camera.View[0][2], 0,
        Camera.View[1][0], Camera.View[1][1], Camera.View[1][2], 0,
        Camera.View[2][0], Camera.View[2][1], Camera.View[2][2], 0,
        0                , 0                , 0                , 1
    );
    gl_Position= Camera.Projection * view * vec4(InPosition, 1.0);
    //vec4 position = Camera.Projection * Camera.View * vec4(InPosition, 1.0);
    //gl_Position = position.xyww;
} 