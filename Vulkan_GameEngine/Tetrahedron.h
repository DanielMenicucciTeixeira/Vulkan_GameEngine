#ifndef O_TETRAHEDRON_H
#define O_TETRAHEDRON_H

#include "../Engine/Objects/GameObjects/GameObject.h"

class GO_Tetrahedron : public O_GameObject
{
protected:
	class C_StaticMeshComponent* Mesh[3] = { nullptr, nullptr, nullptr };
	class C_MeshCollision* Collider = nullptr;

	class C_BoundingBox* coll1 = nullptr;
public:
	GO_Tetrahedron(std::string name = "");
	virtual ~GO_Tetrahedron();

	virtual void Update(float deltaTime) override;
	virtual void Start() override;

	inline C_StaticMeshComponent* GetMesh() { return Mesh[0]; }

	static void OnOverlapBegin(O_GameObject* self, const struct S_CollisionData& data);
};

#endif

