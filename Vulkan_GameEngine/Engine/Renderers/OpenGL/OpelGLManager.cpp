#include "OpelGLManager.h"
#include "Renderers/RenderInitializationData.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"
#include <glew.h>

bool OpelGLManager::Initialize(RenderInitializationData* initializationData)
{
	if (!initializationData)
	{
		DebugLogger::FatalError("Failed to get valid RenderInitializationData", "Renderers/OpenGL/OpenGLManager.cpp", __LINE__);
		return false;
	}
	RenderData = initializationData;

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

void OpelGLManager::CleanUp()
{
}

SDL_Window* OpelGLManager::CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY)
{
	return nullptr;
}

void OpelGLManager::Render(SDL_Window** windowArray, unsigned int numberOfWindows, unsigned int arrayOffset)
{

	for (auto mesh : VertexObjectsMap)
	{
		GenerateBuffers(mesh.first);
	}
	for (const auto mesh : RenderData->MeshToMaterialMap)
	{
		glBindVertexArray(VertexObjectsMap[mesh.first].first);
		auto test = VertexObjectsMap[mesh.first].first;

		for (const auto material : RenderData->MeshToMaterialMap[mesh.first])
		{
			for (const auto model : RenderData->MaterialToModelMap[material])
			{
				glDrawArrays(GL_TRIANGLES, 0, mesh.first->Vertices.size());
			}
		}

		glBindVertexArray(0);
	}
}

void OpelGLManager::FramebufferResizeCallback()
{
}

void OpelGLManager::GenerateBuffers(S_Mesh* mesh)
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
