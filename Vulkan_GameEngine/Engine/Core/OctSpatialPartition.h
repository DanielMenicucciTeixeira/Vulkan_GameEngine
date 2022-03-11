#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "../Math/FVector3.h"
#include "Geometry/BoxBounds.h"
#include <set>
#include <iostream>
#include "../Objects/Components/Colliders/CollisionComponent.h"

constexpr unsigned int CHILDREN_NUMBER = 8;

enum class EOctChildren
{
	OCT_LBB,
	OCT_LBF,
	OCT_LTB,
	OCT_LTF,
	OCT_RBB,
	OCT_RBF,
	OCT_RTB,
	OCT_RTF
};

class Ray;

class C_BoundingBox;
class C_SphereCollider;
class C_BoxCollider;

class OctNode
{
public:
	OctNode(FVector3 position, float size, OctNode* parent);

	~OctNode();

	void Octify(unsigned int depth);

	inline OctNode* GetParent() const { return Parent; }
	inline OctNode* GetChild(EOctChildren childPostion) const { return Children[static_cast<int>(childPostion)]; }

	inline void AddCollider(C_BoundingBox* element) { AABBColliders.push_back(element); ColliderCount++; }
	inline void AddCollider(C_SphereCollider* element) { SphereColliders.push_back(element); ColliderCount++; }
	inline void AddCollider(C_BoxCollider* element) { OBBColliders.push_back(element); ColliderCount++; }
	void RemoveCollider(C_BoundingBox* element);
	void RemoveCollider(C_SphereCollider* element);
	void RemoveCollider(C_BoxCollider* element);

	inline std::vector<C_BoundingBox*> GetAABBColliders() { return AABBColliders; }
	inline std::vector<C_SphereCollider*> GetSphereColliders() { return SphereColliders; }
	inline std::vector<C_BoxCollider*> GetOBBColliders() { return OBBColliders; }
	inline int GetColliderCount() { return ColliderCount; }
	inline int GetAABBCount() { return AABBColliders.size(); }
	inline int GetSphereCount() { return SphereColliders.size(); }
	inline int GetOBBCount() { return OBBColliders.size(); }
	inline static const unsigned int& GetChildrenCount() { return ChildrenCount; };
	inline bool IsLeaf() const { return Children[0] == nullptr; }
	S_BoxBounds GetBoundingBox() const;
	inline const bool IsEmpty() const { return (AABBColliders.empty() && SphereColliders.empty() && OBBColliders.empty()); }

private:
	friend class OctSpatialPartition;
	S_BoxBounds* OctBounds;
	OctNode* Parent;
	OctNode* Children[CHILDREN_NUMBER];


	//This is done instead of a single vector as a memory trade for faster Update calls due to removeing the need for a switch and cast.
	std::vector <C_BoundingBox*> AABBColliders;
	std::vector <C_SphereCollider*> SphereColliders;
	std::vector <C_BoxCollider*> OBBColliders;
	float ColliderCount;
	float Size;
	bool Empty;

	static unsigned int ChildrenCount;
};


class OctSpatialPartition
{
public:

	OctSpatialPartition(float worldSize, unsigned int depth = 3);

	~OctSpatialPartition();

	void AddCollider(C_CollisionComponent* collider);

	std::set<OctNode*> GetActiveLeaves() const;

	std::vector<C_CollisionComponent*> GetCollision(Ray& ray);
	std::vector<C_CollisionComponent*> GetCollision(Sphere& sphere);
	std::vector<C_CollisionComponent*> GetCollision(S_BoxBounds& bounds);
	std::vector<C_CollisionComponent*> GetCollision(Box& box);

	//TODO: make these work better with multiple collisions

	//Check to see if the collider is still in the same bounding box
	void UpdateColliderNode(C_BoundingBox* collider);
	void UpdateColliderNode(C_SphereCollider* collider);
	void UpdateColliderNode(C_BoxCollider* collider);

	inline OctNode* GetRoot() { return root; }

protected:
	void GetActiveLeaves(OctNode* cell, std::set<OctNode*>& outSet) const;

private:
	OctNode* root;
	std::vector<C_CollisionComponent*> intersectionList;

	//Functions to get which cell/s the are being collided with
	std::vector<OctNode*> GetCollidingNodes(Ray& ray);
	std::vector<OctNode*> GetCollidingNodes(S_BoxBounds bounds);
	std::vector<OctNode*> GetCollidingNodes(Sphere sphere);
	std::vector<OctNode*> GetCollidingNodes(Box box);

	//Query functions for GetCollidingNodes
	void GetIntersectedLeaves(Ray& ray, OctNode* cell, std::vector<OctNode*> nodes);
	void GetIntersectedLeaves(Sphere& sphere, OctNode* cell, std::vector<OctNode*> nodes);
	void GetIntersectedLeaves(S_BoxBounds bounds, OctNode* cell, std::vector<OctNode*> nodes);
	void GetIntersectedLeaves(Box& box, OctNode* cell, std::vector<OctNode*> nodes);


	void AddColliderToCell(C_BoundingBox* collider, OctNode* cell);
	void AddColliderToCell(C_SphereCollider* collider, OctNode* cell);
	void AddColliderToCell(C_BoxCollider* collider, OctNode* cell);



};
#endif
