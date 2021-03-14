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

	for (const auto mesh : RenderData->MeshToMaterialMap)
	{
		GenerateBuffers(mesh.first);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(VertexObjectsMap[mesh.first].first);

		for (const auto material : RenderData->MeshToMaterialMap[mesh.first])
		{
			glUseProgram(ShaderManager->GetShader(material->ShaderName));
			const auto modelLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ModelMatrix");
			const auto viewLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ViewMatrix");
			const auto projectionLocation = glGetUniformLocation(ShaderManager->GetShader(material->ShaderName), "ProjectionMatrix");
			for (const auto model : RenderData->MaterialToModelMap[material])
			{
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

void OpenGLManager::GenerateBuffers(S_Mesh* mesh)
{
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(S_Vertex), (GLvoid*)(offsetof(S_Vertex, TextureCoordinates)));
	//Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(S_Vertex), (GLvoid*)(offsetof(S_Vertex, Normal)));

	//Free Arrays and Buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
