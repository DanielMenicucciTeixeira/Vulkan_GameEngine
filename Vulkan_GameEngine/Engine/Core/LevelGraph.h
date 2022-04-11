#ifndef LEVELGRAPH_H
#define LEVELGRAPH_H

#include "../Objects/Object.h"
#include <memory>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include "../Math/FMatrix4.h"
#include "Renderers/Materials/Material.h"
#include "Math/FTransform.h"

#include <fstream>
#include <iostream>

struct S_ModelData
{
	const FMatrix4* ModelMatrix;
	const bool* InFrustum;

public:
	inline S_ModelData(const FMatrix4* modelMatrix, const bool* inFrustum) : ModelMatrix(modelMatrix), InFrustum(inFrustum) {}
	inline S_ModelData(FMatrix4* modelMatrix, const bool* inFrustum) : ModelMatrix(modelMatrix), InFrustum(inFrustum) {}

	//The Coistructors and comparators with std::pair exist so I don't need to tell C_StaticMeshComponent about S_ModelData

	inline S_ModelData(std::pair<FMatrix4*, const bool*> pair) : ModelMatrix(pair.first), InFrustum(pair.second) {}
	inline S_ModelData(std::pair<const FMatrix4*, const bool*> pair) : ModelMatrix(pair.first), InFrustum(pair.second) {}
	
	inline bool operator==(std::pair<FMatrix4*, const bool*> pair) const { return ModelMatrix == pair.first; }
	inline bool operator==(std::pair<const FMatrix4*, const bool*> pair) const { return ModelMatrix == pair.first; }

	//This allows me to use this struct in sets and maps while still only comparing the ModelMatrix,
	//making it more efficient and guaranteeing there will be no duplicates.
	inline bool operator< (const S_ModelData& other) const { return ModelMatrix < other.ModelMatrix; }
	inline bool operator< (const S_ModelData& other) { return ModelMatrix < other.ModelMatrix; }
	inline bool operator< (S_ModelData& other) const { return ModelMatrix < other.ModelMatrix; }
	inline bool operator< (S_ModelData& other) { return ModelMatrix < other.ModelMatrix; }
};

typedef std::unordered_map < S_Mesh*, std::set<S_ModelData>> ModelsByMesh_T;
typedef std::unordered_map<Material*, ModelsByMesh_T> MeshesByMaterial_T;
typedef std::unordered_map<std::string, MeshesByMaterial_T> MaterialsByShader_T;

typedef unsigned int ShaderID;
typedef std::string ObjectName;

class O_Object;
class Material;
class C_RenderableComponent;
class C_StaticMeshComponent;
class C_CameraComponent;
class C_CollisionComponent;
class GO_Camera;
class OctNode;
struct S_Mesh;
struct S_Material;
struct S_Texture;
struct UniformCameraObject;

struct S_RenderData
{
	UniformCameraObject* Camera;
	MaterialsByShader_T DataMapByShader;

	std::set<FMatrix4*> Models;
	std::set<S_Texture*> Textures;
	std::set<S_CubeSampler*> CubeSamplers;
	std::vector<FMatrix4> LightSources;

	void Clear()
	{
		Camera = nullptr;
		DataMapByShader.clear();
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


	void Render();


	inline const std::set<O_Object*>& GetGameObjectsByTag(std::string tag) const { return GameObjectsByTag[tag]; }
	inline const std::set<O_Object*>& GetGameObjectsByClass(size_t classID) const { return GameObjectsByClass[classID]; }

	void AddObject(O_Object* gameObject);
	void RemoveObjectFromTagList(O_Object* gameObject, std::string tag);
	void AddObjectToTagList(O_Object* gameObject, std::string tag);
	void FrameBufferResizeCallback();

	void AddMesh(S_Mesh* mesh);
	void AddMeshComponent(C_StaticMeshComponent* meshComponent);
	void AddCollisionComponent(C_CollisionComponent* component);
	void RemoveMeshComponent(C_StaticMeshComponent* meshComponent);
	void AddTexture(S_Texture* texture);
	void AddCubeSampler(S_CubeSampler* sampler);
	void AddMaterial(Material* material);
	void AddLight(FMatrix4*& matrix, unsigned int& index);
	void RemoveLight(unsigned int index);

	//Camera

	//TODO:Should we add a remove function.
	void AddCamera(C_CameraComponent* cam, std::string = "");
	void SetActiveCamera(C_CameraComponent* camera);
	std::vector<C_CameraComponent*>* GetAllCameras();
	C_CameraComponent* GetCamera(int cameraID);
	
	//Getters

	inline std::unordered_map<std::string, S_Mesh*>& GetMeshes() { return MeshesByName; }
	inline std::unordered_map<std::string, Material*>& GetMaterials() { return MaterialsByName; }
	inline std::unordered_map<std::string, S_Texture*>& GetTextures() { return TexturesByName; }
	inline std::unordered_map<std::string, S_CubeSampler*>& GetCubeSamplers() { return CubeSamplersByName; }
	inline const std::map<std::string, O_Object*>& GetObjects() const { return GameObjectsByName; }
	inline S_RenderData* GetRenderData() { return &RenderData; }
	inline C_CameraComponent* GetActiveCamera() { return ActiveCamera; }

	friend std::ostream& operator<<(std::ostream& out, const O_Object& obj);

	void CleanUp();

	//Switch's the games pause state.
	void Pause();

	//Returns the games paused state.
	inline bool GetPaused() { return isPaused;};

protected:

	void LoadMesh();
	void LoadModel();
	void LoadMaterial(Material* material);
	bool LoadTexture(S_Texture*& texture, const std::string& textureName);

	static std::unique_ptr<LevelGraph> Instance;
	friend std::default_delete<LevelGraph>;
	
	std::unordered_map<std::string, S_Texture*> TexturesByName;
	std::unordered_map<std::string, S_CubeSampler*> CubeSamplersByName;
    std::unordered_map<std::string, Material*> MaterialsByName;
	std::unordered_map<std::string, S_Mesh*> MeshesByName;
	
	static std::set<O_Object*> UnloadedObjects;
	std::map<std::string, O_Object*> GameObjectsByName;
	static std::map<std::string, std::set<O_Object*>> GameObjectsByTag;
	static std::map<size_t, std::set<O_Object*>> GameObjectsByClass;


	S_RenderData RenderData;
	std::set<C_StaticMeshComponent*> StaticMehes;
	std::set<unsigned int> FreeLightSlots;

private:
	LevelGraph();
	~LevelGraph();

	bool isPaused;

	C_CameraComponent* ActiveCamera;
	std::vector<C_CameraComponent*> CameraList;
};
#endif

