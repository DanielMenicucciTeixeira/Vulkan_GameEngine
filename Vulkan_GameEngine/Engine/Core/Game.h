#ifndef GAME_H
#define GAME_H

#include "Auxiliary/Singleton.h"
#include "DebugLogger.h"

#include <set>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>


class Clock;
class O_Object;
class O_GameObject;
class O_Level;
class Renderer;
class SDLManager;
struct S_RenderData;

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

	virtual bool Initialize(Renderer* gameRenderer);
	virtual void HandleEvents();
	virtual void Update(const float deltaTime);
	virtual void Render();

	//Using a pair of the SDL_Event type and keycode (uint32_t, int32_t respectively), map a function pointer to the GameInputFunctions map.
	//The maped function must be of static void type and have a self reference to the engine and an SDL_Event as it's only parameters.
	//Use SDLK_UNKNOWN for the keycode of non key or button functions. USE SDL_BUTTON or SDL_CONTROLLERBUTTON for keycode when applicable.
	void SetGameInputFunction(sdlEventType eventType, sdlKeycode keycode, static void(*function)(Game*, SDL_Event*));
	inline static void QuitGame(Game* self, SDL_Event* event) { self->SetRunning(false); }
	Renderer* GetRenderer();

	void SetPause(const bool& pause);
	inline bool IsPaused() const  { return Paused; };
	inline bool IsRunning() const { return Running; }
	inline void SetRunning(bool isRunning) { Running = isRunning; }
	int Run();
	
	//Loaders

	template<class levelClass>
	bool StartNewLevel()
	{
		static_assert(std::is_base_of<O_Level, levelClass>::value, "levelClass must derive from O_Level!");
		auto nextLevel = new levelClass;
		if (dynamic_cast<O_Level*>(nextLevel))
		{
			NextLevel = nextLevel;
			ShouldStartNewLevel = true;
			return true;
		}
		else
		{
			DebugLogger::Error("Invalid Level class: " + std::string(typeid(levelClass).name()), "Core/Game.h", __LINE__);
		}
	}

	//Getters

	inline O_Level* GetCurrentLevel() { return CurrentLevel; }
	inline SDLManager* GetInterfaceManager();
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
	Renderer* GameRenderer;

	std::map<std::pair<sdlEventType, sdlKeycode>, void(*)(Game*, SDL_Event*)> GameInputFunctions;

	unsigned int FramesPerSecond;
	bool Paused;
	bool Running;
	bool ShouldStartNewLevel;
};

#endif

