#ifndef OPENGLMANAGER_H
#define OPENGLMANAGER_H

#include "Renderers/Renderer.h"
#include <vector>
#include <unordered_map>
#include <glew.h>

class Material;
struct ShaderVariableInfo;

class OpenGLShaderManager;
struct S_RenderData;
struct S_Mesh;
struct S_Material;
struct S_Texture;

struct S_BindingData
{
	unsigned int Vertex  = 0;
	unsigned int VertexBuffer = 0;
	unsigned int IndexBuffer = 0;
};

class OpenGLManager : public Renderer
{
public:
	~OpenGLManager(); 

	virtual bool Initialize() override;
	virtual void UpdateWithNewObjects() override;
	virtual  void CleanUp() override;
	virtual SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) override;
	virtual void Render(SDL_Window** windowArray = nullptr, unsigned int numberOfWindows = 1, unsigned int arrayOffset = 0) override;

	virtual void FramebufferResizeCallback() override;

protected:

	OpenGLShaderManager* ShaderManager;
	void CreateGLTexture(S_Texture* textureData);
	void GenerateBuffers(S_Mesh* mesh);
	std::unordered_map<S_Mesh*, S_BindingData> VertexObjectsMap;
	std::unordered_map<S_Texture*, unsigned int> TextureMap;
	std::vector<float> Lights;
	S_RenderData* RenderData;

	//Binds UniformBuffers and TextureSamplers from Material variables
	void HandleMaterial(Material* material, int currentBinding, const unsigned int& program);
	void BindTextureFromMaterial(ShaderVariableInfo info, void* data, const unsigned int& program);
	void BindUniformBufferFromMaterial(ShaderVariableInfo info, void* data, int currentBinding, const unsigned int& program);
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


#endif

