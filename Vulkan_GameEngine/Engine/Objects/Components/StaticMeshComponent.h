#ifndef C_STATICMESHCOMPONENT_H
#define C_STATICMESHCOMPONENT_H

#include "RenderableComponent.h"
#include <string>
struct S_Mesh;
struct S_Material;
struct S_Texture;
struct FTransform;
class FMatrix4;
class C_BoundingBox;

class C_StaticMeshComponent : public C_RenderableComponent
{
public:
	virtual void Start() override;
	virtual void Update(const float deltaTime) override;
	C_StaticMeshComponent(O_GameObject* owner);
	virtual ~C_StaticMeshComponent();

	///Getters
	inline FMatrix4* GetModelMatrix() const { return ModelMatrix; }
	inline S_Mesh* GetMesh() const { return Mesh; }
	S_Texture* GetTextureDifuse() const;
	S_Texture* GetTextureSpecular() const;
	inline S_Material* GetMaterial() const { return Material; }
	inline const std::string& GetMeshName() const { return MeshName; }
	inline const std::string& GetMaterialName() const { return MaterialName; }

	///Setters
	inline void SetMesh(S_Mesh* mesh) { Mesh = mesh; }
	inline void SetMaterial(S_Material* material) { Material = material; }
	inline void SetMeshName(const std::string& name) { MeshName = name; }
	inline void SetMaterialName(const std::string& name) { MaterialName = name; }
	inline const bool* IsInFrustum() const { return &InFrustum; }
	void SetInFrustum(bool inFrustum);
	inline C_BoundingBox* GetBoundingBox() { return Box; }

protected:
	void UpdateModelMatrix();

	FMatrix4* ModelMatrix;
	C_BoundingBox* Box;

	S_Mesh* Mesh = nullptr;
	std::string MeshName;

	S_Material* Material = nullptr;
	std::string MaterialName;

	bool InFrustum = true;
};
#endif

