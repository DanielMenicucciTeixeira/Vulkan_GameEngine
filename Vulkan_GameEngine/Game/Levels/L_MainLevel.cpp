#include "L_MainLevel.h"
#include "Game.h"
#include "SDL/SDLManager.h"
#include "GO_Triangle.h"
#include "Renderers/RenderObject.h"
#include "Renderers/Renderer.h"

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

	Materials["Ball14"] = new S_Material();
	Materials["Ball14"]->Name = "Ball14";
	Materials["Ball14"]->TextureDifuse->Path = "Game/Textures/Ball14.jpg";

	for (auto& mesh : Meshes) LoadMesh(mesh.second);

	if (!O_Level::Initialize(game)) return false;

	return true;
}

void L_MainLevel::Render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	O_Level::Render();
	SDL_GL_SwapWindow(CurrentGame->GetInterfaceManager()->GetSDLWindowByName(CurrentGame->GetInterfaceManager()->GetDefaultWindowName()));
}

void L_MainLevel::Update(float deltaTime)
{
	O_Level::Update(deltaTime);
	printf("MainLevel\tDelta Time: %f\tTotal Time: %f\n", deltaTime, CurrentGame->GetTimeSeconds());
}

void L_MainLevel::Start()
{
	SpawnGameObjectOfClass<GO_Triangle>();
	O_Level::Start();
	printf("MainLevel Started!\n");
}
