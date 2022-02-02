#include "BoundingBox.h"
#include "Renderers/RenderObject.h"
#include "Math/FTransform.h"
#include "Objects/GameObjects/GameObject.h"

C_BoundingBox::C_BoundingBox(O_GameObject* owner, ECollisionType type) : C_CollisionComponent(owner, type)
{
}

C_BoundingBox::~C_BoundingBox()
{
}

void C_BoundingBox::GetDimensionsFromMesh(S_Mesh* mesh)
{
	BoxBounds.Max = mesh->Vertices[0].Position;
	BoxBounds.Min = mesh->Vertices[0].Position;

	for (const auto& vertex : mesh->Vertices)
	{
		if (BoxBounds.Min.X > vertex.Position.X) BoxBounds.Min.X = vertex.Position.X;
		else if (BoxBounds.Max.X < vertex.Position.X) BoxBounds.Max.X = vertex.Position.X;

		if (BoxBounds.Min.Y > vertex.Position.Y) BoxBounds.Min.Y = vertex.Position.Y;
		else if (BoxBounds.Max.Y < vertex.Position.Y) BoxBounds.Max.Y = vertex.Position.Y;
		
		if (BoxBounds.Min.Z > vertex.Position.Z) BoxBounds.Min.Z = vertex.Position.Z;
		else if (BoxBounds.Max.Y < vertex.Position.Y) BoxBounds.Max.Y = vertex.Position.Y;
	}
}

void C_BoundingBox::Start()
{
	C_CollisionComponent::Start();
	BoxBounds.Model = GetComponentModelMatrix();
}

void C_BoundingBox::Update(float deltaTime)
{
	C_CollisionComponent::Update(deltaTime);
	BoxBounds.Model = GetComponentModelMatrix();
}

FVector3 C_BoundingBox::GetMin()
{
	//FVector4 temp = Transform->GetModelMatrix() * FVector4(BoxBounds.Min.X, BoxBounds.Min.Y, BoxBounds.Min.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(BoxBounds.Min.X, BoxBounds.Min.Y, BoxBounds.Min.Z, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

FVector3 C_BoundingBox::GetMax()
{

	//FVector4 temp = Transform->GetModelMatrix() * FVector4(BoxBounds.Max.X, BoxBounds.Max.Y, BoxBounds.Max.Z, 1.0f);
	FVector4 temp = GetComponentModelMatrix() * FVector4(BoxBounds.Max.X, BoxBounds.Max.Y, BoxBounds.Max.Z, 1.0f);
	temp = temp / temp.W;
	return FVector3(temp.X, temp.Y, temp.Z);
}

bool C_BoundingBox::SpatialPartitionCheck(S_BoxBounds box)
{
	S_CollisionData data;
	return BoundingBoxBoundingBoxCollision(BoxBounds, box, data);
}
