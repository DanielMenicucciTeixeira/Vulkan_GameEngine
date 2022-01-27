#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "Math/FVector3.h"
#include "Geometry/BoxBounds.h"
#include <set>
#include <iostream>
#include "Objects/Components/Colliders/CollisionComponent.h"

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

class OctSpatialPartition
{
public:
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
		const S_BoxBounds GetBoundingBox() const;
		inline const bool& IsEmpty() const { return Empty; }
		inline void SetEmpty(bool isEmpty) { Empty = isEmpty; }

	private:
		friend OctSpatialPartition;
		S_BoxBounds* OctBounds;
		OctNode* Parent;
		OctNode* Children[CHILDREN_NUMBER];
		std::vector<C_CollisionComponent*> Colliders;
		float Size;
		bool Empty;

		static unsigned int ChildrenCount;
	};

	OctSpatialPartition(float worldSize, unsigned int depth = 3);

	~OctSpatialPartition();

	inline void AddCollider(C_CollisionComponent* collider) { AddColliderToCell(collider, Root); }

	std::set<OctNode*> GetActiveLeaves() const;
	std::set<OctNode*> GetIntersectedLeaves(Ray& ray) const;
	inline OctNode* GetRoot() { return Root; }

protected:
	OctNode* Root;
	void GetActiveLeaves(OctNode* cell, std::set<OctNode*>& outSet) const;
	void GetIntersectedLeaves(Ray& ray, OctNode* cell, std::set<OctNode*>& outSet) const;
	void AddColliderToCell(C_CollisionComponent* collider, OctNode* cell);
};
#endif
