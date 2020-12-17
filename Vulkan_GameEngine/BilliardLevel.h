#pragma once
#include "Level.h"

class Game;

class L_BilliardLevel : public O_Level
{
public:
	L_BilliardLevel(Game* game);
	void Start() override;
};

