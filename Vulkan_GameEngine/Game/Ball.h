#pragma once

#include "../Engine/Objects/GameObjects/GameObject.h"

class GO_Ball : public O_GameObject
{
protected:
	class C_StaticMeshComponent* Mesh = nullptr;
	class C_MeshCollision* Collider = nullptr;
public:
	GO_Ball(std::string name = "");
	virtual ~GO_Ball();

	virtual void Update(float deltaTime) override;
	virtual void Start() override;

	inline C_StaticMeshComponent* GetMesh() { return Mesh; }

	static void OnOverlapBegin(O_GameObject* self, const struct S_CollisionData& data);
};

