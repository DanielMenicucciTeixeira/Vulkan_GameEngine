#version 450

//Light
//{
//	vec3 Position; vec3(Lights[index*offset + 0], Lights[index*offset + 1], Lights[index*offset + 2])
//	float Ambient; Lights[index*offset + 3]
//	float Diffuse; Lights[index*offset + 4]
//	float Specular; Lights[index*offset + 5]
//	vec3 Colour; vec3(Lights[index*offset + 6], Lights[index*offset + 7], Lights[index*offset + 8])
//	float TurnedOn; Lights[index*offset + 9]
//  float LightType; Lights[index*offset + 10]

in vec3 Normal;
in vec2 TextureCoords;
in vec3 FragPosition;

uniform sampler2D TextureDifuse;
//uniform sampler2D TextureSpecular;
uniform float Lights[330];
uniform int NumberOfLights;
uniform vec3 CameraPosition;


out vec4 FragColour;

void main()
{
	int offset = 11;
	vec3 ambient = vec3(0);
	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);

	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(CameraPosition - FragPosition);
	vec3 lightDirection;
	float intensity = 1;

	for(int index = 0; index < NumberOfLights; index++)
	{
		if(Lights[index*offset + 9] > 0.1f)
		{
			if(Lights[index*offset + 10] == 1) intensity = 1/pow(distance(vec3(Lights[index*offset + 0], Lights[index*offset + 1], Lights[index*offset + 2]), FragPosition), 2);
			else intensity = 1;

			if(Lights[index*offset + 10] == 2) lightDirection = vec3(Lights[index*offset + 0], Lights[index*offset + 1], Lights[index*offset + 2]);
			else lightDirection = normalize(vec3(Lights[index*offset + 0], Lights[index*offset + 1], Lights[index*offset + 2]) - FragPosition);
			
			//Ambient light calculations
			ambient = ambient + (Lights[index*offset + 3] * texture(TextureDifuse, TextureCoords).rgb * vec3(Lights[index*offset + 6], Lights[index*offset + 7], Lights[index*offset + 8])) * intensity;

			//Difuse light calculations
			float diff = max(dot(normal,lightDirection), 0.0);
			diffuse = diffuse + ((diff * Lights[index*offset + 4]) * texture(TextureDifuse, TextureCoords).rgb * vec3(Lights[index*offset + 6], Lights[index*offset + 7], Lights[index*offset + 8])) * intensity;

			//Specular light calculations
			vec3 reflectionDirection = normalize(-lightDirection - FragPosition);
			float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
			specular = specular + ((spec * Lights[index*offset + 5]) * vec3(Lights[index*offset + 6], Lights[index*offset + 7], Lights[index*offset + 8])) * intensity;
		}
	}

	FragColour = vec4(ambient + diffuse + specular, 1.0f);
}