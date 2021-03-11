#ifndef COREENGINE_H
#define COREENGINE_H

#include <memory>
#include <map>

typedef uint32_t sdlEventType;
typedef int32_t sdlKeycode;

class SDLManager;
class Window;
class Clock;
class Renderer;
class GameInterface;
enum ERendererType;
union SDL_Event;;

struct S_InputFunctionPointers;
enum EInputSources;

class CoreEngine
{
//Uses a define macro to make this class a Singleton (only one instance of it can exist)
#define singletonClass CoreEngine
#include "Auxiliary/Singleton.h"
//---------------------------------------------------------------------------------------------

public:

	bool Initialize(const char* name, ERendererType renderType, int width = -1, int height = -1, int positionX = -1, int positionY = -1);
	void Run();

	///Gettters

	inline bool IsRunningEngine() const { return RunningEngine; }
	inline bool IsRunningGame() const { return RunningGame; }
	inline void SetGame(GameInterface* game) { Game = game; }

	//Using a pair of the SDL_Event type and keycode (uint32_t, int32_t respectively), map a function pointer to the EngineInputFunctions map.
	//The maped function must be of static void type and have a self reference to the engine as it's only parameter
	void SetEngineInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(CoreEngine*));
	inline static void LoadGame(CoreEngine* self) { self->StartGame(); }
	inline static void Quit(CoreEngine* self) { self->CleanUp(); }
	void CleanUp();

protected:
	void HandleEvents();
	void Update(const float deltaTime);
	void Render();
	bool StartGame();
	inline void StopGame() { RunningGame = false; }

	Renderer* EngineRenderer;
	SDLManager* InterfaceManager;
	Window* EngineWindow;
	Clock* EngineClock;
	GameInterface* Game;
	unsigned int FramesPerSecond;
	bool RunningEngine;
	bool RunningGame;
	std::map<std::pair<sdlEventType, sdlKeycode>, void(*)(CoreEngine*)> EngineInputFunctions;

private:
	CoreEngine();
	~CoreEngine();
};
#endif

