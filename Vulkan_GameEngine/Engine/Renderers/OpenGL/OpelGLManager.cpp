#include "OpelGLManager.h"
#include "OpenGLShaderManager.h"
#include "LevelGraph.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"
#include "SDL/SDLManager.h"

#include <SDL.h>
#include <iostream>

OpenGLManager::~OpenGLManager()
{
	CleanUp();
}

bool OpenGLManager::Initialize()
{
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	if (!ShaderManager)
	{
		ShaderManager = new OpenGLShaderManager();
		ShaderManager->CreateShaderProgram("TextureShader", "Engine/Shaders/TextureShader.vert", "Engine/Shaders/TextureShader.frag");
	}

	RenderData = LevelGraph::GetInstance()->GetRenderData();
	if (!RenderData)
	{
		DebugLogger::FatalError("Failed to get valid RenderData", "Renderers/OpenGL/OpenGLManager.cpp", __LINE__);
		return false;
	}

	VertexObjectsMap.clear();
	for (const auto mesh : RenderData->Meshes)
	{
		VertexObjectsMap[mesh] = S_BindingData();
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
	for (const auto mesh : RenderData->Meshes)
	{
		VertexObjectsMap[mesh] = S_BindingData();
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

	for (const auto& shader : RenderData->MaterialsByShader)
	{
		const auto& program = ShaderManager->GetShader(shader.first);
		glUseProgram(program);

		GLuint cameraBinding = 0, cameraIndex, cameraBuffer;
		cameraIndex = glGetUniformBlockIndex(program, "UniformCamera");
		glUniformBlockBinding(program, cameraIndex, cameraBinding);
		glGenBuffers(1, &cameraBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, cameraBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformCameraObject), RenderData->Camera, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, cameraBinding, cameraBuffer);

		GLuint modelBinding = 1, modelIndex, modelBuffer;
		modelIndex = glGetUniformBlockIndex(program, "UniformModel");
		glUniformBlockBinding(program, modelIndex, modelBinding);
		glGenBuffers(1, &modelBuffer);

		GLuint numberOfLightsBinding = 3, numberOfLightsIndex, numberOfLightsBuffer;
		numberOfLightsIndex = glGetUniformBlockIndex(program, "UniformNumberOfLights");
		glUniformBlockBinding(program, numberOfLightsIndex, numberOfLightsBinding);
		glGenBuffers(1, &numberOfLightsBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, numberOfLightsBuffer);
		int numberOfLights = RenderData->LightSources.size();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(int), &numberOfLights, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, numberOfLightsBinding, numberOfLightsBuffer);

		GLuint lightsBinding = 4, lightsIndex, lightsBuffer;
		glGenBuffers(1, &lightsBuffer);
		lightsIndex = glGetUniformBlockIndex(program, "UniformLights");
		glUniformBlockBinding(program, lightsIndex, lightsBinding);
		glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(FMatrix4) * numberOfLights, RenderData->LightSources.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, lightsBinding, lightsBuffer);

		for (const auto& material : shader.second)
		{
			GLuint materialBinding = 5, materialIndex, materialBuffer;
			materialIndex = glGetUniformBlockIndex(program, "UniformMaterial");
			glUniformBlockBinding(program, materialIndex, materialBinding);
			glGenBuffers(1, &materialBuffer);
			glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(FMatrix4), &material->Data, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, materialBinding, materialBuffer);

			const auto& difuseLocation = glGetUniformLocation(program, "TextureDifuse");
			glUniform1i(difuseLocation, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureMap[material->TextureDifuse]);

			glBindBuffer(GL_UNIFORM_BUFFER, modelBuffer);
			for (const auto& mesh : RenderData->MeshesByMaterial[material])
			{
				glBindVertexArray(VertexObjectsMap[mesh].Vertex);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexObjectsMap[mesh].IndexBuffer);

				for (const auto& model : RenderData->InstancesByMesh[mesh])
				{
					if (*RenderData->Models[model])
					{
						glBufferData(GL_UNIFORM_BUFFER, sizeof(FMatrix4), model, GL_DYNAMIC_DRAW);
						glBindBufferBase(GL_UNIFORM_BUFFER, modelBinding, modelBuffer);
						glDrawElements(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, (void*)0);
					}
				}
				glBindVertexArray(0);
			}
		}
		glUseProgram(0);
	}
	SDL_GL_SwapWindow(SDLManager::GetInstance()->GetSDLWindowByName());
}

void OpenGLManager::FramebufferResizeCallback()
{
	int w, h;
	SDL_GetWindowSize(SDLManager::GetInstance()->GetSDLWindowByName(), &w, &h);
	glViewport(0, 0, w, h);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(S_Vertex), (GLvoid*)offsetof(S_Vertex, Position));
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

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if(type != GL_DEBUG_TYPE_ERROR) return;
	fprintf(stderr, "GL CALLBACK: %s message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), message);
}
