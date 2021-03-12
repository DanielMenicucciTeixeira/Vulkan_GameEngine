#ifndef GAME_H
#define GAME_H

#include "Auxiliary/Singleton.h"

#include <set>
#include <unordered_map>
#include <map>
#include <string>


class Clock;
class O_Object;
class O_GameObject;
class O_Level;
class Renderer;
class SDLManager;

union SDL_Event;

typedef uint32_t sdlEventType;
typedef int32_t sdlKeycode;

class Game
{
public:
	Game();
	virtual ~Game();

	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

	virtual bool Initialize(SDLManager* interfaceManager, Renderer* gameRenderer);
	virtual void HandleEvents();
	virtual void Update(const float deltaTime);
	virtual void Render();

	//Using a pair of the SDL_Event type and keycode (uint32_t, int32_t respectively), map a function pointer to the GameInputFunctions map.
	//The maped function must be of static void type and have a self reference to the engine and an SDL_Event as it's only parameters.
	//Use SDLK_UNKNOWN for the keycode of non key or button functions. USE SDL_BUTTON or SDL_CONTROLLERBUTTON for keycode when applicable.
	void SetGameInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(Game*, SDL_Event*));
	inline static void QuitGame(Game* self, SDL_Event* event) { self->SetRunning(false); }
	inline Renderer* GetRenderer() { return GameRenderer; }

	void SetPause(const bool& pause);
	inline bool IsPaused() const  { return Paused; };
	inline bool IsRunning() const { return Running; }
	inline void SetRunning(bool isRunning) { Running = isRunning; }
	int Run();
	
	//Loaders

	void StartNewLevel(O_Level* level);
	void StartNewLevel(std::string levelName);
	inline void AddLevel(std::string levelName, O_Level* level) { Levels[levelName] = level; }

	//Getters

	inline O_Level* GetCurrentLevel() { return CurrentLevel; }
	inline SDLManager* GetInterfaceManager() { return InterfaceManager; }
	float GetTimeSeconds();
	float GetDeltaTimeSeconds();
	float GetDeltaTimeMiliseconds();
	float GetSleepTime();
	inline float GetFramesPerSecond() { return FramesPerSecond; }

protected:
	void SetCurrentLevel();
	void CleanUp();

	Clock* GameClock;
	O_Level* CurrentLevel;
	O_Level* NextLevel;
	SDLManager* InterfaceManager;
	Renderer* GameRenderer;
	
	std::unordered_map<std::string, O_Level*> Levels;

	std::map<std::pair<sdlEventType, sdlKeycode>, void(*)(Game*, SDL_Event*)> GameInputFunctions;

	unsigned int FramesPerSecond;
	bool Paused;
	bool Running;
	bool ShouldStartNewLevel;
};

#endif

