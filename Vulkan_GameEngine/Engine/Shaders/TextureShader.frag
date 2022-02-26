#version 450
#extension GL_ARB_separate_shader_objects : enable

//Light
//{
//	vec3 Position; [0].xyz
//	float Ambient; [1].x
//	float Diffuse; [1].y
//	float Specular; [1].z
//	vec3 Colour; [2].xyz
//  float LightType; [3].x
//	float TurnedOn; [3].y

layout (location = 1) in vec2 TextureCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 FragPosition;
layout (location = 4) in vec3 CameraPosition;

layout (binding = 2, std140) uniform UniformNumberOfLights
{
	int Number;
} NumberOfLights;

layout (binding = 3, std140) uniform UniformLights 
{
	mat4 InfoMatrix[30];
} Lights;

layout (binding = 4) uniform UniformMaterial
{
	//			x				y				z			w
	// [0]	Ambient.X		Ambient.Y		Ambient.Z		0
	// [1]	Diffuse.X		Diffuse.Y		Diffuse.Z		0
	// [2]	Specular.X		Specular.Y		Specular.Z		0
	// [3]	Shinines		Opacity			0				0
	mat4 Data;
} Material;

layout (binding = 5) uniform sampler2D TextureDifuse;
layout (binding = 6) uniform sampler2D TextureSpecular;


layout(location = 0) out vec4 FragColour;

void main()
{
	vec2 textureCoords = vec2(TextureCoords.x, 1 - TextureCoords.y);
	int offset = 11;
	vec3 ambient = vec3(0);
	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);

	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(CameraPosition - FragPosition);
	vec3 lightDirection;
	float intensity = 1;

	for(int index = 0; index < NumberOfLights.Number; index++)
	{
		if(Lights.InfoMatrix[index][3].y > 0)
		{
			if(Lights.InfoMatrix[index][3].x == 1) intensity = 1/pow(distance(Lights.InfoMatrix[index][0].xyz, FragPosition), 2);
			else intensity = 1;

			if(Lights.InfoMatrix[index][3].x == 2) lightDirection = Lights.InfoMatrix[index][0].xyz;
			else lightDirection = normalize(Lights.InfoMatrix[index][0].xyz - FragPosition);
			
			//Ambient light calculations
			ambient = ambient + (Material.Data[0].xyz * texture(TextureDifuse, textureCoords).rgb) * (Lights.InfoMatrix[index][2].xyz * intensity);

			//Difuse light calculations
			float diff = max(dot(normal,lightDirection), 0.0);
			diffuse = diffuse + ((diff * Material.Data[1].xyz) * texture(TextureDifuse, textureCoords).rgb) * (Lights.InfoMatrix[index][2].xyz * intensity);

			//Specular light calculations
			vec3 reflectionDirection = normalize(-lightDirection - FragPosition);
			float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), Material.Data[3].x);
			specular = specular + ((spec * Material.Data[2].xyz) * (Lights.InfoMatrix[index][2].xyz * intensity));
		}
	}

	FragColour = vec4(ambient + diffuse + specular, Material.Data[3].y);
}