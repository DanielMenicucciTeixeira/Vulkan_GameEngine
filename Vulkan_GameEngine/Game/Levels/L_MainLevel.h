#pragma once
#include "Level.h"
class L_MainLevel : public O_Level
{
public:
	L_MainLevel();
	virtual void Render() override;
	virtual void Update(float deltaTime) override;
	virtual void Start() override;
};

