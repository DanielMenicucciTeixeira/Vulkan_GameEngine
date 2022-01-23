#include "Tetrahedron.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"
#include "Objects/Components/MeshCollision.h"

GO_Tetrahedron::GO_Tetrahedron(L_Level* level, std::string name) : O_GameObject(level, name)
{
	Mesh = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh->SetMeshName("Tetrahedron");
	Mesh->SetMaterialName("M_Tetrahedron");

	Movement = AddComponentOfClass<C_MovementComponent>();

	Collider = AddComponentOfClass<C_MeshCollision>();
	Collider->SetCollisionType(OVERLAP);
	Collider->SetOverlapBeginFunction(GO_Tetrahedron::OnOverlapBegin);
}

GO_Tetrahedron::~GO_Tetrahedron()
{
}

void GO_Tetrahedron::Update(float deltaTime)
{
	O_GameObject::Update(deltaTime);
	
	/*printf("\nVertex Positions:\n");
	for (auto vertex : Mesh->GetMesh()->Vertices)
	{
		FQuaternion temp = FQuaternion(vertex.Position.X, vertex.Position.Y, vertex.Position.Z, 0);
		temp = GetRotation() * temp * GetRotation().GetConjugated();
		FVector3 outVector = FVector3(temp.X, temp.Y, temp.Z);
		outVector += GetPosition();
		outVector.Print();
	}
	printf("\n");*/
}

void GO_Tetrahedron::Start()
{
	Collider->SetCollisionMesh(Mesh);
}

void GO_Tetrahedron::OnOverlapBegin(O_GameObject* self, const S_CollisionData& data)
{
	printf("Overlapping!\n");
	/*static bool overlapping = false;
	if (!overlapping)
	{
		printf("Overlapping!\n");
		overlapping = true;
	}*/
}
