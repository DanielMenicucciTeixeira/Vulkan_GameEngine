#ifndef L_MAINLEVEL_H
#define L_MAINLEVEL_H

#include "Level.h"

struct S_Mesh;

class L_MainLevel : public O_Level
{
public:
	L_MainLevel();
	virtual bool Initialize(Game* game) override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};
#endif

