#ifndef L_MAINLEVEL_H
#define L_MAINLEVEL_H

#include "../Engine/Core/Level.h"

struct S_Mesh;

class L_MainLevel : public L_Level
{
public:
	L_MainLevel();
	virtual bool Initialize() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
};
#endif

