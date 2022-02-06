#ifndef COREENGINE_H
#define COREENGINE_H

#include <memory>
#include <map>
#include <set>
#include "GameInterface.h"
#include "Core/DebugLogger.h"
#include "Clock.h"

typedef uint32_t sdlEventType;
typedef int32_t sdlKeycode;

class SDLManager;
class Renderer;
class BaseGame;
class L_Level;
class Window;
enum ERendererType;
union SDL_Event;
class FVector2;

struct S_InputFunctionPointers;
enum EInputSources;

class CoreEngine
{

public:

	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	bool Initialize(const char* name, ERendererType renderType, int width = -1, int height = -1, int positionX = -1, int positionY = -1);
	void Run();

	inline void Exit() { RunningEngine = false; };

	///Gettters
	static CoreEngine* GetInstance();
	inline Renderer* GetRenderer() { return EngineRenderer; }
	inline bool IsRunningEngine() const { return RunningEngine; }
	inline void SetGame(BaseGame* game) { CurrentGame = game; }
	inline BaseGame* GetGame() { return CurrentGame; }
	inline Clock* GetClock() { return &EngineClock; };


	inline void SetGameInterface(GameInterface* sceneManager) { gameInterface = sceneManager; };


	//Using a pair of the SDL_Event type and keycode (uint32_t, int32_t respectively), map a function pointer to the EngineInputFunctions map.
	//The maped function must be of static void type and have a self reference to the engine and an SDL_Event as it's only parameters.
	//Use SDLK_UNKNOWN for the keycode of non key or button functions. USE SDL_BUTTON or SDL_CONTROLLERBUTTON for keycode when applicable.
	void SetEngineInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(SDL_Event*));
	inline static void LoadGame(SDL_Event* event) { GetInstance()->StartGame(); }
	inline static void Quit(SDL_Event* event) { GetInstance()->StopEngine(); }
	void CleanUp();

	inline int GetCurrentScene() { return currentSceneNumber; };

	Window* GetWindow() {return engineWindow;};

	//Return window dimentions
	FVector2 GetWindowSize();

protected:
	void HandleEvents();
	void Update(const float deltaTime);
	void Render();
	bool StartGame();
	inline void StopEngine() { RunningEngine = false; }
	inline void StopGame() { RunningGame = false; }

	bool AddGameEvent(const char* eventName);
	bool RemoveGameEvent(const char* eventName);
	void AddInputsToGameEvent(const char* eventName, std::set<SDL_Event> events);
	void RemoveInputsFromGameEvent(const char* eventName, std::set<SDL_Event> events);

	Renderer* EngineRenderer;
	Window* engineWindow;
	BaseGame* CurrentGame;
	L_Level* StartingLevel;



	//Frame cap?
	unsigned int FramesPerSecond;
	bool RunningEngine;
	bool RunningGame;


	std::map<std::pair<sdlEventType, sdlKeycode>, void(*)(SDL_Event*)> EngineInputFunctions;


private:
	CoreEngine();
	~CoreEngine();

	//Engine to game interface
	GameInterface* gameInterface;

	int currentSceneNumber;

	Clock EngineClock;

	static std::unique_ptr<CoreEngine> Instance;
	friend std::default_delete<CoreEngine>;
};
#endif

