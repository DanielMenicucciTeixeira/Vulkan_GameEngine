#pragma once
#include "../Engine/Core/Level.h"

class GO_Tetrahedron;
class GO_Wall;
class GO_Skybox;

class L_TetrahedronLevel : public L_Level
{
public:
	L_TetrahedronLevel();
	virtual bool Initialize() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	GO_Tetrahedron* T1 = nullptr;
	GO_Tetrahedron* T2 = nullptr;
	GO_Skybox* Skybox = nullptr;

	void PrintMinowskiDifference();
};

