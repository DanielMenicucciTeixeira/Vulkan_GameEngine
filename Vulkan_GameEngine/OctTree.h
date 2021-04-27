#ifndef OCTTREE_H
#define OCTTREE_H

#include "Math/FVector3.h"

class C_BoundingBox;

constexpr int CHILDREN_NUMBER = 8;

enum class EOctChildren
{
	OCT_TLF,
	OCT_BLF,
	OCT_BRF,
	OCT_TRF,
	OCT_TLR,
	OCT_BLR,
	OCT_BRR,
	OCT_TRR
};

template <typename T>
class OctTree
{
	class OctNode
	{
	public:
		OctNode(FVector3 position, float size, OctNode* parent);
		~OctNode();
		void Octfy(int depth);

		inline OctNode* GetParent() const {}
		OctNode* GetChild(EOctChildren childPosition) const;
		void AddElement(T* element);
		int Count() const;
		bool IsLeaf() const;
		inline C_BoundingBox* GetBounds() const {}
		int GetChildCount() const;

	private:
		C_BoundingBox* Bounds;
		OctNode* Parent;
		OctNode* Children[CHILDREN_NUMBER];
	};
};
#endif
