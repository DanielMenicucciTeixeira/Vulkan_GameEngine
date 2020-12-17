#ifndef C_STATICMESHCOMPONENT_H
#define C_STATICMESHCOMPONENT_H

#include "RenderableComponent.h"
struct S_Mesh;
struct S_Texture;
struct FTransform;
class FMatrix4;

class C_StaticMeshComponent : public C_RenderableComponent
{
public:
	void Start() override;
	void Update(float deltaTime) override;
	C_StaticMeshComponent(O_GameObject* owner);
	~C_StaticMeshComponent();

	///Getters
	inline FMatrix4* GetModelMatrix() const { return ModelMatrix; }
	inline S_Mesh* GetMesh() const { return Mesh; }
	inline S_Texture* GetTexture() const { return Texture; }

	///Setters
	inline void SetMesh(S_Mesh* mesh) { Mesh = mesh; }
	inline void SetTexture(S_Texture* texture) { Texture = texture; }

protected:
	FMatrix4* ModelMatrix;
	S_Mesh* Mesh = nullptr;
	S_Texture* Texture = nullptr;
};
#endif

