#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>
#include "Auxiliary/Singleton.h"
#include <set>
#include <vector>

struct S_Mesh;
class FVector3;

struct S_OBJData
{
	std::vector<FVector3> Vertices, TextureCoords, Normals;
	std::vector<unsigned int> Indices, TextureIndices, NormalIndicies;
};

class MeshLoader : StaticClass<MeshLoader>
{
public:
	static bool LoadMesh(std::string meshPath, S_Mesh* mesh);
	static bool LoadModel(std::string modelPath, std::set<S_Mesh*>& outMeshSet);
	static bool LoadMaterial(std::string materialName);
	static bool LoadMaterialLibrary(std::string materialPath);

protected:
	static void LoadMeshVertices(S_Mesh* mesh, S_OBJData& data);
};
#endif

