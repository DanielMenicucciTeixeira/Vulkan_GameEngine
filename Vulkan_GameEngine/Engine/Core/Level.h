#ifndef O_LEVEL_H
#define O_LEVEL_H

#include "../Engine/Objects/GameObjects/GameObject.h"
#include "../Math/FTransform.h"
#include "DebugLogger.h"
#include <set>
#include <vector>
#include <unordered_map>
#include <string>

class LevelGraph;
class C_CollisionComponent;
class FMatrix4;
struct S_Mesh;
class Material;
struct S_Texture;
struct S_CubeSampler;
struct S_RenderData;


/// <summary>
/// Designed to load information involving a level (actual management should go to LevelGraph).
/// </summary>
class L_Level
{
public:
	L_Level(float worldSize = 100.0f);
	virtual ~L_Level();

	virtual bool Initialize();
	virtual void Start();
	virtual void PreUpdate(const float deltaTime);
	virtual void Update(const float deltaTime);
	virtual void PostUpdate(const float deltaTime);
	virtual void Render();

	template<class objectClass>
	objectClass* SpawnGameObjectOfClass(FTransform transform = FTransform(), std::string name = "")
	{
		static_assert(std::is_base_of<O_GameObject, objectClass>::value, "objectClass must derive from O_GameObject!");
		objectClass* gameObject = new objectClass(name);
		if (!dynamic_cast<O_GameObject*>(gameObject))
		{
			DebugLogger::Error("Invalid GameObject class: " + std::string(typeid(gameObject).name()), "Core/Level.h", __LINE__);
			delete(gameObject);
			gameObject = nullptr;
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
	void LoadMaterial(Material* material);
	bool LoadTexture(S_Texture*& texture, const std::string& textureName);
	bool LoadCubeSampler(S_CubeSampler*& texture, const std::string& samplerName);
	void LoadLevelObjects();
	inline const float& GetWorldSize() const { return WorldSize; }

protected:

	std::string Name;

	std::set<O_Object*> UnloadedObjects;

	std::set<std::string> ModelPaths;
	std::set<std::string> MaterialPaths;

	const float WorldSize;
};
#endif
