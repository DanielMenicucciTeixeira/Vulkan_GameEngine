#include "CollisionComponent.h"
#include "Geometry/Ray.h"
#include "Geometry/Sphere.h"
#include "Geometry/Box.h"
#include "Geometry/Plane.h"
#include "Math/FVector3.h"
#include <cmath>
#include <iostream>
#include "BoxCollision.h"
#include "GameObject.h"
#include "Level.h"

bool C_CollisionComponent::IsCollidingWith(C_CollisionComponent* collider)
{
	if (OverlapedColliders.count(collider)) return true;
	return false;
}

bool C_CollisionComponent::RaySphereCollision(const Ray& ray, const Sphere& sphere, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision)
{
	Ray line = ray;

	float a, b, c;
	a = 1.0f;
	b = (line.GetDirection() * (line.GetStartPosition() - sphere.position) * 2.0f);
	c = ((line.GetStartPosition() - sphere.position) * (line.GetStartPosition() - sphere.position)) - (sphere.radius * sphere.radius);

	float delta = (b * b) - 4 * a * c;

	if (delta < 0)
	{
		return false;
	}
	else if (delta == 0)
	{
		collisionPoints[0] = line.GetPosition(-b / (2.0f * a));
		return true;
	}
	else if (!stopAtFirstCollision)
	{
		float q = (b > 0) ? -0.5 * (b + sqrt(delta)) : -0.5 * (b - sqrt(delta));
		collisionPoints[0] = line.GetPosition(q / a);
		collisionPoints[1] = line.GetPosition(c / q);

		return true;
	}
	else
	{
		float q = (b > 0) ? -0.5 * (b + sqrt(delta)) : -0.5 * (b - sqrt(delta));
		FVector3 point0 = line.GetPosition(q / a);
		FVector3 point1 = line.GetPosition(c / q);

		if ((line.GetStartPosition() - point0).Length() <= (line.GetStartPosition() - point1).Length()) collisionPoints[0] = point0;
		else collisionPoints[0] = point1;

		return true;
	}
}

bool C_CollisionComponent::RayBoxCollision(const Ray& ray, const Box& box, FVector3 collisionPoints[2], S_CollisionData& data, bool stopAtFirstCollision)
{
	Ray line = ray;
	int i = 0;
	FVector3 interssection[6];
	for (auto plane : box.box)
	{
		if
			(
				plane.InterssectionPoint(line, interssection[i]) &&
				interssection[i].X <= box.GetExtent().X && interssection[i].X >= 0 &&
				interssection[i].Y <= box.GetExtent().Y && interssection[i].Y >= 0 &&
				interssection[i].Z <= box.GetExtent().Z && interssection[i].Z >= 0
				)
		{
			i++;
			if (i >= 2) break;
		}
	}


	if (i == 0) return false;
	else
	{
		if (stopAtFirstCollision)
		{
			FVector3 closestPoint = interssection[0];
			for (int j = 1; j <= i; j++)
			{
				if ((line.GetStartPosition() - interssection[j]).Length() < (line.GetStartPosition() - closestPoint).Length())
				{
					closestPoint = interssection[j];
				}
			}
			collisionPoints[0] = closestPoint;
		}
		else
		{
			for (int j = 0; j <= i; j++)
			{
				collisionPoints[j] = interssection[j];
			}
		}

		data.CollisionPoint = collisionPoints[0];
		return true;
	}

}

bool C_CollisionComponent::SphereSphereCollision(const Sphere& sphere0, const Sphere& sphere1, S_CollisionData& data, float tolerance)
{
	float distance = (sphere0.position - sphere1.position).Length();
	if (distance - (sphere0.radius + sphere1.radius) <= tolerance)
	{
		data.CollisionPoint = ((sphere0.position - sphere1.position).GetNormal() * sphere1.radius) + sphere1.position;
		return true;
	}
	else return false;
}

bool C_CollisionComponent::SpherePlaneCollision(const Sphere& sphere, const FVector3& direction, const Plane& plane, S_CollisionData& data)
{

	FVector3 collisionDirection = ((plane.GetPlaneNormal() * -1) + sphere.position).GetNormal();

	FVector3 collisionPoint;
	bool result = plane.InterssectionPoint(Ray(sphere.position, collisionDirection, sphere.radius, false), collisionPoint);
	data.CollisionPoint = collisionPoint;
	return result;
}

