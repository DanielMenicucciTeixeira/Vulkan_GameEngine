#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

class CoreEngine;

class GameInterface
{
public:
	GameInterface() { GameEngine = nullptr; }
	virtual ~GameInterface() {}

	virtual bool Initialize(CoreEngine* gameEngine) = 0;
	virtual void HandleEvents() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Run() = 0;
	virtual void CleanUp() = 0;

protected:
	CoreEngine* GameEngine;
};

#endif
