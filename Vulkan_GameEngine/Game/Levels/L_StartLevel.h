#ifndef L_STARTLEVEL_H
#define L_STARTLEVEL_H
#include "../Engine/Core/Level.h"
class L_StartLevel : public L_Level
{
public:
	L_StartLevel();
	virtual bool Initialize();
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};
#endif

