#include "OctSpactilPartition.h"
#include "Objects/GameObjects/GameObject.h"
#include <iostream>

unsigned int OctSpactilPartition::OctNode::ChildrenCount = 0;

OctSpactilPartition::OctNode::OctNode(FVector3 position, float size, OctNode* parent) : OctBounds(nullptr), Parent(nullptr), Children(), Colliders(std::vector<C_CollisionComponent*>()), Empty(true)
{
	Size = size;
	Parent = parent;

	OctBounds = new S_BoxBounds();
	OctBounds->Min = position;
	OctBounds->Max = position + FVector3(size);

	for (int i = 0; i < CHILDREN_NUMBER; i++) Children[i] = nullptr;
	if (IsLeaf()) Colliders.reserve(10);
}

OctSpactilPartition::OctNode::~OctNode()
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

void OctSpactilPartition::OctNode::Octify(unsigned int depth)
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
					Children[z + (2 * y) + (4 * x)] = new OctNode(FVector3(OctBounds->Min.X + (x * half), OctBounds->Min.Y + (y * half), OctBounds->Min.Z + (z * half)), half, this);
				}
			}
		}

		ChildrenCount += 8;
		for (int i = 0; i < CHILDREN_NUMBER; i++) Children[i]->Octify(depth - 1);
	}
}

const S_BoxBounds OctSpactilPartition::OctNode::GetBoundingBox() const
{
	return *OctBounds;
}

OctSpactilPartition::OctSpactilPartition(float worldSize, unsigned int depth) : Root(nullptr)
{
	OctNode::ChildrenCount = 0;
	Root = new OctNode(FVector3(-(worldSize / 2.0f)), worldSize, nullptr);
	Root->Octify(depth);
	std::cout << "There are " << Root->GetChildrenCount() << " child nodes" << std::endl;
}

OctSpactilPartition::~OctSpactilPartition()
{
	delete(Root);
	Root = nullptr;
}

std::set<OctSpactilPartition::OctNode*> OctSpactilPartition::GetActiveLeaves() const
{
	std::set<OctNode*> returnSet = std::set<OctNode*>();
	GetActiveLeaves(Root, returnSet);
	return returnSet;
}

std::set<OctSpactilPartition::OctNode*> OctSpactilPartition::GetIntersectedLeaves(Ray& ray) const
{
	std::set<OctNode*> returnSet = std::set<OctNode*>();

	GetIntersectedLeaves(ray, Root, returnSet);

	return returnSet;
}

void OctSpactilPartition::GetActiveLeaves(OctNode* cell, std::set<OctNode*>& outSet) const
{
	if (cell->IsEmpty()) return;
	if (cell->IsLeaf()) outSet.insert(cell);
	else for (int i = 0; i < CHILDREN_NUMBER; i++) GetActiveLeaves(cell->GetChild(static_cast<EOctChildren>(i)), outSet);
}

void OctSpactilPartition::GetIntersectedLeaves(Ray& ray, OctNode* cell, std::set<OctNode*>& outSet) const
{
	if (cell->IsEmpty()) return;
	S_CollisionData data;
	if (C_CollisionComponent::RayBoundingBoxCollision(ray, cell->GetBoundingBox(), data))
	{
		if (cell->IsLeaf()) outSet.insert(cell);
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(ray, cell->GetChild(static_cast<EOctChildren>(i)), outSet);
	}
}

void OctSpactilPartition::AddColliderToCell(C_CollisionComponent* collider, OctNode* cell)
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
