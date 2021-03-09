#ifndef O_LEVEL_H
#define O_LEVEL_H

#include "Objects/Object.h"
#include "Objects/GameObjects/GameObject.h"
#include "Math/FTransform.h"
#include <set>
#include <vector>

class C_CollisionComponent;
class Game;

class O_Level : public O_Object
{
public:
	O_Level(Game* game);
	~O_Level();
	void Start() override;
	void Update(float deltaTime) override;
	void AddCollider(C_CollisionComponent* collider);
	inline std::set<O_Object*> GetObjects() { return LevelObjects; }

	template<class ObjectClass>
	ObjectClass* SpawnGameObjectOfClass(FTransform transform = FTransform())
	{
		ObjectClass* gameObject = new ObjectClass(this);
		dynamic_cast<O_GameObject*>(gameObject)->SetTransform(transform);
		LevelObjects.insert(gameObject);
		dynamic_cast<O_GameObject*>(gameObject)->Start();
		return gameObject;
	}

	template<class ObjectClass>
	std::set<ObjectClass*> GetAllObjectsOfClass()
	{
		std::set<ObjectClass*> result;
		for (const auto& object : GetAllObjectsOfClass)
		{
			if (dynamic_cast<ObjectClass*>(object)) result.insert(dynamic_cast<ObjectClass*>(object));
		}

		return result;
	}

protected:
	std::set<O_Object*> LevelObjects;
	std::vector<C_CollisionComponent*> Colliders;
	Game* CurrentGame;
};
#endif
