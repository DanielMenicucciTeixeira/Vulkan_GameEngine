#ifndef OPENGLMANAGER_H
#define OPENGLMANAGER_H
#include "Renderers/Renderer.h"
#include <vector>
#include <unordered_map>

class OpenGLShaderManager;
struct S_RenderData;
struct S_Mesh;
struct S_Material;
struct S_Texture;

class OpenGLManager : public Renderer
{
public:
	~OpenGLManager(); 

	virtual bool Initialize(S_RenderData* initializationData) override;
	virtual void UpdateWithNewObjects() override;
	virtual  void CleanUp() override;
	virtual SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) override;
	virtual void Render(SDL_Window** windowArray = nullptr, unsigned int numberOfWindows = 1, unsigned int arrayOffset = 0) override;

	virtual void FramebufferResizeCallback() override;

protected:
	OpenGLShaderManager* ShaderManager;
	void CreateGLTexture(S_Texture* textureData);
	void PopulateLigthsVector();
	

	void GenerateBuffers(S_Mesh* mesh);
	std::unordered_map<S_Mesh*, std::pair<unsigned int, unsigned int>> VertexObjectsMap;
	std::unordered_map<S_Texture*, unsigned int> TextureMap;
	std::vector<float> Lights;
	S_RenderData* RenderData;
};
#endif

