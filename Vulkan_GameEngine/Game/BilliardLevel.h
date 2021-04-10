#pragma once
#include "Level.h"

class Game;

class L_BilliardLevel : public L_Level
{
public:
	L_BilliardLevel(Game* game);
	void Start() override;
};

