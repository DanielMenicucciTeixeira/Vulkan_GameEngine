#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <string>
#include <set>
#include <vector>

struct S_Mesh;
class Material;
class FVector3;

struct S_OBJData
{
	std::vector<FVector3> Position, TextureCoords, Normals;
	std::vector<unsigned int> PositionIndices, TextureIndices, NormalIndicies;
};

class AssetLoader
{
public:
	AssetLoader(const AssetLoader&) = delete;
	AssetLoader(AssetLoader&&) = delete;
	AssetLoader& operator =(const AssetLoader&) = delete;
	AssetLoader& operator =(AssetLoader&&) = delete;

	AssetLoader() = delete;

	static bool LoadMesh(std::string meshPath, S_Mesh* mesh);
	static bool LoadModel(std::string modelPath, std::set<S_Mesh*>& outMeshSet);
	static bool LoadMaterial(std::string materialPath, Material* outMaterial);
	static bool LoadMaterialLibrary(std::string materialPath, std::set<Material*>& outMaterialSet);

protected:
	static void LoadMeshVertices(S_Mesh* mesh, S_OBJData& data);
};
#endif

