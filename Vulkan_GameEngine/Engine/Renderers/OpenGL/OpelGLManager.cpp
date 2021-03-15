#include "OpelGLManager.h"
#include "OpenGLShaderManager.h"
#include "Renderers/RenderInitializationData.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"
#include "SDL/SDLManager.h"
#include <SDL.h>
#include <glew.h>

OpenGLManager::~OpenGLManager()
{
	CleanUp();
}

bool OpenGLManager::Initialize(RenderInitializationData* initializationData)
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
		VertexObjectsMap[mesh.first] = std::make_pair<unsigned int, unsigned int>(0, 0);
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
		VertexObjectsMap[mesh.first] = std::make_pair<unsigned int, unsigned int>(0, 0);
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

	for (const auto mesh : RenderData->MeshToMaterialMap)
	{
		glBindVertexArray(VertexObjectsMap[mesh.first].first);

		for (const auto material : RenderData->MeshToMaterialMap[mesh.first])
		{
			glUseProgram(ShaderManager->GetShader(material->ShaderName));
			const auto modelLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ModelMatrix");
			const auto viewLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ViewMatrix");
			const auto projectionLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ProjectionMatrix");
			const auto difuseLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "TextureDifuse");
			const auto specularLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "TextureSpecular");
			for (const auto model : RenderData->MaterialToModelMap[material])
			{
				glUniform1i(difuseLocation, 0);
				glActiveTexture(GL_TEXTURE0);
				//auto checkGLuint = TextureMap[material->TextureDifuse];
				//auto checkTexture = material->TextureDifuse;
				glBindTexture(GL_TEXTURE_2D, TextureMap[material->TextureDifuse]);

				/*glUniform1i(specularLocation, 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, TextureMap[material->TextureSpecular]);*/

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model[0]);
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, RenderData->Camera->View);
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, RenderData->Camera->Projection);
				glDrawArrays(GL_TRIANGLES, 0, mesh.first->Vertices.size());
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

void OpenGLManager::GenerateBuffers(S_Mesh* mesh)
{
	if (VertexObjectsMap[mesh].first != 0) return;

	//Bind Arrays and Buffers
	glGenVertexArrays(1, &VertexObjectsMap[mesh].first);
	glGenBuffers(1, &VertexObjectsMap[mesh].second);
	glBindVertexArray(VertexObjectsMap[mesh].first);
	glBindBuffer(GL_ARRAY_BUFFER, VertexObjectsMap[mesh].second);
	glBufferData(GL_ARRAY_BUFFER, mesh->Vertices.size() * sizeof(S_Vertex), &mesh->Vertices[0], GL_STATIC_DRAW);

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
