#include "OpelGLManager.h"
#include "OpenGLShaderManager.h"
#include "Renderers/RenderInitializationData.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"
#include "SDL/SDLManager.h"
#include <SDL.h>
#include <glew.h>
#include "FX/LightInfo.h"

OpenGLManager::~OpenGLManager()
{
	CleanUp();
}

bool OpenGLManager::Initialize(S_RenderData* initializationData)
{
	if (!ShaderManager)
	{
		ShaderManager = new OpenGLShaderManager();
		ShaderManager->CreateShaderProgram("ColourShader", "Engine/Shaders/ColourShader.vert", "Engine/Shaders/ColourShader.frag");
		ShaderManager->CreateShaderProgram("TextureShader", "Engine/Shaders/TextureShader.vert", "Engine/Shaders/TextureShader.frag");
	}

	if (!initializationData)
	{
		DebugLogger::FatalError("Failed to get valid RenderInitializationData", "Renderers/OpenGL/OpenGLManager.cpp", __LINE__);
		return false;
	}
	RenderData = initializationData;

	VertexObjectsMap.clear();
	for (const auto mesh : RenderData->MeshToMaterialMap)
	{
		VertexObjectsMap[mesh.first] = S_BindingData();
	}

	for (auto mesh : VertexObjectsMap)
	{
		GenerateBuffers(mesh.first);
	}

	return true;
}

void OpenGLManager::UpdateWithNewObjects()
{
	VertexObjectsMap.clear();
	for (const auto mesh : RenderData->MeshToMaterialMap)
	{
		VertexObjectsMap[mesh.first] = S_BindingData();
	}

	for (auto mesh : VertexObjectsMap)
	{
		GenerateBuffers(mesh.first);
	}

	for (auto& texture : RenderData->Textures)
	{
		CreateGLTexture(texture);
	}
}

void OpenGLManager::CleanUp()
{
	if (ShaderManager) delete(ShaderManager);
}

SDL_Window* OpenGLManager::CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY)
{
	return nullptr;
}

void OpenGLManager::Render(SDL_Window** windowArray, unsigned int numberOfWindows, unsigned int arrayOffset)
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	PopulateLigthsVector();
	for (const auto mesh : RenderData->MeshToMaterialMap)
	{
		glBindVertexArray(VertexObjectsMap[mesh.first].Vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexObjectsMap[mesh.first].IndexBuffer);

		for (const auto material : RenderData->MeshToMaterialMap[mesh.first])
		{
			glUseProgram(ShaderManager->GetShader(material->ShaderName));
			
			const auto modelLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ModelMatrix");
			const auto viewLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ViewMatrix");
			const auto projectionLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ProjectionMatrix");
			const auto cameraPositionLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "CameraPosition");
			const auto difuseLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "TextureDifuse");
			//const auto specularLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "TextureSpecular");
			const auto lightsLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "Lights");
			const auto numberOfLightsLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "NumberOfLights");
			
			glUniform1i(difuseLocation, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureMap[material->TextureDifuse]);

			/*glUniform1i(specularLocation, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, TextureMap[material->TextureSpecular]);*/

			glUniform1i(numberOfLightsLocation, GLint(RenderData->LightSources.size()));
			glUniform1fv(lightsLocation, GLsizei(Lights.size()), Lights.data());
			glUniform3fv(cameraPositionLocation, 1, RenderData->Camera->Position);
			for (const auto model : RenderData->MaterialToModelMap[material])
			{
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, RenderData->Camera->View);
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, RenderData->Camera->Projection);
				glDrawElements(GL_TRIANGLES, mesh.first->Indices.size(), GL_UNSIGNED_INT, (void*)0);
			}
			glUseProgram(0);
		}

		glBindVertexArray(0);
	}
	SDL_GL_SwapWindow(SDLManager::GetInstance()->GetSDLWindowByName());
}

void OpenGLManager::FramebufferResizeCallback()
{
}

void OpenGLManager::CreateGLTexture(S_Texture* textureData)
{
	//if (TextureMap.find(textureData) != TextureMap.end()) return;

	glGenTextures(1, &TextureMap[textureData]);
	glBindTexture(GL_TEXTURE_2D, TextureMap[textureData]);
	int mode = textureData->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureData->Width, textureData->Height, 0, mode, GL_UNSIGNED_BYTE, textureData->Pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLManager::PopulateLigthsVector()
{
	//TODO look at Observer design pattern to fix this
	if (Lights.size() > 0) Lights.clear();
	Lights = std::vector<float>();
	Lights.reserve(RenderData->LightSources.size() * sizeof(RenderData->LightSources) / sizeof(float));
	for (const auto& light : RenderData->LightSources)
	{
		Lights.push_back(light->Position.X);
		Lights.push_back(light->Position.Y);
		Lights.push_back(light->Position.Z);

		Lights.push_back(light->Ambient);
		Lights.push_back(light->Diffuse);
		Lights.push_back(light->Specular);

		Lights.push_back(light->Colour.X);
		Lights.push_back(light->Colour.Y);
		Lights.push_back(light->Colour.Z);

		Lights.push_back(light->TurnedOn);

		Lights.push_back(light->LightType);
	}
}

void OpenGLManager::GenerateBuffers(S_Mesh* mesh)
{
	if (VertexObjectsMap[mesh].Vertex != 0) return;

	//Bind Vertex Arrays and Buffers
	glGenVertexArrays(1, &VertexObjectsMap[mesh].Vertex);
	glGenBuffers(1, &VertexObjectsMap[mesh].VertexBuffer);
	glBindVertexArray(VertexObjectsMap[mesh].Vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VertexObjectsMap[mesh].VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->Vertices.size() * sizeof(S_Vertex), &mesh->Vertices[0], GL_STATIC_DRAW);

	//Bind Index Arrays and Buffers
	glGenBuffers(1, &VertexObjectsMap[mesh].IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexObjectsMap[mesh].IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Indices.size() * sizeof(unsigned int), &mesh->Indices[0], GL_STATIC_DRAW);

	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(S_Vertex), static_cast<GLvoid*>(0));
	//TextureCoordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(S_Vertex), (GLvoid*)(offsetof(S_Vertex, TextureCoordinates)));
	//Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(S_Vertex), (GLvoid*)(offsetof(S_Vertex, Normal)));

	//Free Arrays and Buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
