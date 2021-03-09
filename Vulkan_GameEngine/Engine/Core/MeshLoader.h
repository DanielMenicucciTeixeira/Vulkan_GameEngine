#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>

struct S_Mesh;

class MeshLoader
{
public:
	static void LoadMesh(std::string meshPath, S_Mesh* mesh);
};
#endif

