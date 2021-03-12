#ifndef O_LEVEL_H
#define O_LEVEL_H

#include "Objects/Object.h"
#include "Objects/GameObjects/GameObject.h"
#include "Math/FTransform.h"
#include <set>
#include <vector>
#include <unordered_map>

class C_CollisionComponent;
class Game;
struct S_Mesh;
struct S_Material;
struct RenderInitializationData;

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
	inline std::set<O_Object*> GetObjects() { return LevelObjects; }

	template<class objectClass>
	objectClass* SpawnGameObjectOfClass(FTransform transform = FTransform())
	{
		objectClass* gameObject = new objectClass(this);
		dynamic_cast<O_GameObject*>(gameObject)->SetTransform(transform);
		LevelObjects.insert(gameObject);
		dynamic_cast<O_GameObject*>(gameObject)->Start();
		return gameObject;
	}

	template<class objectClass>
	std::set<objectClass*> GetAllObjectsOfClass()
	{
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
	void LoadLevelObjects();

protected:
	std::set<O_Object*> LevelObjects;
	std::vector<C_CollisionComponent*> Colliders;
	Game* CurrentGame;

	std::unordered_map<std::string, S_Mesh*> Meshes;
	std::unordered_map<std::string, S_Material*> Materials;
	RenderInitializationData* RenderData;
};
#endif
