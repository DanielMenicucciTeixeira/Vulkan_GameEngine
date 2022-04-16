#include "Tetrahedron.h"
#include "Objects/Components/StaticMeshComponent.h"
#include "Objects/Components/MovementComponent.h"
#include "Renderers/RenderObject.h"
#include "Math/FQuaternion.h"
#include "Objects/Components/MeshCollision.h"
#include "Objects/Components/PhysicsComponent.h"
#include "Objects/Components/Colliders/BoundingBox.h"
GO_Tetrahedron::GO_Tetrahedron(std::string name) : O_GameObject(name, true)
{
	AddComponentOfClass<C_PhysicsComponent>();

	Mesh[0] = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh[0]->SetMeshName("TableSurface");
	Mesh[0]->SetMaterialName("M_Green");
	Mesh[0]->SetComponentScale(FVector3(300.0f, 1.0f, 300.0f));
	Mesh[0]->SetComponentPosition(FVector3(-150.0f, 2.0f, -150.0f));

	Mesh[1] = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh[1]->SetMeshName("TableFrame");
	Mesh[1]->SetMaterialName("M_Brown");
	Mesh[1]->SetComponentScale({ 300.0f, 3.0f, 300.0f });
	Mesh[1]->SetComponentPosition({ 0.0f, -0.1, 0.0 });

	Mesh[2] = AddComponentOfClass<C_StaticMeshComponent>();
	Mesh[2]->SetMeshName("Box001");
	Mesh[2]->SetMaterialName("M_Brown");
	Mesh[2]->SetComponentScale(FVector3(50.0f, 3.0f, 10.0f));
	Mesh[2]->SetComponentPosition(FVector3(0.0f,1.0f,-20.0f));

	Mesh[0]->SetComponentPosition(GetPosition());

	coll1 = AddComponentOfClass<C_BoundingBox>();
	coll1->SetCollisionType(ECollisionType::COLLISION);
	coll1->SetComponentScale(FVector3(500.0f, 10.0f, 10.0f));
	coll1->SetComponentPosition(FVector3(-25.0f, -1.0f, -17.0f));
	
	


	Collider = AddComponentOfClass<C_MeshCollision>();
	//Collider->SetCollisionType(OVERLAP);
	//Collider->SetOverlapBeginFunction(GO_Tetrahedron::OnOverlapBegin);
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
	Collider->SetCollisionMesh(Mesh[0]);

	coll1->GetBoundsFromMesh(Mesh[2]->GetMesh(), Mesh[2]->GetComponentScale(), Mesh[2]->GetComponentScale());
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
