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

using namespace std;

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

	std::unordered_map<std::string, std::set<Material*>> MaterialsByShader;
	std::unordered_map<Material*, std::set<S_Mesh*>> MeshesByMaterial;
	std::unordered_map<S_Mesh*, std::set<FMatrix4*>> InstancesByMesh;

	std::unordered_map<FMatrix4*, const bool*> Models;
	std::set<S_Texture*> Textures;
	std::set<S_Mesh*> Meshes;
	std::set<Material*> Materials;
	std::vector<FMatrix4> LightSources;

	void Clear()
	{
		Camera = nullptr;
		MaterialsByShader.clear();
		MeshesByMaterial.clear();
		InstancesByMesh.clear();
		Models.clear();
		Materials.clear();
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
	void AddMaterial(Material* material);
	void AddLight(FMatrix4*& matrix, unsigned int& index);
	void RemoveLight(unsigned int index);

	//Camera

	//TODO:Should we add a remove function.
	void AddCamera(std::string = "", FTransform transform = FTransform());
	void AddCamera(GO_Camera* cam, std::string = "");
	void SetActiveCamera(C_CameraComponent* camera);
	std::vector<GO_Camera>* GetAllCameras();
	GO_Camera * GetCamera(int cameraID);
	
	//Getters

	inline std::unordered_map<std::string, S_Mesh*>& GetMeshes() { return MeshesByName; }
	inline std::unordered_map<std::string, Material*>& GetMaterials() { return MaterialsByName; }
	inline std::unordered_map<std::string, S_Texture*>& GetTextures() { return TexturesByName; }
	inline const std::map<std::string, O_Object*>& GetObjects() const { return GameObjectsByName; }
	inline S_RenderData* GetRenderData() { return &RenderData; }
	inline C_CameraComponent* GetActiveCamera() { return ActiveCamera; }

	friend ostream& operator<<(ostream& out, const O_Object& obj);

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
	std::vector<GO_Camera> CameraList;
};
#endif

