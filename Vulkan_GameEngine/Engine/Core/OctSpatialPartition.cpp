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

OctNode::OctNode(FVector3 position, float size, OctNode* parent) : OctBounds(nullptr), Parent(nullptr), Children(), 
AABBColliders(std::vector<C_BoundingBox*>()), SphereColliders(std::vector<C_SphereCollider*>()), OBBColliders(std::vector<C_BoxCollider*>()), Empty(true)
{
	Size = size;
	Parent = parent;

	OctBounds = new S_BoxBounds();
	OctBounds->Min = position;
	OctBounds->Max = position + FVector3(size);

	for (int i = 0; i < CHILDREN_NUMBER; i++) Children[i] = nullptr;
	if (IsLeaf()) {
		AABBColliders.reserve(10);
		SphereColliders.reserve(10);
		OBBColliders.reserve(10);
	}
}

OctNode::~OctNode()
{
	delete(OctBounds);
	OctBounds = nullptr;

	if (AABBColliders.size()) for (auto element : AABBColliders) element = nullptr;
	AABBColliders.clear();

	if (SphereColliders.size()) for (auto element : SphereColliders) element = nullptr;
	SphereColliders.clear();

	if (OBBColliders.size()) for (auto element : OBBColliders) element = nullptr;
	OBBColliders.clear();

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

void OctNode::RemoveCollider(C_BoundingBox* element)
{
	AABBColliders.erase(std::find(AABBColliders.begin(), AABBColliders.end(), element));

	ColliderCount--;
}

void OctNode::RemoveCollider(C_SphereCollider* element)
{
	SphereColliders.erase(std::find(SphereColliders.begin(), SphereColliders.end(), element));

	ColliderCount--;
}

void OctNode::RemoveCollider(C_BoxCollider* element)
{
	OBBColliders.erase(std::find(OBBColliders.begin(), OBBColliders.end(), element));

	ColliderCount--;
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

void OctSpatialPartition::AddCollider(C_CollisionComponent* collider)
{
	switch (collider->GetColliderType())
	{
	case ColliderType::BoundingBox:
		AddColliderToCell(static_cast<C_BoundingBox*>(collider), root);
		break;

	case ColliderType::Sphere:
		AddColliderToCell(static_cast<C_SphereCollider*>(collider), root);
		break;

	case ColliderType::Box:
		AddColliderToCell(static_cast<C_BoxCollider*>(collider), root);
		break;
	}
}

void OctSpatialPartition::RemoveCollider(C_CollisionComponent* collider)
{

	switch (collider->GetColliderType())
	{
	case ColliderType::BoundingBox:
		for (auto& cell : collider->GetCurrentNodes()) {
			cell->RemoveCollider(static_cast<C_BoundingBox*>(collider));
		}
		break;

	case ColliderType::Sphere:
		for (auto& cell : collider->GetCurrentNodes()) {
			cell->RemoveCollider(static_cast<C_SphereCollider*>(collider));
		}
		break;

	case ColliderType::Box:
		for (auto& cell : collider->GetCurrentNodes()) {
			cell->RemoveCollider(static_cast<C_BoxCollider*>(collider));
		}
		break;
	}
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


	for (auto cell : GetCollidingNodes(ray)) {
		for (auto& coll : cell->GetAABBColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetSphereColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetOBBColliders()) {
			intersectionList.push_back(coll);
		}
	}
	return intersectionList;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(Sphere& sphere)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	for (auto cell : GetCollidingNodes(sphere)) {
		for (auto coll : cell->GetAABBColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetSphereColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetOBBColliders()) {
			intersectionList.push_back(coll);
		}
	}

	return intersectionList;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(S_BoxBounds& bounds)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	for (auto cell : GetCollidingNodes(bounds)) {
		for (auto coll : cell->GetAABBColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetSphereColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetOBBColliders()) {
			intersectionList.push_back(coll);
		}
	}

	return intersectionList;
}

std::vector<C_CollisionComponent*> OctSpatialPartition::GetCollision(Box& box)
{
	for (auto inter : intersectionList) {
		inter = nullptr;
	}
	intersectionList.clear();

	intersectionList.reserve(20);

	for (auto cell : GetCollidingNodes(box)) {
		for (auto coll : cell->GetAABBColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetSphereColliders()) {
			intersectionList.push_back(coll);
		}
		for (auto& coll : cell->GetOBBColliders()) {
			intersectionList.push_back(coll);
		}
	}

	return intersectionList;
}
//TODO:Test this to see if it works.
void OctSpatialPartition::UpdateColliderNode(C_BoundingBox* collider)
{

	//Trick to prevent need for double check.
	int c1 = 0;
	int c2 = 0;
	bool doesExist = false;
	std::set<int> copies;
	
	auto newCells = GetCollidingNodes(collider->GetBoxBounds());
	auto remainingCells = collider->GetCurrentNodes();

	std::vector<OctNode*> currentCells;
	currentCells.reserve(4);

	//Check to see which one's exist in both, then remove them from each.
	for(auto& cell : newCells){
		for (auto& cell2 : remainingCells) {
			if (cell == cell2) {
				doesExist = true;
				break;
			}
			c2++;
		}
		if (doesExist) {
			currentCells.push_back(remainingCells[c2]);
			remainingCells.erase(remainingCells.begin() + c2);
			copies.insert(c1);
			doesExist = false;
		}
		c2 = 0;
		c1++;
	}
	int loop = 0;
	for (auto cop : copies) {
		newCells.erase(newCells.begin() + (cop - loop));
		loop++;
	}

	//if new && remaining are both 0 return early
	if (newCells.empty() && remainingCells.empty()) { return; }

	//Add and remove remaining

	for (auto& cell: newCells) {
		cell->AddCollider(collider);
	}
	for (auto cell : remainingCells) {
		cell->RemoveCollider(collider);
	}

	//merge both and set to collider.
	for (auto& cell : currentCells) {
		newCells.push_back(cell);
	}

	collider->SetCurrentNodes(newCells);
}

void OctSpatialPartition::UpdateColliderNode(C_SphereCollider* collider)
{
	//Trick to prevent need for double check.
	int c1 = 0;
	int c2 = 0;
	bool doesExist = false;
	std::set<int> copies;

	auto newCells = GetCollidingNodes(collider->GetCollisionSphere());
	auto remainingCells = collider->GetCurrentNodes();

	std::vector<OctNode*> currentCells;
	currentCells.reserve(4);

	//Check to see which one's exist in both, then remove them from each.
	for (auto& cell : newCells) {
		for (auto& cell2 : remainingCells) {
			if (cell == cell2) {
				doesExist = true;
				break;
			}
			c2++;
		}
		if (doesExist) {
			currentCells.push_back(remainingCells[c2]);
			remainingCells.erase(remainingCells.begin() + c2);
			copies.insert(c1);
			doesExist = false;
		}
		c2 = 0;
		c1++;
	}
	int loop = 0;
	for (auto cop : copies) {
		newCells.erase(newCells.begin() + (cop - loop));
		loop++;
	}

	//if new && remaining are both 0 return early
	if (newCells.empty() && remainingCells.empty()) { return; }

	//Add and remove remaining

	for (auto& cell : newCells) {
		cell->AddCollider(collider);
	}
	for (auto cell : remainingCells) {
		cell->RemoveCollider(collider);
	}

	//merge both and set to collider.
	for (auto& cell : currentCells) {
		newCells.push_back(cell);
	}

	collider->SetCurrentNodes(newCells);
}

void OctSpatialPartition::UpdateColliderNode(C_BoxCollider* collider)
{
	//Trick to prevent need for double check.
	int c1 = 0;
	int c2 = 0;
	bool doesExist = false;
	std::set<int> copies;

	auto newCells = GetCollidingNodes(collider->GetCollisionBox());
	auto remainingCells = collider->GetCurrentNodes();

	std::vector<OctNode*> currentCells;
	currentCells.reserve(4);

	//Check to see which one's exist in both, then remove them from each.
	for (auto& cell : newCells) {
		for (auto& cell2 : remainingCells) {
			if (cell == cell2) {
				doesExist = true;
				break;
			}
			c2++;
		}
		if (doesExist) {
			currentCells.push_back(remainingCells[c2]);
			remainingCells.erase(remainingCells.begin() + c2);
			copies.insert(c1);
			doesExist = false;
		}
		c2 = 0;
		c1++;
	}
	int loop = 0;
	for (auto cop : copies) {
		newCells.erase(newCells.begin() + (cop - loop));
		loop++;
	}

	//Add and remove remaining

	for (auto& cell : newCells) {
		cell->AddCollider(collider);
	}
	for (auto cell : remainingCells) {
		cell->RemoveCollider(collider);
	}

	//merge both and set to collider.
	for (auto& cell : currentCells) {
		newCells.push_back(cell);
	}

	collider->SetCurrentNodes(newCells);
}

void OctSpatialPartition::GetActiveLeaves(OctNode* cell, std::set<OctNode*>& outSet) const
{
	if (cell->IsLeaf()) { 
		if (cell->IsEmpty()) return;
		outSet.insert(cell); 
	}
	else for (int i = 0; i < CHILDREN_NUMBER; i++) GetActiveLeaves(cell->GetChild(static_cast<EOctChildren>(i)), outSet);
}

std::vector<OctNode*> OctSpatialPartition::GetCollidingNodes(Ray& ray)
{
	std::vector<OctNode*> nodes;
	nodes.reserve(8);

	GetIntersectedLeaves(ray, root, nodes);
	return nodes;
}

std::vector<OctNode*> OctSpatialPartition::GetCollidingNodes(S_BoxBounds bounds)
{
	std::vector<OctNode*> nodes;
	nodes.reserve(8);

	GetIntersectedLeaves(bounds, root, nodes);
	return nodes;
}

std::vector<OctNode*> OctSpatialPartition::GetCollidingNodes(Sphere sphere)
{
	std::vector<OctNode*> nodes;
	nodes.reserve(8);

	GetIntersectedLeaves(sphere, root, nodes);
	return nodes;
}

std::vector<OctNode*> OctSpatialPartition::GetCollidingNodes(Box box)
{
	std::vector<OctNode*> nodes;
	nodes.reserve(8);

	GetIntersectedLeaves(box, root, nodes);
	return nodes;
}

void OctSpatialPartition::GetIntersectedLeaves(Ray& ray, OctNode* cell, std::vector<OctNode*> &nodes)
{
	//Check if cell is empty
	if (cell->IsEmpty()) return;

	if (CollisionDetection::RayAABBIntersection(ray, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) { 
			nodes.push_back(cell);

		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(ray, cell->GetChild(static_cast<EOctChildren>(i)), nodes);
	}
}

void OctSpatialPartition::GetIntersectedLeaves(Sphere& sphere, OctNode* cell, std::vector<OctNode*> &nodes)
{
	if (CollisionDetection::SphereAABBIntersection(sphere, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) {
			nodes.push_back(cell);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(sphere, cell->GetChild(static_cast<EOctChildren>(i)), nodes);
	}
}

void OctSpatialPartition::GetIntersectedLeaves(S_BoxBounds bounds, OctNode* cell, std::vector<OctNode*> &nodes)
{
	if (CollisionDetection::AABBIntersection(bounds, cell->GetBoundingBox()))
	{
		if (cell->IsLeaf()) {
			nodes.push_back(cell);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(bounds, cell->GetChild(static_cast<EOctChildren>(i)), nodes);
	}
}

void OctSpatialPartition::GetIntersectedLeaves(Box& box, OctNode* cell, std::vector<OctNode*> &nodes)
{
	if (CollisionDetection::AABBOBBIntersection(cell->GetBoundingBox(), box))
	{
		if (cell->IsLeaf()) {
			nodes.push_back(cell);
		}

		else for (int i = 0; i < CHILDREN_NUMBER; i++) GetIntersectedLeaves(box, cell->GetChild(static_cast<EOctChildren>(i)), nodes);
	}
}

void OctSpatialPartition::AddColliderToCell(C_BoundingBox* collider, OctNode* cell)
{
	if (CollisionDetection::AABBIntersection(collider->GetBoxBounds(), cell->GetBoundingBox())) {
		if (cell->IsLeaf())
		{
			collider->SetCurrentNodes(cell);
			cell->AddCollider(collider);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) AddColliderToCell(collider, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}

void OctSpatialPartition::AddColliderToCell(C_SphereCollider* collider, OctNode* cell)
{
	if (CollisionDetection::SphereAABBIntersection(collider->GetCollisionSphere(), cell->GetBoundingBox())) {
		if (cell->IsLeaf())
		{
			collider->SetCurrentNodes(cell);
			cell->AddCollider(collider);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) AddColliderToCell(collider, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}

void OctSpatialPartition::AddColliderToCell(C_BoxCollider* collider, OctNode* cell)
{
	if (CollisionDetection::AABBOBBIntersection(cell->GetBoundingBox(), collider->GetCollisionBox())) {
		if (cell->IsLeaf())
		{
			collider->SetCurrentNodes(cell);
			cell->AddCollider(collider);
		}
		else for (int i = 0; i < CHILDREN_NUMBER; i++) AddColliderToCell(collider, cell->GetChild(static_cast<EOctChildren>(i)));
	}
}
