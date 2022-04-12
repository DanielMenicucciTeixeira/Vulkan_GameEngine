#ifndef O_TETRAHEDRON_H
#define O_TETRAHEDRON_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class GO_Tetrahedron : public O_GameObject
{
protected:
	class C_StaticMeshComponent* Mesh = nullptr;
	class C_MovementComponent* Movement = nullptr;
	class C_MeshCollision* Collider = nullptr;
public:
	GO_Tetrahedron(L_Level* level = nullptr, std::string name = "");
	virtual ~GO_Tetrahedron();

	virtual void Update(float deltaTime) override;
	virtual void Start() override;

	inline C_StaticMeshComponent* GetMesh() { return Mesh; }
	inline C_MovementComponent* GetMovement() { return Movement; }

	static void OnOverlapBegin(O_GameObject* self, const struct S_CollisionData& data);

};

#endif

