#include "L_TetrahedronLevel.h"


#include "GO_Triangle.h"
#include "Apple.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Renderer.h"
#include "Math/FQuaternion.h"
#include "GO_Camera.h"
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
#include "Renderers/Materials/SkyboxMaterial.h"
#include "Objects/GameObjects/Skybox.h"

L_TetrahedronLevel::L_TetrahedronLevel()
{
	Name = "Tetrahedron Level";
}

bool L_TetrahedronLevel::Initialize()
{
	printf("\n\n---------------------------------------Tetrahedron Initialized!----------------------------------------\n\n");

	S_Texture* tetrahedron_Texture = new S_Texture();
	tetrahedron_Texture->Name = "DumbTexture";
	tetrahedron_Texture->Path = "Assets/Textures/DumbTexture.png";
	LevelGraph::GetInstance()->AddTexture(tetrahedron_Texture);
	LoadTexture(tetrahedron_Texture, tetrahedron_Texture->Name);

	S_CubeSampler* skybox_Sampler = new S_CubeSampler();
	skybox_Sampler->Name = "SkyboxSampler";
	/*skybox_Sampler->Textures[0]->Name = "Front";
	skybox_Sampler->Textures[0]->Path = "Assets/Textures/skybox/front.png";
	skybox_Sampler->Textures[1]->Name = "Back";
	skybox_Sampler->Textures[1]->Path = "Assets/Textures/skybox/back.png";
	skybox_Sampler->Textures[2]->Name = "Top";
	skybox_Sampler->Textures[2]->Path = "Assets/Textures/skybox/top.png";
	skybox_Sampler->Textures[3]->Name = "Bottom";
	skybox_Sampler->Textures[3]->Path = "Assets/Textures/skybox/bottom.png";
	skybox_Sampler->Textures[4]->Name = "Right";
	skybox_Sampler->Textures[4]->Path = "Assets/Textures/skybox/right.png";
	skybox_Sampler->Textures[5]->Name = "Left";
	skybox_Sampler->Textures[5]->Path = "Assets/Textures/skybox/left.png";*/
	skybox_Sampler->Textures[0]->Name = "Front";
	skybox_Sampler->Textures[0]->Path = "Assets/Textures/CheckerboardTexture.png";
	skybox_Sampler->Textures[1]->Name = "Back";
	skybox_Sampler->Textures[1]->Path = "Assets/Textures/Apple_Body.png";
	skybox_Sampler->Textures[2]->Name = "Top";
	skybox_Sampler->Textures[2]->Path = "Assets/Textures/CheckerboardTexture.png";
	skybox_Sampler->Textures[3]->Name = "Bottom";
	skybox_Sampler->Textures[3]->Path = "Assets/Textures/Apple_Body.png";
	skybox_Sampler->Textures[4]->Name = "Right";
	skybox_Sampler->Textures[4]->Path = "Assets/Textures/CheckerboardTexture.png";
	skybox_Sampler->Textures[5]->Name = "Left";
	skybox_Sampler->Textures[5]->Path = "Assets/Textures/Apple_Body.png";
	LevelGraph::GetInstance()->AddCubeSampler(skybox_Sampler);
	LoadCubeSampler(skybox_Sampler, skybox_Sampler->Name);

	ModelPaths.insert("Assets/Models/Tetrahedron.obj");
	ModelPaths.insert("Assets/Models/Skybox.obj");
	LoadModels();
	MaterialPaths.insert("Assets/Materials/Tetrahedron.mtl");
	LoadMaterialLibrary();

	M_StandardMaterial* standardMaterial = dynamic_cast<M_StandardMaterial*>(LevelGraph::GetInstance()->GetMaterials()["M_Tetrahedron"]);
	standardMaterial->DiffuseTexture = tetrahedron_Texture;
	standardMaterial->SpecularTexture = tetrahedron_Texture;

	M_SkyboxMaterial* skyboxMaterial = new M_SkyboxMaterial();
	skyboxMaterial->CubeMap = skybox_Sampler;
	LevelGraph::GetInstance()->AddMaterial(skyboxMaterial);

	return L_Level::Initialize();
}

void L_TetrahedronLevel::Start()
{
	printf("\n\n---------------------------------------Tetrahedron Started!----------------------------------------\n\n");

	T1 = SpawnGameObjectOfClass<GO_Tetrahedron>(FTransform(FVector3(-3, 0, 0), FQuaternion(), FVector3(1)));
	T2 = SpawnGameObjectOfClass<GO_Tetrahedron>(FTransform(FVector3(3, 0, 0), FQuaternion(), FVector3(1)));
	Skybox = SpawnGameObjectOfClass<GO_Skybox>();

	for (auto phys : T1->GetComponentsOfClass<C_PhysicsComponent>())
	{
		phys->SetVelocity({ 1.0f, 0.0f, 0.0f });
		phys->SetAngularVelocity({ 0.0f, 45.0f, 0.0f });
	}

	for (auto phys : T2->GetComponentsOfClass<C_PhysicsComponent>())
	{
		phys->SetVelocity({ -1.0f, 0.0f, 0.0f });
		phys->SetAngularVelocity({ 0.0f, -45.0f, 0.0f });
	}

	auto sun = SpawnGameObjectOfClass<GO_DirectionalLight>();
	sun->SetColour({ 1.0, 1.0, 1.0 });
	sun->SetIntensity(1.0);
	sun->SetAmbientMultiplier(0);
	sun->SetSpecularMultiplier(0.5);
	sun->SetRotation(FQuaternion({ 1, 0,  0 }, -90.0f));
	sun->SetTurnedOn(true);

	auto camera = SpawnGameObjectOfClass<GO_Camera>(FTransform(FVector3(0.0f, 0.0f, 15.0f), FQuaternion(0.0f, FVector3(0, 0, 1)), FVector3(1.0f)));
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
	for (auto v1 : T1->GetMesh()->GetMesh()->Vertices)//for each vertex in Tetrahedron1
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
}
