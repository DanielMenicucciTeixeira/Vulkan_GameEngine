#include "Pocket.h"

Pocket::Pocket() : O_GameObject("Pocket", true), box(C_BoundingBox(this, ECollisionType::OVERLAP))
{
	box.SetOverlapBeginFunction(Overlap);
}

Pocket::~Pocket()
{
}

void Pocket::Overlap(O_GameObject* self, const S_CollisionData& data)
{
	int i = 0;
}
