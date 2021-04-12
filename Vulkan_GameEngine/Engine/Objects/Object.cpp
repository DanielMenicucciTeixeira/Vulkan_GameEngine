#include "Object.h"
#include "LevelGraph.h"

bool O_Object::HasTag(std::string tag) const
{
	return Tags.count(tag);
}

void O_Object::AddTag(const std::string& tag)
{
	LevelGraph::GetInstance()->AddObjectToTagList(this, tag);
	Tags;
	int breakPoint = 0;
}

void O_Object::RemoveTag(const std::string& tag)
{
	LevelGraph::GetInstance()->RemoveObjectFromTagList(this, tag);
}

void O_Object::RemoveAllTags()
{
	for (auto& tag : Tags) LevelGraph::GetInstance()->RemoveObjectFromTagList(this, tag);
}
