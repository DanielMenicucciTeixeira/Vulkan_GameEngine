#include "MeshCollision.h"
#include "Renderers/RenderObject.h"
#include "Math/FMatrix4.h"
#include "Math/FTransform.h"
#include "StaticMeshComponent.h"

#include <algorithm>

//bool C_MeshCollision::CheckSimplexForOrigin(Simplex3D& simplex) const
//{
//	FMatrix4 matrices[5];
//
//	matrices[0] = FMatrix4
//	(
//		FVector4(simplex.GetVertexAtIndex(0).X, simplex.GetVertexAtIndex(0).Y, simplex.GetVertexAtIndex(0).Z, 1.0f),
//		FVector4(simplex.GetVertexAtIndex(1).X, simplex.GetVertexAtIndex(1).Y, simplex.GetVertexAtIndex(1).Z, 1.0f),
//		FVector4(simplex.GetVertexAtIndex(2).X, simplex.GetVertexAtIndex(2).Y, simplex.GetVertexAtIndex(2).Z, 1.0f),
//		FVector4(simplex.GetVertexAtIndex(3).X, simplex.GetVertexAtIndex(3).Y, simplex.GetVertexAtIndex(3).Z, 1.0f)
//	);
//
//	int determinatCheckIndex;
//	float matrixDet = matrices[0].Det();
//	int detSign = abs(matrixDet) / matrixDet;
//	for (determinatCheckIndex = 1; determinatCheckIndex < 4; determinatCheckIndex++)
//	{
//		matrices[determinatCheckIndex] = matrices[0];
//		matrices[determinatCheckIndex][determinatCheckIndex -1] = FVector4(0.0f);
//		matrixDet = matrices[determinatCheckIndex].Det();
//
//		if ((abs(matrixDet) / matrixDet) != detSign)
//		{
//			simplex.SetVertexAtIndex(determinatCheckIndex, simplex.GetVertexAtIndex(4));
//			return false;
//		}
//	}
//
//	return true;
//}

C_MeshCollision::C_MeshCollision(O_GameObject* owner) : C_CollisionComponent(owner)
{
}

FVector3 C_MeshCollision::GetFurthestPoint(const FVector3& direction) const
{
	S_Mesh tempMesh = *CollisionMesh->GetMesh();
	//The point in meshA furthest in the calculated direction
	for (int i= 0; i < tempMesh.Vertices.size(); i++)
	{
		tempMesh.Vertices[i].Position = GetCollisionMesh()->GetComponentAbsoluteTransform().GetModelMatrix() * FVector4(tempMesh.Vertices[i].Position);
	}

	FVector3 point = tempMesh.Vertices[0].Position;
	float currentDotProduct = tempMesh.Vertices[0].Position * direction;
	float tempDotProduct;
	for (const auto& vertex : tempMesh.Vertices)
	{
		tempDotProduct = vertex.Position * direction;
		if (currentDotProduct < tempDotProduct)
		{
			point = vertex.Position;
			currentDotProduct = tempDotProduct;
		}
	}

	return point;
}

FVector3 C_MeshCollision::GetCollisionMeshCenter() const
{
	return CollisionMesh->GetComponentTransform().GetModelMatrix() * FVector4(CollisionMeshCenter);
}

//FVector3 C_MeshCollision::GetFurthestMinkDiffPoint(const C_MeshCollision* meshA, const C_MeshCollision* meshB, FVector3 direction)
//{
//	//The point in meshA furthest in the calculated direction
//	FVector3 pointA = meshA->GetCollisionMesh()->Vertices[0].Position;
//	float currentDotProduct = meshA->GetCollisionMesh()->Vertices[0].Position * direction;
//	for (const auto& vertex : meshA->GetCollisionMesh()->Vertices)
//	{
//		float tempDotProduct = vertex.Position * direction;
//		if (currentDotProduct < tempDotProduct)
//		{
//			pointA = vertex.Position;
//			currentDotProduct = tempDotProduct;
//		}
//	}
//
//	//The point in meshB furthest in the opposit of the calculated direction
//	FVector3 pointB = meshB->GetCollisionMesh()->Vertices[0].Position;
//	currentDotProduct = meshB->GetCollisionMesh()->Vertices[0].Position * direction;
//	for (const auto& vertex : meshB->GetCollisionMesh()->Vertices)
//	{
//		float tempDotProduct = vertex.Position * direction;
//		if (currentDotProduct > tempDotProduct)
//		{
//			pointB = vertex.Position;
//			currentDotProduct = tempDotProduct;
//		}
//	}
//
//	return pointA - pointB;
//}

