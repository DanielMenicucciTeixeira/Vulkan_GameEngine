#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

class GameInterface {
public:
	GameInterface() {}
	virtual ~GameInterface() {}

	virtual bool OnCreate() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render() = 0;

};

#endif // !GAMEINTERFACE_H