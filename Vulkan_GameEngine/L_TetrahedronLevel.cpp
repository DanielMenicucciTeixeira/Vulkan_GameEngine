#include "L_TetrahedronLevel.h"


#include "GO_Triangle.h"
#include "Apple.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Renderer.h"
#include "Math/FQuaternion.h"
#include "Objects/GameObjects/GO_Camera.h"
#include "FX/LightSource.h"
#include "FX/DirectionalLight.h"
#include "Graphics/AssetLoader.h"
#include "LevelGraph.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Pawn.h"
#include "Tetrahedron.h"
#include "Math/FQuaternion.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Renderers/Materials/StandardMaterial.h"
#include "Renderers/Materials/VulkanSkyboxMaterial.h"
#include "Objects/GameObjects/Skybox.h"
#include "Game/Wall.h"

#include "../Audio/BackgroundSound.h"
#include "../Audio/AudioSource.h"

#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Game/Ball.h"

L_TetrahedronLevel::L_TetrahedronLevel() : L_Level(1000.0f)
{
	Name = "Tetrahedron Level";
}

bool L_TetrahedronLevel::Initialize()
{
	printf("\n\n---------------------------------------Tetrahedron Initialized!----------------------------------------\n\n");

	S_Texture* tetrahedron_Texture = new S_Texture();
	tetrahedron_Texture->Name = "PoolTable";
	tetrahedron_Texture->Path = "Assets/Textures/TableFrame.png";
	LevelGraph::GetInstance()->AddTexture(tetrahedron_Texture);
	LoadTexture(tetrahedron_Texture, tetrahedron_Texture->Name);

	S_Texture* tableSurface_Texture = new S_Texture();
	tableSurface_Texture->Name = "TableSurface";
	tableSurface_Texture->Path = "Assets/Textures/TableSurface.png";
	LevelGraph::GetInstance()->AddTexture(tableSurface_Texture);
	LoadTexture(tableSurface_Texture, tableSurface_Texture->Name);

	S_Texture* box_Texture = new S_Texture();

	box_Texture->Name = "PoolBall";
	box_Texture->Path = "Assets/Textures/1.jpeg";

	LevelGraph::GetInstance()->AddTexture(box_Texture);
	LoadTexture(box_Texture, box_Texture->Name);



	S_CubeSampler* skybox_Sampler = new S_CubeSampler();
	skybox_Sampler->Name = "SkyboxSampler";
	skybox_Sampler->Textures[0]->Name = "Front";
	skybox_Sampler->Textures[0]->Path = "Assets/Textures/skybox/front.jpg";
	skybox_Sampler->Textures[1]->Name = "Back";
	skybox_Sampler->Textures[1]->Path = "Assets/Textures/skybox/back.jpg";
	skybox_Sampler->Textures[2]->Name = "Top";
	skybox_Sampler->Textures[2]->Path = "Assets/Textures/skybox/top.jpg";
	skybox_Sampler->Textures[3]->Name = "Bottom";
	skybox_Sampler->Textures[3]->Path = "Assets/Textures/skybox/bottom.jpg";
	skybox_Sampler->Textures[4]->Name = "Right";
	skybox_Sampler->Textures[4]->Path = "Assets/Textures/skybox/left.jpg";
	skybox_Sampler->Textures[5]->Name = "Left";
	skybox_Sampler->Textures[5]->Path = "Assets/Textures/skybox/right.jpg";
	LevelGraph::GetInstance()->AddCubeSampler(skybox_Sampler);
	LoadCubeSampler(skybox_Sampler, skybox_Sampler->Name);

	ModelPaths.insert("Assets/Models/PoolTableV2.obj");
	ModelPaths.insert("Assets/Models/PoolBall.obj");
	ModelPaths.insert("Assets/Models/Dice.obj");
	LoadModels();
	MaterialPaths.insert("Assets/Materials/PoolTableV2.mtl");
	MaterialPaths.insert("Assets/Materials/PoolBall.mtl");
	MaterialPaths.insert("Assets/Materials/Dice.mtl");
	LoadMaterialLibrary();

	M_StandardMaterial* standardMaterial = dynamic_cast<M_StandardMaterial*>(LevelGraph::GetInstance()->GetMaterials()["M_Brown"]);
	standardMaterial->DiffuseTexture = tetrahedron_Texture;
	standardMaterial->SpecularTexture = tetrahedron_Texture;

	M_StandardMaterial* tableSurfaceMaterial = dynamic_cast<M_StandardMaterial*>(LevelGraph::GetInstance()->GetMaterials()["M_Green"]);
	tableSurfaceMaterial->DiffuseTexture = tableSurface_Texture;
	tableSurfaceMaterial->SpecularTexture = tableSurface_Texture;

	M_StandardMaterial* boxMaterial = dynamic_cast<M_StandardMaterial*>(LevelGraph::GetInstance()->GetMaterials()["M_diceTexture"]);
	boxMaterial->DiffuseTexture = box_Texture;
	boxMaterial->SpecularTexture = box_Texture;

	M_StandardMaterial* floorMaterial = dynamic_cast<M_StandardMaterial*>(LevelGraph::GetInstance()->GetMaterials()["M_PoolBall"]);
	floorMaterial->DiffuseTexture = box_Texture;
	floorMaterial->SpecularTexture = box_Texture;

	MV_SkyboxMaterial* skyboxMaterial = new MV_SkyboxMaterial();
	skyboxMaterial->CubeMap = skybox_Sampler;
	LevelGraph::GetInstance()->AddMaterial(skyboxMaterial);

	return L_Level::Initialize();
}

