#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <string>
#include "Auxiliary/Singleton.h"
#include <set>
#include <vector>

struct S_Mesh;
struct S_Material;
class FVector3;

struct S_OBJData
{
	std::vector<FVector3> Position, TextureCoords, Normals;
	std::vector<unsigned int> PositionIndices, TextureIndices, NormalIndicies;
};

class AssetLoader : StaticClass<AssetLoader>
{
public:
	static bool LoadMesh(std::string meshPath, S_Mesh* mesh);
	static bool LoadModel(std::string modelPath, std::set<S_Mesh*>& outMeshSet);
	static bool LoadMaterial(std::string materialPath, S_Material* outMaterial);
	static bool LoadMaterialLibrary(std::string materialPath, std::set<S_Material*>& outMaterialSet);

protected:
	static void LoadMeshVertices(S_Mesh* mesh, S_OBJData& data);
};
#endif

