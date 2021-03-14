#ifndef OPENGLMANAGER_H
#define OPENGLMANAGER_H
#include "Renderers/Renderer.h"
#include "Renderers/RenderObject.h"
#include <vector>
#include <unordered_map>

class OpenGLShaderManager;

class OpenGLManager : public Renderer
{
public:
	~OpenGLManager();

	virtual bool Initialize(RenderInitializationData* initializationData) override;
	virtual  void CleanUp() override;
	virtual SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) override;
	virtual void Render(SDL_Window** windowArray = nullptr, unsigned int numberOfWindows = 1, unsigned int arrayOffset = 0) override;

	virtual void FramebufferResizeCallback() override;

protected:
	OpenGLShaderManager* ShaderManager;

	void GenerateBuffers(S_Mesh* mesh);
	std::unordered_map<S_Mesh*, std::pair<unsigned int, unsigned int>> VertexObjectsMap;
	RenderInitializationData* RenderData;
};
#endif

