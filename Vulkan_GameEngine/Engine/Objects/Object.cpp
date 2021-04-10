#include "Object.h"
#include "LevelGraph.h"

void O_Object::AddTag(const std::string& tag)
{
	LevelGraph::GetInstance()->AddObjectToTagList(this, tag);
}

void O_Object::RemoveTag(const std::string& tag)
{
	LevelGraph::GetInstance()->RemoveObjectFromTagList(this, tag);
}

void O_Object::RemoveAllTags()
{
	for (auto& tag : Tags) LevelGraph::GetInstance()->RemoveObjectFromTagList(this, tag);
}
