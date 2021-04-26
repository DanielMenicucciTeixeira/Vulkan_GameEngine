#ifndef LEVELGRAPH_H
#define LEVELGRAPH_H

#include "Objects/Object.h"

#include <memory>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include "Math/FMatrix4.h"

typedef unsigned int ShaderID;
typedef std::string ObjectName;

class O_Object;
class C_RenderableComponent;
class C_StaticMeshComponent;
class C_CameraComponent;
class C_CollisionComponent;
struct S_Mesh;
struct S_Material;
struct S_Texture;
struct UniformCameraObject;

struct S_RenderData
{
	UniformCameraObject* Camera;

	std::unordered_map<std::string, std::set<S_Material*>> MaterialsByShader;
	std::unordered_map<S_Material*, std::set<S_Mesh*>> MeshesByMaterial;
	std::unordered_map<S_Mesh*, std::set<FMatrix4*>> InstancesByMesh;

	std::unordered_map<FMatrix4*, const bool*> Models;
	std::set<S_Texture*> Textures;
	std::set<S_Material*> Materials;
	std::set<S_Mesh*> Meshes;
	std::vector<FMatrix4> LightSources;

	void Clear()
	{
		Camera = nullptr;
		MaterialsByShader.clear();
		MeshesByMaterial.clear();
		InstancesByMesh.clear();
		Models.clear();
		Textures.clear();
		LightSources.clear();
	}
};

class LevelGraph
{
public:
	static LevelGraph* GetInstance();

	LevelGraph(const LevelGraph&) = delete;
	LevelGraph(LevelGraph&&) = delete;
	LevelGraph& operator=(const LevelGraph&) = delete;
	LevelGraph& operator=(LevelGraph&&) = delete;

	inline const std::set<O_Object*>& GetGameObjectsByTag(std::string tag) const { return GameObjectsByTag[tag]; }
	inline const std::set<O_Object*>& GetGameObjectsByClass(size_t classID) const { return GameObjectsByClass[classID]; }

	void AddObject(O_Object* gameObject);
	void RemoveObjectFromTagList(O_Object* gameObject, std::string tag);
	void AddObjectToTagList(O_Object* gameObject, std::string tag);
	void SetActiveCamera(C_CameraComponent* camera);
	void FrameBufferResizeCallback();

	void AddMesh(S_Mesh* mesh);
	void AddMeshComponent(C_StaticMeshComponent* meshComponent);
	void AddCollisionComponent(C_CollisionComponent* component);
	void RemoveMeshComponent(C_StaticMeshComponent* meshComponent);
	void AddTexture(S_Texture* texture);
	void AddMaterial(S_Material* material);
	void AddLight(FMatrix4*& matrix, unsigned int& index);
	void RemoveLight(unsigned int index);

	inline std::unordered_map<std::string, S_Mesh*>& GetMeshes() const { return MeshesByName; }
	inline std::unordered_map<std::string, S_Material*>& GetMaterials() const { return MaterialsByName; }
	inline std::unordered_map<std::string, S_Texture*>& GetTextures() const { return TexturesByName; }
	inline std::map<std::string, O_Object*>& GetObjects() const { return GameObjectsByName; }
	inline S_RenderData* GetRenderData() { return &RenderData; }
	inline C_CameraComponent* GetActiveCamera() { return ActiveCamera; }
	inline const std::vector<C_CollisionComponent*>& GetColliders() const { return Colliders; }


	void CleanUp();

protected:

	void LoadMesh();
	void LoadModel();
	void LoadMaterial(S_Material* material);
	bool LoadTexture(S_Texture*& texture, const std::string& textureName);

	static std::unique_ptr<LevelGraph> Instance;
	friend std::default_delete<LevelGraph>;
	
	static std::unordered_map<std::string, S_Texture*> TexturesByName;
	static std::unordered_map<std::string, S_Material*> MaterialsByName;
	static std::unordered_map<std::string, S_Mesh*> MeshesByName;
	
	static std::set<O_Object*> UnloadedObjects;
	static std::map<std::string, O_Object*> GameObjectsByName;
	static std::map<std::string, std::set<O_Object*>> GameObjectsByTag;
	static std::map<size_t, std::set<O_Object*>> GameObjectsByClass;
	static C_CameraComponent* ActiveCamera;
	static std::vector<C_CollisionComponent*> Colliders;
	
	S_RenderData RenderData;
	std::set<C_StaticMeshComponent*> StaticMehes;
	std::set<unsigned int> FreeLightSlots;

private:
	LevelGraph();
	~LevelGraph();

	friend class L_Level;
};

#endif

