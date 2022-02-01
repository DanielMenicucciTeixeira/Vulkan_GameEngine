#pragma once
#include "../Engine/Core/Level.h"

class BaseGame;

class L_BilliardLevel : public L_Level
{
public:
	L_BilliardLevel(BaseGame* game);
	void Start() override;
};

