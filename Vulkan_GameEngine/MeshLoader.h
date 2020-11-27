#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>

struct MeshStruct;

class MeshLoader
{
public:
	static void LoadMesh(std::string meshPath, MeshStruct* mesh);
};
#endif

