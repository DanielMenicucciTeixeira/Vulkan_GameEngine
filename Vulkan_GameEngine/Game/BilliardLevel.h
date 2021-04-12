#pragma once
#include "Level.h"

class BaseGame;

class L_BilliardLevel : public L_Level
{
public:
	L_BilliardLevel(BaseGame* game);
	void Start() override;
};

