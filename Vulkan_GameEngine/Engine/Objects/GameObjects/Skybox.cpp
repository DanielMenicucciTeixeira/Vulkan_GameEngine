#include "Skybox.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Renderers/RenderObject.h"

GO_Skybox::GO_Skybox(L_Level* level, std::string name) : O_GameObject(level, name)
{
	Cube = AddComponentOfClass<C_StaticMeshComponent>();
	Cube->SetMeshName("Box001");
	Cube->SetMaterialName("M_Skybox");
}
