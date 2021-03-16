#ifndef RENDERER_H
#define RENDERER_H

struct SDL_Window;
struct S_RenderData;

enum ERendererType
{
	VULKAN,
	OPEN_GL
};

class Renderer
{
public:
	Renderer() {}
	virtual ~Renderer() {}

	virtual bool Initialize(S_RenderData* initializationData) = 0;
	virtual  void CleanUp() = 0;
	virtual SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) = 0;
	virtual void Render(SDL_Window** windowArray = nullptr, unsigned int numberOfWindows = 1, unsigned int arrayOffset = 0) = 0;
	virtual void UpdateWithNewObjects() = 0;

	virtual void FramebufferResizeCallback() = 0;
	inline ERendererType GetType() const { return Type; }

protected:
	ERendererType Type;
};
#endif