void C_MeshCollision::SetCollisionMesh(C_StaticMeshComponent* mesh)
{
	if (!mesh) return;
	CollisionMesh = mesh;

	CollisionMeshCenter = FVector3(0.0f);
	for (const auto& vertex : CollisionMesh->GetMesh()->Vertices)
	{
		CollisionMeshCenter += vertex.Position;
	}

	CollisionMeshCenter = CollisionMeshCenter / CollisionMesh->GetMesh()->Vertices.size();
}

//bool C_MeshCollision::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
//{
//	const auto& collider = dynamic_cast<C_MeshCollision*>(otherCollider);
//	if (!collider) return false;
//
//	Simplex3D simplex;
//	
//	//First Simplex Vertex
//	FVector3 direction = collider->GetCollisionMeshCenter() - CollisionMeshCenter;
//	simplex.PushFront(GetFurthestMinkDiffPoint(this, collider, direction));
//
//	//Second Simplex Vertex
//	direction = simplex.GetVertexAtIndex(0) * -1.0f;
//	simplex.PushFront(GetFurthestMinkDiffPoint(this, collider, direction));
//	if (simplex.GetVertexAtIndex(0) * direction < 0) return false;
//
//	//Third Simplex Vertex
//	direction = (simplex.GetVertexAtIndex(1) - simplex.GetVertexAtIndex(0)).CrossProduct(simplex.GetVertexAtIndex(0) * -1.0f).CrossProduct(simplex.GetVertexAtIndex(1) - simplex.GetVertexAtIndex(0));
//	simplex.PushFront(GetFurthestMinkDiffPoint(this, collider, direction));
//	if (simplex.GetVertexAtIndex(0) * direction < 0) return false;
//	
//	//Fourth Simplex Vertex
//	FVector3 pointA = simplex.GetVertexAtIndex(0);
//	FVector3 pointB = simplex.GetVertexAtIndex(1);
//	FVector3 pointC = simplex.GetVertexAtIndex(2);
//	FVector3 lineAB = pointB - pointA;
//	FVector3 lineAC = pointC - pointA;
//	FVector3 lineABC = lineAB.CrossProduct(lineAC);
//	FVector3 lineAO = pointA * -1.0f;
//
//	if (lineABC.CrossProduct(lineAC) * lineAO >= 0)
//	{
//		if (lineAC * lineAO >= 0) direction = lineAC.CrossProduct(lineAO).CrossProduct(lineAC);
//		else if (lineAB * lineAO >= 0) direction = lineAB.CrossProduct(lineAO).CrossProduct(lineAB);
//		else direction = lineAO;
//	}
//	else if (lineAB.CrossProduct(lineABC) * lineAO >= 0)
//	{
//		if (lineAB * lineAO >= 0) direction = lineAB.CrossProduct(lineAO).CrossProduct(lineAB);
//		else direction = lineAO;
//	}
//	else if (lineABC * lineAO >= 0) direction = lineABC;
//	else direction = lineABC * -1.0f;
//	simplex.PushFront(GetFurthestMinkDiffPoint(this, collider, direction));
//	if (simplex.GetVertexAtIndex(0) * direction < 0) return false;
//
//	return CheckSimplexForOrigin(simplex);
//
//	/*FVector3 pointA, pointB, pointC;
//	do
//	{
//		pointA = simplex.GetVertexAtIndex(0);
//		pointB = simplex.GetVertexAtIndex(1);
//		pointC = simplex.GetVertexAtIndex(2);
//
//		direction = (pointB - pointA).CrossProduct(pointC - pointA);
//		FVector3 centerToOrigin = (pointA + pointB + pointC) / -3.0f;
//
//		if ((direction * centerToOrigin) < ((direction * -1.0f) * centerToOrigin)) direction = direction * -1.0f;
//
//		simplex.SetVertexAtIndex(3, GetFurthestMinkDiffPoint(this, collider, direction));
//	}
//	while (!CheckSimplexForOrigin(simplex));*/
//}