bool C_CollisionComponent::SphereBoxCollision(const Sphere& sphere, const Box& box, S_CollisionData& data)
{
	if
		(
			   (sphere.position.X - sphere.radius <= box.GetPosition().X + box.GetExtent().X && sphere.position.X + sphere.radius >= box.GetPosition().X)
			&& (sphere.position.Y - sphere.radius <= box.GetPosition().Y + box.GetExtent().Y && sphere.position.Y + sphere.radius >= box.GetPosition().Y)
			&& (sphere.position.Z - sphere.radius <= box.GetPosition().Z + box.GetExtent().Z && sphere.position.Z + sphere.radius >= box.GetPosition().Z)
		)
	{
		for (const auto& plane : box.box)
		{
			float distance = (sphere.position - plane.GetRandomPointInPlane()) * plane.GetPlaneNormal();
			if (distance < sphere.radius)
			{
				data.CollisionPoint = (plane.GetPlaneNormal() * sphere.radius) + sphere.position;
				return true;
			}
		}
	}
	int debug = 0;
	return false;
}

void C_CollisionComponent::CheckForCollisions(std::vector<C_CollisionComponent*> colliderVector)
{
	S_CollisionData data;
	for (int i = 0; i < colliderVector.size(); i++)
	{
		for (int j = i+1; j < colliderVector.size(); j++)
		{
			if (colliderVector[i]->Collide(colliderVector[j], data))
			{
				colliderVector[i]->AddOverlapedCollider(colliderVector[j]);
				colliderVector[j]->AddOverlapedCollider(colliderVector[i]);

				data.OtherGameObject = colliderVector[j]->GetOwner();
				data.OtherCollisonComponent = colliderVector[j];
				colliderVector[i]->ChooseCollisionType(colliderVector[j], data);

				data.OtherGameObject = colliderVector[i]->GetOwner();
				data.OtherCollisonComponent = colliderVector[i];
				colliderVector[j]->ChooseCollisionType(colliderVector[i], data);
			}
		}
	}
}

void C_CollisionComponent::ChooseCollisionType(C_CollisionComponent* otherCollider, const S_CollisionData& data)
{
	if (this->CollisionType == NO_COLLISION || otherCollider->CollisionType == NO_COLLISION) return;

	if (this->CollisionType == BOTH)
	{
		switch (otherCollider->CollisionType)
		{
		case OVERLAP:
			OnOverlapBegin(data);
		case COLLISION:
			OnCollision(data);
		default:
			break;
		}
	}
	else if (otherCollider->CollisionType == BOTH || this->CollisionType == otherCollider->CollisionType)
	{
		switch (this->CollisionType)
		{
		case OVERLAP:
			OnOverlapBegin(data);
		case COLLISION:
			OnCollision(data);
		default:
			break;
		}
	}
	else OnOverlapBegin(data);
}


bool C_CollisionComponent::Collide(C_CollisionComponent* otherCollider, S_CollisionData& data) const
{
	return false;
}

void C_CollisionComponent::OnCollision(const S_CollisionData& data)
{
	if(CollisionFunction) CollisionFunction(Owner, data);
}

void C_CollisionComponent::OnOverlapBegin(const S_CollisionData& data)
{
	if(OverlapBeginFunction) OverlapBeginFunction(Owner, data);
}

void C_CollisionComponent::OnOverlapEnd(C_CollisionComponent* otherCollider)
{
	if (OverlapEndFunction)
	{
		OverlapEndFunction(Owner, otherCollider);
	}

	int debug = 0;
}

void C_CollisionComponent::Update(float deltaTime)
{
	C_TransformComponent::Update(deltaTime);
	S_CollisionData data;

	std::set<C_CollisionComponent*> collidersToRemove;

	for (const auto& collider : OverlapedColliders)
	{
		if (!Collide(collider, data))
		{
			OnOverlapEnd(collider);
			collidersToRemove.insert(collider);
		}
	}

	for (const auto& collider : collidersToRemove) OverlapedColliders.erase(collider);
}

C_CollisionComponent::C_CollisionComponent(O_GameObject* owner, ECollisionType collisionType) : C_TransformComponent(owner)
{
	CollisionType = collisionType;
	Owner->GetLevel()->AddCollider(this);
}

C_CollisionComponent::~C_CollisionComponent()
{
	C_TransformComponent::~C_TransformComponent();
}
