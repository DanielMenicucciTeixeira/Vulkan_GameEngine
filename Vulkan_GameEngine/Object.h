#ifndef O_OBJECT_H
#define O_OBJECT_H

#include <set>
#include <string>

class O_Object
{
public:
	virtual void Update(float deltaTime) {};
	
	///Tag Functions
	inline std::set<std::string> GetTags() { return Tags; }
	inline void AddTag(const std::string& tag) { Tags.insert(tag); }
	inline void RemoveTag(const std::string& tag) { Tags.erase(tag); }
	inline void RemoveAllTags() { Tags.clear(); }

protected:
	std::set<std::string> Tags;
};
#endif // !O_OBJECT_H

