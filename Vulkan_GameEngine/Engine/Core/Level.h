#ifndef O_LEVEL_H
#define O_LEVEL_H

#include "Objects/GameObjects/GameObject.h"
#include "Math/FTransform.h"
#include "DebugLogger.h"
#include <set>
#include <vector>
#include <unordered_map>
#include <string>

class LevelGraph;
class C_CollisionComponent;
class BaseGame;
class C_CameraComponent;
class FMatrix4;
struct S_Mesh;
struct S_Material;
struct S_Texture;
struct S_RenderData;

class L_Level
{
public:
	L_Level(float worldSize = 100.0f);
	virtual ~L_Level();

	virtual bool Initialize(BaseGame* game);
	virtual void Start();
	virtual void Update(const float deltaTime);
	virtual void Render();
	virtual void CleanUp();
	virtual void CheckCollisions();
	virtual void AddCollider(C_CollisionComponent* collider);
	bool CheckForCamera();
	inline void SetCamera(C_CameraComponent* camera) { NextCamera = camera;}

	template<class objectClass>
	objectClass* SpawnGameObjectOfClass(FTransform transform = FTransform(), std::string name = "")
	{
		static_assert(std::is_base_of<O_GameObject, objectClass>::value, "objectClass must derive from O_GameObject!");
		objectClass* gameObject = new objectClass(this, name);
		if (!dynamic_cast<O_GameObject*>(gameObject))
		{
			DebugLogger::Error("Invalid GameObject class: " + std::string(typeid(gameObject).name()), "Core/Level.h", __LINE__);
			return nullptr;
		}
		dynamic_cast<O_GameObject*>(gameObject)->SetTransform(transform);
		UnloadedObjects.insert(gameObject);
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

	//Loaders

	void LoadModels();
	void LoadMaterialLibrary();
	void LoadMaterial(S_Material* material);
	bool LoadTexture(S_Texture*& texture, const std::string& textureName);
	void LoadLevelObjects();
	void ReloadLevelObjects();
	bool LoadCamera(C_CameraComponent* camera);
	inline const float& GetWorldSize() const { return WorldSize; }

protected:

	std::string Name;

	bool FindAnyCamera();
	bool ChangeCamera();
	std::set<O_Object*> UnloadedObjects;
	BaseGame* CurrentGame;
	C_CameraComponent* NextCamera;

	std::set<std::string> ModelPaths;
	std::set<std::string> MaterialPaths;

	const float WorldSize;
};
#endif
