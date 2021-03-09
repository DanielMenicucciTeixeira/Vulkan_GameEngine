#ifndef COREENGINE_H
#define COREENGINE_H

#include <memory>

class SDLManager;
class Window;
class Clock;
class Renderer;
enum ERendererType;

class CoreEngine
{
public:
	///Deleted Functions, help make sure there will only be 1 engine object (Singleton Design)
	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	static CoreEngine* GetInstance();

	bool OnCreate(const char* name, ERendererType renderType, int width = -1, int height = -1, int positionX = -1, int positionY = -1);
	void Run();

	///Gettters
	inline bool IsRunning() const { return Running; }

protected:
	void HandleEvents();
	void Update(const float deltaTime);
	void Render();
	void OnDestroy();

	Renderer* EngineRenderer;
	SDLManager* InterfaceManager;
	Window* EngineWindow;
	Clock* EngineClock;
	bool Running;

private:
	CoreEngine();
	~CoreEngine();

	static std::unique_ptr<CoreEngine> Instance;
	friend std::default_delete<CoreEngine>;
};
#endif

