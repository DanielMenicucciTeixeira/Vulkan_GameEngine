#include "OctSpatialPartition.h"
#include "Geometry/Ray.h"
#include "Physics/CollisionDetection.h"
#include "Objects/GameObjects/GameObject.h"
#include "Objects/Components/Colliders/BoundingBox.h"
#include "Objects/Components/Colliders/SphereCollider.h"
#include "Objects/Components/Colliders/BoxCollider.h"
#include "Geometry/Box.h"
#include <iostream>

unsigned int OctNode::ChildrenCount = 0;

OctNode::OctNode(FVector3 position, float size, OctNode* parent) : OctBounds(nullptr), Parent(nullptr), Children(), Colliders(std::vector<C_CollisionComponent*>()), Empty(true)
{
	Size = size;
	Parent = parent;

	OctBounds = new S_BoxBounds();
	OctBounds->Min = position;
	OctBounds->Max = position + FVector3(size);

	for (int i = 0; i < CHILDREN_NUMBER; i++) Children[i] = nullptr;
	if (IsLeaf()) Colliders.reserve(10);
}

OctNode::~OctNode()
{
	delete(OctBounds);
	OctBounds = nullptr;

	if (Colliders.size()) for (auto element : Colliders) element = nullptr;
	Colliders.clear();

	if (!IsLeaf()) for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		if (Children[i]) delete Children[i];
		Children[i] = nullptr;
	}
}

void OctNode::Octify(unsigned int depth)
{
	if (depth > 0 && this)
	{
		float half = Size / 2.0f;
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					//TODO: This seem's wrong.
					Children[z + (2 * y) + (4 * x)] = new OctNode(FVector3(OctBounds->Min.X + (x * half), OctBounds->Min.Y + (y * half), OctBounds->Min.Z + (z * half)), half, this);
				}
			}
		}

		ChildrenCount += 8;
		for (int i = 0; i < CHILDREN_NUMBER; i++) Children[i]->Octify(depth - 1);
	}
}

S_BoxBounds OctNode::GetBoundingBox() const
{
	return *OctBounds;
}

OctSpatialPartition::OctSpatialPartition(float worldSize, unsigned int depth) : root(nullptr)
{
	OctNode::ChildrenCount = 0;
	root = new OctNode(FVector3(-(worldSize / 2.0f)), worldSize, nullptr);
	root->Octify(depth);
	std::cout << "There are " << root->GetChildrenCount() << " child nodes" << std::endl;
}

OctSpatialPartition::~OctSpatialPartition()
{
	delete(root);
	root = nullptr;
}

std::set<OctNode*> OctSpatialPartition::GetActiveLeaves() const
{
	std::set<OctNode*> returnSet = std::set<OctNode*>();
	GetActiveLeaves(root, returnSet);
	return returnSet;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(Ray& ray)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}

	intersectionList.clear();

	intersectionList.reserve(20);

	GetIntersectedLeaves(ray, root);

	return intersectionList;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(Sphere& sphere)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	GetIntersectedLeaves(sphere, root);

	return intersectionList;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(S_BoxBounds& bounds)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	GetIntersectedLeaves(bounds, root);

	return intersectionList;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(Box& box)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	GetIntersectedLeaves(box, root);

	return intersectionList;
}

void OctSpatialPartition::Update(const float deltaTime_)
{
	for (auto leaves : GetActiveLeaves()) {
		for (int i = 0; i < leaves->GetColliderCount(); i++) {
			for (int j = i + 1; j < leaves->GetColliderCount(); j++) {
				//if(CollisionDetection::Collision(leaves->Colliders[i], leaves->Colliders[j])
			}
		}
	}
}

void OctSpatialPartition::GetActiveLeaves(OctNode* cell, std::set<OctNode*>& outSet) const
{
	if (cell->IsEmpty()) return;
	if (cell->IsLeaf()) outSet.insert(cell);
	else for (int i = 0; i < CHILDREN_NUMBER; i++) GetActiveLeaves(cell->GetChild(static_cast<EOctChildren>(i)), outSet);
}

void OctSpatialPartition::GetIntersectedLeaves(Ray& ray, OctNode* cell)
{
	//Check if cell is empty
	if (cell->IsEmpty()) return;

	if (CollisionDetection::RayAABBIntersection(ray, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) { 
			for (auto coll : cell->GetColliders()) {
				intersectionList.push_back(coll);
			}
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(ray, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}

void OctSpatialPartition::GetIntersectedLeaves(Sphere& sphere, OctNode* cell)
{
	if (CollisionDetection::SphereAABBIntersection(sphere, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) {
			for (auto coll : cell->GetColliders()) {
				intersectionList.push_back(coll);
			}
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(sphere, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}

void OctSpatialPartition::GetIntersectedLeaves(S_BoxBounds& bounds, OctNode* cell)
{
	if (CollisionDetection::AABBIntersection(bounds, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) {
			for (auto coll : cell->GetColliders()) {
				intersectionList.push_back(coll);
			}
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(bounds, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}

void OctSpatialPartition::GetIntersectedLeaves(Box& box, OctNode* cell)
{
	if (CollisionDetection::AABBOBBIntersection(cell->GetBoundingBox(), box))
	{
		if (cell->IsLeaf()) {
			for (auto coll : cell->GetColliders()) {
				intersectionList.push_back(coll);
			}
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(box, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}


//TODO: could you create sub versions of them that would take in the specific types?  this would remove the need for the switch and cast.
void OctSpatialPartition::AddColliderToCell(C_CollisionComponent* collider, OctNode* cell)
{
	bool isCollideing = false;
	switch (collider->GetColliderType())
	{
	case ColliderType::BoundingBox:
		isCollideing = CollisionDetection::AABBIntersection(cell->GetBoundingBox(), static_cast<C_BoundingBox*>(collider)->GetBoxBounds());
		break;

	case ColliderType::Sphere:
		isCollideing = CollisionDetection::SphereAABBIntersection(static_cast<C_SphereCollider*>(collider)->GetCollisionSphere(), cell->GetBoundingBox());
		break;

	case ColliderType::Box:
		isCollideing = CollisionDetection::AABBOBBIntersection(cell->GetBoundingBox(), static_cast<C_BoxCollider*>(collider)->GetCollisionBox());
		break;
	}

	if (isCollideing)
	{
		if (cell->IsLeaf())
		{
			cell->AddCollider(collider);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) AddColliderToCell(collider, cell->GetChild(static_cast<EOctChildren>(i)));
	}
	
}
