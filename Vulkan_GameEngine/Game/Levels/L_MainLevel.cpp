#include "L_MainLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"
#include "GO_Triangle.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Renderer.h"
#include "Math/FQuaternion.h"
#include "GO_Camera.h"
#include "FX/LightSource.h"
#include "FX/DirectionalLight.h"
#include "FX/LightInfo.h"

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
L_MainLevel::L_MainLevel() : O_Level()
{
	Name = "MainLevel";
}

bool L_MainLevel::Initialize(Game* game)
{
	Meshes["SphereMesh"] = new S_Mesh();
	Meshes["SphereMesh"]->Name = "SphereMesh";
	Meshes["SphereMesh"]->Path = "Game/Models/sphere.obj";

	Textures["Ball14"] = new S_Texture();
	Textures["Ball14"]->Name = "Ball14";
	Textures["Ball14"]->Path = "Game/Textures/Ball14.jpg";

	Materials["Ball14"] = new S_Material();
	Materials["Ball14"]->Name = "Ball14";
	Materials["Ball14"]->TextureNameDifuse = "Ball14";
	Materials["Ball14"]->ShaderName = "TextureShader";

	if (!O_Level::Initialize(game)) return false;

	return true;
}

void L_MainLevel::Render()
{
	O_Level::Render();
}

void L_MainLevel::Update(float deltaTime)
{
	O_Level::Update(deltaTime);
}

void L_MainLevel::Start()
{
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(-3.0f, -3.0f, 0.0f), FQuaternion(), FVector3(1.0f)));
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(0.0f, 3.0f, -3.0f), FQuaternion(), FVector3(1.0f)));
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(3.0f, -3.0f, -6.0f), FQuaternion(), FVector3(1.0f)));
	
	auto red = SpawnGameObjectOfClass<GO_LightSource>(FTransform(FVector3(4.0f, 0.0f, -1.5f), FQuaternion(), FVector3(1.0f)));
	auto blue = SpawnGameObjectOfClass<GO_LightSource>(FTransform(FVector3(0.0f, -2.0f, 1.5f), FQuaternion(), FVector3(1.0f)));
	auto green = SpawnGameObjectOfClass<GO_LightSource>(FTransform(FVector3(0.0f, 0.0f, 0.0f), FQuaternion(), FVector3(1.0f)));
	auto sun = SpawnGameObjectOfClass<GO_DirectionalLight>();

	sun->SetColour({ 1, 1, 1 });
	sun->SetIntensity(1);
	sun->SetAmbientMultiplier(0);
	sun->SetRotation(FQuaternion({ 0, 0, 1 }, 90));
	sun->SetTurnedOn(false);

	red->SetColour({ 1, 0, 0 });
	red->SetLightType(E_LightType::POINT_LIGHT);
	red->SetIntensity(2);
	green->SetColour({ 0, 1, 0 });
	green->SetLightType(E_LightType::POINT_LIGHT);
	green->SetIntensity(2);
	blue->SetColour({ 0, 0, 1 });
	blue->SetLightType(E_LightType::POINT_LIGHT);
	blue->SetIntensity(2);
	//blue->SetLightType(E_LightType::FIXED_INTENSITY_POINT_LIGHT);
	//blue->SetIntensity(0.5);

	SpawnGameObjectOfClass<GO_Camera>(FTransform(FVector3(0.0f, 0.0f, 20.0f), FQuaternion(), FVector3()));
	O_Level::Start();
	printf("MainLevel Started!\n");
}
