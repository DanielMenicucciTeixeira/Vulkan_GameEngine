#pragma once

#include "../Engine/Objects/Components/Colliders/BoundingBox.h"
#include "../Engine/Objects/GameObjects/GameObject.h"

class Pocket: public O_GameObject {
public:
	Pocket();
	~Pocket();

	static void Overlap(O_GameObject* self, const S_CollisionData& data);

private:

	C_BoundingBox box;
};

