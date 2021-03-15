#include "L_MainLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"
#include "GO_Triangle.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Renderer.h"
#include "Math/FQuaternion.h"
#include "GO_Camera.h"

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
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(0.5f, -0.25f, 0.0f), FQuaternion(), FVector3()));
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(0.0f, 0.5f, -2.5f), FQuaternion(), FVector3()));
	SpawnGameObjectOfClass<GO_Triangle>(FTransform(FVector3(-0.5f, -0.25f, -5.0f), FQuaternion(), FVector3()));
	SpawnGameObjectOfClass<GO_Camera>(FTransform(FVector3(0.0f, 0.0f, 5.0f), FQuaternion(), FVector3()));
	O_Level::Start();
	printf("MainLevel Started!\n");
}
