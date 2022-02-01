#pragma once
#include "../Engine/Core/Level.h"

class GO_Tetrahedron;

class L_TetrahedronLevel : public L_Level
{
public:
	L_TetrahedronLevel();
	virtual bool Initialize(BaseGame* game) override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	GO_Tetrahedron* T1 = nullptr;
	GO_Tetrahedron* T2 = nullptr;

	void PrintMinowskiDifference();
};

