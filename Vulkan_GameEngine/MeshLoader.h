#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>

struct RenderObject;

class MeshLoader
{
public:
	static void LoadMesh(std::string meshPath, RenderObject* renderObject);
};
#endif

