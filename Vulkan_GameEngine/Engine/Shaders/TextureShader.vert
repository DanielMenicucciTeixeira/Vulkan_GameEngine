#version 450
#extension GL_ARB_separate_shader_objects : enable


layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InTextureCoords;
layout (location = 2) in vec3 InNormal;

layout (location = 1) out vec2 TextureCoords;
layout (location = 2) out vec3 Normal;
layout (location = 3) out vec3 FragPosition;
layout (location = 4) out vec3 CameraPosition;

layout(binding = 0, std140) uniform UniformCamera 
{
    mat4 View;
    mat4 Projection;
} Camera;

layout(binding = 1, std140) uniform UniformModel
{
    mat4 Matrix;
} Model;

void main()
{
	gl_Position = Camera.Projection * Camera.View * Model.Matrix * vec4(InPosition, 1.0);
	TextureCoords = InTextureCoords.xy;
	Normal = transpose(inverse(mat3(Model.Matrix))) * InNormal;
	vec4 fragPosition = Model.Matrix * vec4(InPosition, 1.0f);
	fragPosition = fragPosition / fragPosition.w;
	FragPosition = fragPosition.xyz;
	CameraPosition = Camera.View[3].xyz;
}