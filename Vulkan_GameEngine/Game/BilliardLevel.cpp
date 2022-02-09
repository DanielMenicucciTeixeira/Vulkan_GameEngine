 #include "BilliardLevel.h"
#include "Math/FVector3.h"
#include "Math/FQuaternion.h"
#include "BilliardBall.h"
#include "BilliardTable.h"
#include <vector>
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Game.h"

L_BilliardLevel::L_BilliardLevel() : L_Level()
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
			Balls.push_back(SpawnGameObjectOfClass<GO_BilliardBall>(FTransform(initialPosition + displacement, FQuaternion(FVector3(0.0f, 0.0f, 1.0f), -45.0f), FVector3(0.5, 0.5, 0.5))));
		}
	}
	Balls.push_back(SpawnGameObjectOfClass<GO_BilliardBall>(FTransform(FVector3(0.0f, -4.0f, 0.0f), FQuaternion(), FVector3(0.5, 0.5, 0.5))));

	Balls[0]->Mesh->SetMaterialName("Ball01");
	Balls[1]->Mesh->SetMaterialName("Ball02");
	Balls[2]->Mesh->SetMaterialName("Ball03");
	Balls[3]->Mesh->SetMaterialName("Ball04");
	Balls[4]->Mesh->SetMaterialName("Ball05");
	Balls[5]->Mesh->SetMaterialName("Ball06");
	Balls[6]->Mesh->SetMaterialName("Ball07");
	Balls[7]->Mesh->SetMaterialName("Ball08");
	Balls[8]->Mesh->SetMaterialName("Ball09");
	Balls[9]->Mesh->SetMaterialName("Ball10");
	Balls[10]->Mesh->SetMaterialName("Ball11");
	Balls[11]->Mesh->SetMaterialName("Ball12");
	Balls[12]->Mesh->SetMaterialName("Ball13");
	Balls[13]->Mesh->SetMaterialName("Ball14");
	Balls[14]->Mesh->SetMaterialName("Ball15");
	Balls[15]->Mesh->SetMaterialName("BallCue");

	Balls[0]->AddTag("01");
	Balls[1]->AddTag("02");
	Balls[2]->AddTag("03");
	Balls[3]->AddTag("04");
	Balls[4]->AddTag("05");
	Balls[5]->AddTag("06");
	Balls[6]->AddTag("07");
	Balls[7]->AddTag("08");
	Balls[8]->AddTag("09");
	Balls[9]->AddTag("10");
	Balls[10]->AddTag("11");
	Balls[11]->AddTag("12");
	Balls[12]->AddTag("13");
	Balls[13]->AddTag("14");
	Balls[14]->AddTag("15");
	Balls[15]->AddTag("Cue");

	Balls[15]->Physics->SetVelocity(FVector3(0.0f, 2.0f, 0.0f));

	auto Table = SpawnGameObjectOfClass<GO_BilliardTable>(FTransform(FVector3(0.0f, 2.0f, -0.3f), FQuaternion(), FVector3(1.3, 1.3, 1.3)));

	LoadLevelObjects();
}