void L_TetrahedronLevel::Start()
{
	printf("\n\n---------------------------------------Tetrahedron Started!----------------------------------------\n\n");


	T1 = SpawnGameObjectOfClass<GO_Tetrahedron>(FTransform(FVector3(0.0f, -20.0f, 0.0f), FQuaternion(), FVector3(1)));

	SpawnGameObjectOfClass<GO_Ball>(FTransform(FVector3(0.0f, 0.0f, -5.0f), FQuaternion(), FVector3(1)));
	//T3 = SpawnGameObjectOfClass<GO_Pawn>(FTransform(FVector3(0, 0, 0.5f), FQuaternion(), FVector3(1)));
	
	Skybox = SpawnGameObjectOfClass<GO_Skybox>();

	//auto Wall = SpawnGameObjectOfClass<GO_Wall>(FTransform(FVector3(0, 3, 0), FQuaternion(), FVector3(1)));

	//auto dice = SpawnGameObjectOfClass<O_GameObject>(FTransform(FVector3(0, -1, 0), FQuaternion(), FVector3(1)));
	//auto mesh = dice->AddComponentOfClass<C_StaticMeshComponent>();
	//mesh->SetMeshName("Box001");
	//mesh->SetMaterialName("M_Tetrahedron");

	//C_SphereCollider* boxPtr = T1->AddComponentOfClass<C_SphereCollider>();
	//boxPtr->SetCollisionType(ECollisionType::COLLISION);

	C_BoundingBox* spherePtr = T1->AddComponentOfClass<C_BoundingBox>();
	spherePtr->SetCollisionType(ECollisionType::COLLISION);
	spherePtr->SetComponentScale(FVector3(300.0f, 2.f, 300.0f));
	spherePtr->SetComponentPosition(FVector3(-150.0f,-1.5f,-150.0f));
	//spherePtr->SetOverlapBeginFunction(GO_Wall::Overlap);

	
	//SpawnGameObjectOfClass<GO_Wall>(FTransform(FVector3(0.0f, 3.0f, -17.0f), FQuaternion(), FVector3()));

	//wall->Mesh->SetComponentPosition(FVector3(0.0f, 0.3f, -17.0f));

	auto sun = SpawnGameObjectOfClass<GO_DirectionalLight>();
	sun->SetColour({ 1.0, 1.0, 1.0 });
	sun->SetIntensity(1.0);
	sun->SetAmbientMultiplier(0);
	sun->SetSpecularMultiplier(0.5);
	sun->SetRotation(FQuaternion({ 1, 0,  0 }, -90.0f));
	sun->SetTurnedOn(true);


	T2 = SpawnGameObjectOfClass<GO_Pawn>(FTransform(FVector3(0.0f, 0.0f, 1.0f), FQuaternion(), FVector3(1.0f)));



	//auto camera = SpawnGameObjectOfClass<GO_Camera>(FTransform(FVector3(0.0f, 0.0f, 15.0f), FQuaternion(FVector3(0, 1, 0), 0.0f), FVector3(1.0f)));


	BackgroundSound* bgm = new BackgroundSound(LevelGraph::GetInstance()->GetActiveCamera());
	bgm->PlaySound("lol music.mp3", 1.0f, true, false, false); // background music follow cam -> 2d (false)
	//bgm->PlaySound("lofi.mp3", 1.0f, true, false, false);

	AudioSource* test1 = new AudioSource(T1);
	//test1->PlaySound("lol music.mp3", 10.0f, true, true, false); // object sound -> 3d (true)
	L_Level::Start();
}

void L_TetrahedronLevel::Update(float deltaTime)
{
	L_Level::Update(deltaTime);
}

void L_TetrahedronLevel::Render()
{
	L_Level::Render();
}

void L_TetrahedronLevel::PrintMinowskiDifference()
{
	FQuaternion temp;
	FVector3 result1;
	FVector3 result2;
	int i = 0;

	printf("\nMinowski Difference\n");
	/*for (auto v1 : T1->GetMesh()->GetMesh()->Vertices)//for each vertex in Tetrahedron1
	{
		temp = FQuaternion(v1.Position.X, v1.Position.Y, v1.Position.Z, 0);
		temp = T1->GetRotation() * temp * T1->GetRotation().GetConjugated();
		result1 = FVector3(temp.X, temp.Y, temp.Z);
		result1 += T1->GetPosition();

		int j = 0;
		for (auto v2 : T2->GetMesh()->GetMesh()->Vertices)//Multiply it by each vertex in Tetrahedron2
		{
			temp = FQuaternion(v2.Position.X, v2.Position.Y, v2.Position.Z, 0);
			temp = T2->GetRotation() * temp * T2->GetRotation().GetConjugated();
			result2 = FVector3(temp.X, temp.Y, temp.Z);
			result2 += T2->GetPosition();

			(result1 - result2).Print();

			j++;
			if (j >= 4) break;
		}

		i++;
		if (i >= 4) break;
	}
	*/
}
