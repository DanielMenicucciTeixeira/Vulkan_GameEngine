#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "../Math/FVector3.h"
#include "Geometry/BoxBounds.h"
#include <set>
#include <iostream>
#include "../Objects/Components/Colliders/CollisionComponent.h"
#include "Physics/CollisionData.h"

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

class C_CollisionComponent;
class Ray;

class OctNode
{
public:
	OctNode(FVector3 position, float size, OctNode* parent);

	~OctNode();

	void Octify(unsigned int depth);

	inline OctNode* GetParent() const { return Parent; }
	inline OctNode* GetChild(EOctChildren childPostion) const { return Children[static_cast<int>(childPostion)]; }

	inline void AddCollider(C_CollisionComponent* element) { Colliders.push_back(element); }

	inline int GetColliderCount() const { return Colliders.size(); }
	inline std::vector<C_CollisionComponent*> GetColliders() { return Colliders; }
	inline static const unsigned int& GetChildrenCount() { return ChildrenCount; };
	inline bool IsLeaf() const { return Children[0] == nullptr; }
	S_BoxBounds GetBoundingBox() const;
	inline const bool IsEmpty() const { return Colliders.empty(); }

private:
	friend class OctSpatialPartition;
	S_BoxBounds* OctBounds;
	OctNode* Parent;
	OctNode* Children[CHILDREN_NUMBER];
	std::vector<C_CollisionComponent*> Colliders;
	float Size;
	bool Empty;

	static unsigned int ChildrenCount;
};


class OctSpatialPartition
{
public:

	OctSpatialPartition(float worldSize, unsigned int depth = 3);

	~OctSpatialPartition();

	inline void AddCollider(C_CollisionComponent* collider) { AddColliderToCell(collider, root); }

	std::set<OctNode*> GetActiveLeaves() const;
	//the bool is if you want to get all of the collisions not just the shortest.
	std::vector<S_CollisionData> GetCollision(Ray& ray, bool getAll);
	std::vector<S_CollisionData> GetCollision(Sphere& sphere);
	std::vector<S_CollisionData> GetCollision(S_BoxBounds& bounds);
	inline OctNode* GetRoot() { return root; }

	//Does collision detection on every node.
	//TODO: Improve this so that it only needs to go through handlers and will just give back the sets as needed.
	void Update(const float deltaTime_);

protected:
	void GetActiveLeaves(OctNode* cell, std::set<OctNode*>& outSet) const;
	void AddColliderToCell(C_CollisionComponent* collider, OctNode* cell);

private:
	OctNode* root;
	void GetIntersectedLeaves(Ray& ray, OctNode* cell, bool getAll);
	void GetIntersectedLeaves(Sphere& sphere, OctNode* cell);
	void GetIntersectedLeaves(S_BoxBounds& bounds, OctNode* cell);
	std::vector<S_CollisionData> intersectionList;
};
#endif
