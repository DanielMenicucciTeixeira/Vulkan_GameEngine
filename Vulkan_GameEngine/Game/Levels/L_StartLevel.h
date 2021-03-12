#ifndef L_STARTLEVEL_H
#define L_STARTLEVEL_H
#include "Level.h"
class L_StartLevel : public O_Level
{
public:
	L_StartLevel();
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};
#endif

