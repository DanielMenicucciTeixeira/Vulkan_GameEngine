#include "CollisionDetection.h"
#include "BoundingBox.h"

CollisionDetection::~CollisionDetection()
{
}

bool CollisionDetection::Collision(C_BoundingBox a, C_BoundingBox b)
{
	//FVector3 minCorner = a.GetMin() glm::vec3(transform_[3].x, transform_[3].y, transform_[3].z) + point_; GetTransformedPoint(minVert, transform);
	//FVector3 maxCorner = GetTransformedPoint(maxVert, transform);

	//FVector3 otherMinCorner = GetTransformedPoint(box_->minVert, box_->transform);
	//FVector3 otherMaxCorner = GetTransformedPoint(box_->maxVert, box_->transform);

	//if (minCorner.x <= otherMaxCorner.x && maxCorner.x >= otherMinCorner.x &&
	//	minCorner.y <= otherMaxCorner.y && maxCorner.y >= otherMinCorner.y &&
	//	minCorner.z <= otherMaxCorner.z && maxCorner.z >= otherMinCorner.z) {
	//	return true;
	//}

	return false;
}
