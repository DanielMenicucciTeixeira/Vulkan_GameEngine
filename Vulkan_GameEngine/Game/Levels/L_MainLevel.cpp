#include "L_MainLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"
#include "GO_Triangle.h"
#include "Apple.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Renderer.h"
#include "Math/FQuaternion.h"
#include "GO_Camera.h"
#include "FX/LightSource.h"
#include "FX/DirectionalLight.h"
#include "AssetLoader.h"
#include "LevelGraph.h"
#include "Objects/Components/CameraComponent.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
L_MainLevel::L_MainLevel() : L_Level()
{
	Name = "MainLevel";
}

bool L_MainLevel::Initialize(Game* game)
{
	printf("\n\n---------------------------------------MainLevel Initialized!----------------------------------------\n\n");

	/*auto SphereMesh = new S_Mesh();
	SphereMesh->Name = "SphereMesh";
	SphereMesh->Path = "Game/Models/sphere.obj";*/

	/*auto Ball14_Texture = new S_Texture();
	Ball14_Texture->Name = "Ball14";
	Ball14_Texture->Path = "Game/Textures/Ball14.jpg";
	auto Ball14_Material = new S_Material();
	Ball14_Material->Name = "Ball14";
	Ball14_Material->TextureNameDifuse = "Ball14";
	Ball14_Material->ShaderName = "TextureShader";*/

	auto Apple_Body_Texture = new S_Texture();
	Apple_Body_Texture->Name = "Apple_Body";
	Apple_Body_Texture->Path = "Assets/Textures/Apple_Body.png";
	//Apple_Body_Texture->Path = "Game/Textures/Ball15.jpg";
	LevelGraph::GetInstance()->AddTexture(Apple_Body_Texture);
	LoadTexture(Apple_Body_Texture, Apple_Body_Texture->Name);

	auto Apple_Stem_Texture = new S_Texture();
	Apple_Stem_Texture->Name = "Apple_Stem";
	Apple_Stem_Texture->Path = "Assets/Textures/Apple_Stem.png";
	LevelGraph::GetInstance()->AddTexture(Apple_Stem_Texture);
	LoadTexture(Apple_Stem_Texture, Apple_Stem_Texture->Name);

	auto Dice_Texture = new S_Texture();
	Dice_Texture->Name = "diceTexture";
	Dice_Texture->Path = "Assets/Textures/diceTexture.png";
	LevelGraph::GetInstance()->AddTexture(Dice_Texture);
	LoadTexture(Dice_Texture, Dice_Texture->Name);

	ModelPaths.insert("Assets/Models/Apple.obj");
	ModelPaths.insert("Assets/Models/Dice.obj");
	MaterialPaths.insert("Assets/Materials/Apple.mtl");
	MaterialPaths.insert("Assets/Materials/Dice.mtl");
	LoadMaterialLibrary();
	
	LevelGraph::GetInstance()->GetMaterials()["M_Apple_Body"]->TextureNameDifuse = "Apple_Body";
	LevelGraph::GetInstance()->GetMaterials()["M_Apple_Body"]->TextureDifuse = Apple_Body_Texture;
	LevelGraph::GetInstance()->GetMaterials()["M_Apple_Body"]->ShaderName = "TextureShader";
	
	LevelGraph::GetInstance()->GetMaterials()["M_Apple_Stem"]->TextureNameDifuse = "Apple_Stem";
	LevelGraph::GetInstance()->GetMaterials()["M_Apple_Stem"]->TextureDifuse = Apple_Stem_Texture;
	LevelGraph::GetInstance()->GetMaterials()["M_Apple_Stem"]->ShaderName = "TextureShader";

	LevelGraph::GetInstance()->GetMaterials()["M_diceTexture"]->TextureNameDifuse = "diceTexture";
	LevelGraph::GetInstance()->GetMaterials()["M_diceTexture"]->TextureDifuse = Dice_Texture;
	LevelGraph::GetInstance()->GetMaterials()["M_diceTexture"]->ShaderName = "TextureShader";
	return L_Level::Initialize(game);
}

void L_MainLevel::Render()
{
	L_Level::Render();
}

void L_MainLevel::Update(float deltaTime)
{
	auto objects = LevelGraph::GetInstance()->GetObjects();
	L_Level::Update(deltaTime);
}

void L_MainLevel::Start()
{
	printf("\n\n---------------------------------------MainLevel Started!----------------------------------------\n\n");
	/*SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(-3.0f, -3.0f, 0.0f), FQuaternion(), FVector3(1.0f)));
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(0.0f, 3.0f, -3.0f), FQuaternion(), FVector3(1.0f)));
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(3.0f, -3.0f, -6.0f), FQuaternion(), FVector3(1.0f)));*/
	//SpawnGameObjectOfClass<GO_Apple>(FTransform(FVector3(-5, 0, 0), FQuaternion(), FVector3(1)));
	SpawnGameObjectOfClass<GO_Apple>(FTransform(FVector3(5, 0, 0), FQuaternion(), FVector3(1)));
	auto dice = SpawnGameObjectOfClass<O_GameObject>(FTransform(FVector3(-5, 0, 0), FQuaternion(), FVector3(1)));
	auto mesh = dice->AddComponentOfClass<C_StaticMeshComponent>();
	mesh->SetMeshName("Box001");
	mesh->SetMaterialName("M_diceTexture");
	auto movement = dice->AddComponentOfClass<C_MovementComponent>();
	movement->SetAngularVelocity({ 0.0f, 1.0f, 0.0f });

	auto sun = SpawnGameObjectOfClass<GO_DirectionalLight>();
	auto red = SpawnGameObjectOfClass<GO_LightSource>(FTransform(FVector3(3.0f, 3.0f, 1.0f), FQuaternion(), FVector3(1.0f)));
	auto green = SpawnGameObjectOfClass<GO_LightSource>(FTransform(FVector3(7.0f, 7.0f, 1.0f), FQuaternion(), FVector3(1.0f)));
	auto blue = SpawnGameObjectOfClass<GO_LightSource>(FTransform(FVector3(-3.0f, 0.0f, 1.0f), FQuaternion(), FVector3(1.0f)));

	red->SetColour({ 1, 1, 1 });
	red->SetIntensity(3);
	green->SetColour({ 1, 1, 1 });
	green->SetIntensity(3);
	blue->SetColour({ 1, 1, 1 });
	blue->SetIntensity(3);
	

	//blue->SetLightType(E_LightType::FIXED_INTENSITY_POINT_LIGHT);
	//blue->SetIntensity(0.5);


	sun->SetColour({ 1, 1, 1 });
	sun->SetIntensity(1);
	sun->SetAmbientMultiplier(0);
	sun->SetSpecularMultiplier(0.5);
	sun->SetRotation(FQuaternion({ 1, 0,  0 }, -90));
	sun->SetTurnedOn(true);

	auto camera = SpawnGameObjectOfClass<GO_Camera>(FTransform(FVector3(0.0f, 0.0f, 20.0f), FQuaternion(), FVector3()));
	L_Level::Start();
}
