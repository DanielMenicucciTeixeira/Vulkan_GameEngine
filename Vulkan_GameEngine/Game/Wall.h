#pragma once

#include "../Engine/Objects/GameObjects/GameObject.h"
#include "../Engine/Physics/CollisionData.h"

class GO_Wall : public O_GameObject
{
protected:
	class C_StaticMeshComponent* Mesh = nullptr;
	class C_MeshCollision* Collider = nullptr;
public:
	GO_Wall(std::string name = "");
	virtual ~GO_Wall();

	virtual void Update(float deltaTime) override;
	virtual void Start() override;

	inline C_StaticMeshComponent* GetMesh() { return Mesh; }

	static void Overlap(O_GameObject* self, const S_CollisionData& data);
};

