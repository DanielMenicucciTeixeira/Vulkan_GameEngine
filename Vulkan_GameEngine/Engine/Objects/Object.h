#ifndef O_OBJECT_H
#define O_OBJECT_H

#include <set>
#include <string>
class O_Level;

class O_Object
{
public:
	virtual void Start() {};
	virtual void Update(float deltaTime) {};
	bool UpdateWhenPaused = false;
	
	///Tag Functions
	inline std::set<std::string> GetTags() const { return Tags; }
	inline void AddTag(const std::string& tag) { Tags.insert(tag); }
	inline void RemoveTag(const std::string& tag) { Tags.erase(tag); }
	inline void RemoveAllTags() { Tags.clear(); }
	inline O_Level* GetLevel() const { return Level; }
	inline void SetName(std::string name) { Name = name; }
	inline std::string GetName() { return Name; }

protected:
	std::set<std::string> Tags;
	std::string Name = "";
	O_Level* Level;
};
#endif // !O_OBJECT_H

