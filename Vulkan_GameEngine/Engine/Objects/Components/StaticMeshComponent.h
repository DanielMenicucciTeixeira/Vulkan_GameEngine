#ifndef C_STATICMESHCOMPONENT_H
#define C_STATICMESHCOMPONENT_H

#include "RenderableComponent.h"
#include <string>
struct S_Mesh;
struct S_Texture;
struct FTransform;
class FMatrix4;

class C_StaticMeshComponent : public C_RenderableComponent
{
public:
	void Start() override;
	void Update(const float deltaTime) override;
	C_StaticMeshComponent(O_GameObject* owner);
	~C_StaticMeshComponent();

	///Getters
	inline FMatrix4* GetModelMatrix() const { return ModelMatrix; }
	inline S_Mesh* GetMesh() const { return Mesh; }
	inline S_Texture* GetTexture() const { return Texture; }
	inline const std::string& GetMeshName() const { return MeshName; }
	inline const std::string& GetTextureName() const { return TextureName; }

	///Setters
	inline void SetMesh(S_Mesh* mesh) { Mesh = mesh; }
	inline void SetTexture(S_Texture* texture) { Texture = texture; }
	inline void SetMeshName(const std::string& name) { MeshName = name; }
	inline void SetTextureName(const std::string& name) { TextureName = name; }

protected:
	FMatrix4* ModelMatrix;
	S_Mesh* Mesh = nullptr;
	S_Texture* Texture = nullptr;
	std::string TextureName;
	std::string MeshName;
};
#endif

