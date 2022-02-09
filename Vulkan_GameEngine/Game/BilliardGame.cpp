#include "BilliardGame.h"
#include "Renderers/RenderObject.h"
#include "BilliardLevel.h"

BilliardGame::BilliardGame()
{
	/*
	CurrentLevel = new L_BilliardLevel(this);
	Levels["BilliardLevel"] = CurrentLevel;

	Materials["Ball01"] = new S_Material();
	Materials["Ball01"]->Name = "Ball01";
	Materials["Ball01"]->TextureDifuse->Path = "Game/Textures/Ball01.jpg";

	Materials["Ball02"] = new S_Material();
	Materials["Ball02"]->Name = "Ball02";
	Materials["Ball02"]->TextureDifuse->Path = "Game/Textures/Ball02.jpg";

	Materials["Ball03"] = new S_Material();
	Materials["Ball03"]->Name = "Ball03";
	Materials["Ball03"]->TextureDifuse->Path = "Game/Textures/Ball03.jpg";

	Materials["Ball04"] = new S_Material();
	Materials["Ball04"]->Name = "Ball04";
	Materials["Ball04"]->TextureDifuse->Path = "Game/Textures/Ball04.jpg";
	
	Materials["Ball05"] = new S_Material();
	Materials["Ball05"]->Name = "Ball05";
	Materials["Ball05"]->TextureDifuse->Path = "Game/Textures/Ball05.jpg";

	Materials["Ball06"] = new S_Material();
	Materials["Ball06"]->Name = "Ball06";
	Materials["Ball06"]->TextureDifuse->Path = "Game/Textures/Ball06.jpg";

	Materials["Ball07"] = new S_Material();
	Materials["Ball07"]->Name = "Ball07";
	Materials["Ball07"]->TextureDifuse->Path = "Game/Textures/Ball07.jpg";

	Materials["Ball08"] = new S_Material();
	Materials["Ball08"]->Name = "Ball08";
	Materials["Ball08"]->TextureDifuse->Path = "Game/Textures/Ball08.jpg";

	Materials["Ball09"] = new S_Material();
	Materials["Ball09"]->Name = "Ball09";
	Materials["Ball09"]->TextureDifuse->Path = "Game/Textures/Ball09.jpg";

	Materials["Ball10"] = new S_Material();
	Materials["Ball10"]->Name = "Ball10";
	Materials["Ball10"]->TextureDifuse->Path = "Game/Textures/Ball10.jpg";

	Materials["Ball11"] = new S_Material();
	Materials["Ball11"]->Name = "Ball11";
	Materials["Ball11"]->TextureDifuse->Path = "Game/Textures/Ball11.jpg";

	Materials["Ball12"] = new S_Material();
	Materials["Ball12"]->Name = "Ball12";
	Materials["Ball12"]->TextureDifuse->Path = "Game/Textures/Ball12.jpg";

	Materials["Ball13"] = new S_Material();
	Materials["Ball13"]->Name = "Ball13";
	Materials["Ball13"]->TextureDifuse->Path = "Game/Textures/Ball13.jpg";

	Materials["Ball14"] = new S_Material();
	Materials["Ball14"]->Name = "Ball14";
	Materials["Ball14"]->TextureDifuse->Path = "Game/Textures/Ball14.jpg";

	Materials["Ball15"] = new S_Material();
	Materials["Ball15"]->Name = "Ball15";
	Materials["Ball15"]->TextureDifuse->Path = "Game/Textures/Ball15.jpg";

	Materials["BallCue"] = new S_Material();
	Materials["BallCue"]->Name = "BallCue";
	Materials["BallCue"]->TextureDifuse->Path = "Game/Textures/BallCue.jpg";

	Materials["Wood"] = new S_Material();
	Materials["Wood"]->Name = "Wood";
	Materials["Wood"]->TextureDifuse->Path = "Game/Textures/wood.jpg";


	Meshes["SphereMesh"] = new S_Mesh();
	Meshes["SphereMesh"]->Name = "SphereMesh";
	Meshes["SphereMesh"]->Path = "Game/Models/sphere.obj";

	Meshes["BilliardTable"] = new S_Mesh();
	Meshes["BilliardTable"]->Name = "BilliardTable";
	Meshes["BilliardTable"]->Path = "Game/Models/BilliardTable.obj";

	for (auto& material : Materials) LoadMaterial(material.second);
	for (auto& mesh : Meshes) LoadMesh(mesh.second);
	int debug = 0;
	*/
}

void BilliardGame::Update(float deltaTime_)
{
}

void BilliardGame::Render()
{
}
