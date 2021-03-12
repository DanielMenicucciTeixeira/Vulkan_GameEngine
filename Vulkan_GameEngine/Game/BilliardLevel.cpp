#include "BilliardLevel.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "BilliardBall.h"
#include "BilliardTable.h"
#include <vector>
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Game.h"

L_BilliardLevel::L_BilliardLevel(Game* game) : O_Level()
{
}

void L_BilliardLevel::Start()
{
	FVector3 initialPosition = FVector3(0.0f, 1.8f, 0.0f);
	FVector3 colunm = FVector3(0.3f, 0.0f, 0.0f);
	FVector3 row = FVector3(0.0f, 0.3f, 0.0f);
	FVector3 displacement;
	std::vector<GO_BilliardBall*> Balls;
	for (int i = 0; i <= 4; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			displacement = (row * i) + (colunm * (-i + (2.0f * j)));
			Balls.push_back(SpawnGameObjectOfClass<GO_BilliardBall>(FTransform(initialPosition + displacement, FQuaternion(FVector3(0.0f, 0.0f, 1.0f), -45), FVector3(0.5, 0.5, 0.5))));
		}
	}
	Balls.push_back(SpawnGameObjectOfClass<GO_BilliardBall>(FTransform(FVector3(0.0f, -4.0f, 0.0f), FQuaternion(), FVector3(0.5, 0.5, 0.5))));

	Balls[0]->Mesh->SetTextureName("Ball01");
	Balls[1]->Mesh->SetTextureName("Ball02");
	Balls[2]->Mesh->SetTextureName("Ball03");
	Balls[3]->Mesh->SetTextureName("Ball04");
	Balls[4]->Mesh->SetTextureName("Ball05");
	Balls[5]->Mesh->SetTextureName("Ball06");
	Balls[6]->Mesh->SetTextureName("Ball07");
	Balls[7]->Mesh->SetTextureName("Ball08");
	Balls[8]->Mesh->SetTextureName("Ball09");
	Balls[9]->Mesh->SetTextureName("Ball10");
	Balls[10]->Mesh->SetTextureName("Ball11");
	Balls[11]->Mesh->SetTextureName("Ball12");
	Balls[12]->Mesh->SetTextureName("Ball13");
	Balls[13]->Mesh->SetTextureName("Ball14");
	Balls[14]->Mesh->SetTextureName("Ball15");
	Balls[15]->Mesh->SetTextureName("BallCue");

	Balls[0]->SetName("01");
	Balls[1]->SetName("02");
	Balls[2]->SetName("03");
	Balls[3]->SetName("04");
	Balls[4]->SetName("05");
	Balls[5]->SetName("06");
	Balls[6]->SetName("07");
	Balls[7]->SetName("08");
	Balls[8]->SetName("09");
	Balls[9]->SetName("10");
	Balls[10]->SetName("11");
	Balls[11]->SetName("12");
	Balls[12]->SetName("13");
	Balls[13]->SetName("14");
	Balls[14]->SetName("15");
	Balls[15]->SetName("Cue");

	Balls[15]->Physics->SetVelocity(FVector3(0.0f, 2.0f, 0.0f));

	auto Table = SpawnGameObjectOfClass<GO_BilliardTable>(FTransform(FVector3(0.0f, 2.0f, -0.3f), FQuaternion(), FVector3(1.3, 1.3, 1.3)));

	LevelObjects;
	LoadLevelObjects();
}
