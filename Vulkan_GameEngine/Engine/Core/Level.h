#ifndef O_LEVEL_H
#define O_LEVEL_H

#include "Objects/GameObjects/GameObject.h"
#include "Math/FTransform.h"
#include "DebugLogger.h"
#include <set>
#include <vector>
#include <unordered_map>
#include <string>

class C_CollisionComponent;
class Game;
class C_CameraComponent;
struct S_LightInfo;
struct S_Mesh;
struct S_Material;
struct S_Texture;
struct S_RenderData;

class O_Level : public O_Object
{
public:
	O_Level();
	virtual ~O_Level();

	virtual bool Initialize(Game* game);
	virtual void Start() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render();
	virtual void CleanUp();
	virtual void AddCollider(C_CollisionComponent* collider);
	virtual void AddLightSource(S_LightInfo* light);
	virtual void RemoveLightSource(S_LightInfo* light);
	inline std::set<O_Object*> GetObjects() { return LevelObjects; }

	//Checks if there is a CurrentCamera set, if not, checks for any other camera in the level and sets that as current camera.
	//In case no camera is found, returs false with a FatalError message.
	bool CheckForCamera();
	inline void SetCamera(C_CameraComponent* camera) { NextCamera = camera; ShouldChangeCamera = true; }

	template<class objectClass>
	objectClass* SpawnGameObjectOfClass(FTransform transform = FTransform())
	{
		static_assert(std::is_base_of<O_GameObject, objectClass>::value, "objectClass must derive from O_GameObject!");
		objectClass* gameObject = new objectClass(this);
		if (!dynamic_cast<O_GameObject*>(gameObject))
		{
			DebugLogger::Error("Invalid GameObject class: " + std::string(typeid(gameObject).name()), "Core/Level.h", __LINE__);
			return nullptr;
		}
		dynamic_cast<O_GameObject*>(gameObject)->SetTransform(transform);
		UnloadedObjects.insert(gameObject);
		dynamic_cast<O_GameObject*>(gameObject)->Start();
		return gameObject;
	}

	template<class objectClass>
	std::set<objectClass*> GetAllObjectsOfClass()
	{
		static_assert(std::is_base_of<O_Object, objectClass>::value, "objectClass must derive from O_Object!");
		std::set<objectClass*> result;
		for (const auto& object : GetAllObjectsOfClass)
		{
			if (dynamic_cast<objectClass*>(object)) result.insert(dynamic_cast<objectClass*>(object));
		}

		return result;
	}

	//Getters

	S_Mesh* GetMesh(std::string meshName);
	S_Material* GetMaterial(std::string materialName);

	//Loaders

	void LoadMesh(S_Mesh* mesh);
	void LoadMaterial(S_Material* material);
	bool LoadTexture(S_Texture*& texture, const std::string& textureName);
	void LoadLevelObjects();
	void ReloadLevelObjects();
	bool LoadCamera();

protected:

	bool FindAnyCamera();
	void ChangeCamera();

	std::set<O_Object*> LevelObjects;
	std::set<O_Object*> UnloadedObjects;
	std::vector<C_CollisionComponent*> Colliders;
	Game* CurrentGame;
	C_CameraComponent* CurrentCamera;
	C_CameraComponent* NextCamera;
	bool ShouldChangeCamera;

	std::unordered_map<std::string, S_Mesh*> Meshes;
	std::unordered_map<std::string, S_Material*> Materials;
	std::unordered_map<std::string, S_Texture*> Textures;
	S_RenderData* RenderData;
};
#endif
