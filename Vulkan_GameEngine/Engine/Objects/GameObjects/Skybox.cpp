#include "Skybox.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Renderers/RenderObject.h"

GO_Skybox::GO_Skybox(std::string name) : O_GameObject(name)
{
	Cube = AddComponentOfClass<C_StaticMeshComponent>();
	Cube->SetMeshName("Box001");
	Cube->SetMaterialName("M_Skybox");
}
