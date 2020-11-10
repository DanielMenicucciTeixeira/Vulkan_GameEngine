#ifndef RENDERER_H
#define RENDERER_H

class SDL_Window;

class Renderer
{
public:
	virtual void Run() = 0;

protected:
	virtual void Initialize() = 0;
	virtual  void CleanUp() = 0;
	virtual SDL_Window* CreateWindow(const char* windowName, float windowSizeX, float windowSizeY, float windowPositionX, float windowPositionY) = 0;
	virtual void Render() = 0;

};
#endif
