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
class Game;
class L_Level;
enum ERendererType;
union SDL_Event;

struct S_InputFunctionPointers;
enum EInputSources;

class CoreEngine
{

public:

	bool Initialize(const char* name, ERendererType renderType, int width = -1, int height = -1, int positionX = -1, int positionY = -1);
	void Run();

	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;


	///Gettters
	static CoreEngine* GetInstance();
	inline SDLManager* GetInterfaceManager() { return InterfaceManager; }
	inline Renderer* GetRenderer() { return EngineRenderer; }
	inline bool IsRunningEngine() const { return RunningEngine; }
	inline bool IsRunningGame() const { return RunningGame; }
	inline void SetGame(Game* game) { CurrentGame = game; }

	//Using a pair of the SDL_Event type and keycode (uint32_t, int32_t respectively), map a function pointer to the EngineInputFunctions map.
	//The maped function must be of static void type and have a self reference to the engine and an SDL_Event as it's only parameters.
	//Use SDLK_UNKNOWN for the keycode of non key or button functions. USE SDL_BUTTON or SDL_CONTROLLERBUTTON for keycode when applicable.
	void SetEngineInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(SDL_Event*));
	inline static void LoadGame(SDL_Event* event) { GetInstance()->StartGame(); }
	inline static void Quit(SDL_Event* event) { GetInstance()->StopEngine(); }
	void CleanUp();

protected:
	void HandleEvents();
	void Update(const float deltaTime);
	void Render();
	bool StartGame();
	inline void StopEngine() { RunningEngine = false; }
	inline void StopGame() { RunningGame = false; }

	Renderer* EngineRenderer;
	SDLManager* InterfaceManager;
	Window* EngineWindow;
	Clock* EngineClock;
	Game* CurrentGame;
	L_Level* StartingLevel;
	unsigned int FramesPerSecond;
	bool RunningEngine;
	bool RunningGame;
	std::map<std::pair<sdlEventType, sdlKeycode>, void(*)(SDL_Event*)> EngineInputFunctions;
	static std::unique_ptr<CoreEngine> Instance;
	friend std::default_delete<CoreEngine>;

private:
	CoreEngine();
	~CoreEngine();
};
#endif

