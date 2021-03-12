#include "GO_Triangle.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Renderers/RenderObject.h"

GO_Triangle::GO_Triangle(O_Level* level) : O_GameObject(level)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("SphereMesh");
	Mesh->SetTextureName("Ball14");
}
