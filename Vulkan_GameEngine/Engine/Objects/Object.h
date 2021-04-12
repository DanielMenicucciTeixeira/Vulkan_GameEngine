#ifndef O_OBJECT_H
#define O_OBJECT_H

#include <set>
#include <string>

class L_Level;
union SDL_Event;

class O_Object
{
public:
	O_Object(std::string name = "") { Level = nullptr; name = name; }
	virtual ~O_Object() {}

	virtual void Start() {};
	virtual void Update(const float deltaTime) {};
	bool UpdateWhenPaused = false;

	///Tag Functions
	inline const std::set<std::string>& GetTags() const { return Tags; }
	virtual bool HasTag(std::string tag) const;
	virtual void AddTag(const std::string& tag);
	virtual void RemoveTag(const std::string& tag);
	virtual void RemoveAllTags();
	inline L_Level* GetLevel() const { return Level; }
	inline const std::string GetName() const { return Name; }

protected:
	std::set<std::string> Tags;
	std::string Name = "";
	L_Level* Level;

private:
	inline void SetName(std::string name) { Name = name; }
	friend class LevelGraph;
};
#endif // !O_OBJECT_H

