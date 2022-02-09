#include "OctSpatialPartition.h"
#include "Geometry/Ray.h"
#include "Physics/CollisionDetection.h"
#include "Objects/GameObjects/GameObject.h"
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

const S_BoxBounds OctNode::GetBoundingBox() const
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
	for (auto cell : intersectionList) {
		cell = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	GetIntersectedLeaves(ray, root);

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

	
	//Never used variable
	S_CollisionData data;

	//TODO: Linker error? why does this happen when calling Collision
	//Collision detection
	if (CollisionDetection::RayObbIntersection(ray, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) { 
			for (auto coll : cell->GetColliders()) {
				intersectionList.push_back(coll);
			}
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(ray, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}

void OctSpatialPartition::AddColliderToCell(C_CollisionComponent* collider, OctNode* cell)
{
	if (collider && cell && collider->SpatialPartitionCheck(cell->GetBoundingBox()));
	{
		cell->SetEmpty(false);
		if (cell->IsLeaf())
		{
			cell->AddCollider(collider);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) AddColliderToCell(collider, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}
