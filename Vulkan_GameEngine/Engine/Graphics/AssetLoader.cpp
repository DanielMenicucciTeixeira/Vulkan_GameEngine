#include "AssetLoader.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

#include <iostream>
#include <sstream>
#include <fstream>

bool AssetLoader::LoadMesh(std::string meshPath, S_Mesh* mesh)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshPath.c_str()))
    {
        throw std::runtime_error(warn + err);
        return false;
    }

    std::unordered_map<S_Vertex, uint32_t, HASH_Vertex> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            S_Vertex vertex = S_Vertex();

            vertex.Position =
            {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.TextureCoordinates =
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                0
            };

            vertex.Normal =
            {
            attrib.normals[3 * index.vertex_index + 0],
            attrib.normals[3 * index.vertex_index + 1],
            attrib.normals[3 * index.vertex_index + 2]
            };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(mesh->Vertices.size());
                mesh->Vertices.push_back(vertex);
            }

            mesh->Indices.push_back(uniqueVertices[vertex]);
        }
    }

    return true;
}

bool AssetLoader::LoadModel(std::string modelPath, std::set<S_Mesh*>& outMeshSet)
{
    std::ifstream modelFile(modelPath.c_str(), std::ios::in);
    if (!modelFile)
    {
        DebugLogger::Error("Failed to open model file at: " + modelPath, "Core/MeshLoader.cpp", __LINE__);
        return false;
    }

    std::string line;
    bool loaded = false;
    S_Mesh* currentMesh = nullptr;
    S_OBJData data;

    while (std::getline(modelFile, line))
    {
        //Vertex Data
        if (line.substr(0, 2) == "v ")
        {
            std::stringstream vertex(line.substr(2));
            float x, y, z;
            vertex >> x >> y >> z;
            data.Position.push_back(FVector3(x, y, z));
        }

        //Normal Data
        else if (line.substr(0, 3) == "vn ")
        {
            std::stringstream normal(line.substr(3));
            float x, y, z;
            normal >> x >> y >> z;
            data.Normals.push_back(FVector3(x, y, z));
        }

        //Texture Data
        else if (line.substr(0, 3) == "vt ")
        {
            std::stringstream texCoords(line.substr(3));
            float x, y, z;
            texCoords >> x >> y >> z;
            data.TextureCoords.push_back(FVector3(x, y, z));
        }

        //Face Data
        else if (line.substr(0, 2) == "f ")//TODO finish this!
        {
            int value = 1;
            int vectorID = 0;
            for (int i = 0; i < line.size() - 2; i += (1 + static_cast<int>(log10(value))))//Ajustment of how many chars to skip after reading a value
            {
                std::stringstream indices(line.substr(2 + i));
                if (indices.str().substr(0, 1) == "/" || indices.str().substr(0, 1) == " ")
                {
                    value = 1;
                    continue;
                }

                indices >> value;
                switch (vectorID % 3)
                {
                case 2:
                    data.NormalIndicies.push_back(value);
                    vectorID++;
                    break;
                case 1:
                    data.TextureIndices.push_back(value);
                    vectorID++;
                    break;
                case 0:
                    data.PositionIndices.push_back(value);
                    vectorID++;
                }
            }
        }

        //MeshName
        else if (line.substr(0, 9) == "# object ")
        {
            if (data.PositionIndices.size() > 0)
            {
                LoadMeshVertices(currentMesh, data);
                data.PositionIndices.clear();
                data.NormalIndicies.clear();
                data.TextureIndices.clear();
            }
            loaded = true;
            currentMesh = new S_Mesh();
            outMeshSet.insert(currentMesh);
            currentMesh->Name = line.substr(9);
        }

        else if (line.substr(0, 10) == "# 44 faces")
        {
            int breakPoint = 0;
        }
    }

    modelFile.close();

    if(currentMesh) LoadMeshVertices(currentMesh, data);
    currentMesh = nullptr;
    return loaded;
}

bool AssetLoader::LoadMaterial(std::string materialPath, S_Material* outMaterial)
{
    std::ifstream materialFile(materialPath.c_str(), std::ios::in);
    if (!materialFile)
    {
        DebugLogger::Error("Failed to open model file at: " + materialPath, "Core/MeshLoader.cpp", __LINE__);
        return false;
    }

    std::string line;
    bool loaded = false;
    while (std::getline(materialFile, line))
    {
    }
    return loaded;
}

bool AssetLoader::LoadMaterialLibrary(std::string materialPath, std::set<S_Material*>& outMaterialSet)
{
    std::ifstream materialFile(materialPath.c_str(), std::ios::in);
    if (!materialFile)
    {
        DebugLogger::Error("Failed to open model file at: " + materialPath, "Core/MeshLoader.cpp", __LINE__);
        return false;
    }

    std::string line;
    bool loaded = false;
    S_Material* material = nullptr;
    while (std::getline(materialFile, line))
    {
        //MeshName
        if (line.substr(0, 7) == "newmtl ")
        {
            loaded = true;
            material = new S_Material();
            outMaterialSet.insert(material);
            material->Name = "M_" + line.substr(7);
        }

        //Ambient Data
        else if (line.substr(0, 4) == "\tKa ")
        {
            std::stringstream values(line.substr(4));
            float x, y, z;
            values >> x >> y >> z;
            material->Data[0] = FVector4(x, y, z, 0);
        }

        //Diffuse Data
        else if (line.substr(0, 4) == "\tKd ")
        {
            std::stringstream values(line.substr(4));
            float x, y, z;
            values >> x >> y >> z;
            material->Data[1] = FVector4(x, y, z, 0);
        }

        //Specular Data
        else if (line.substr(0, 4) == "\tKs ")
        {
            std::stringstream values(line.substr(4));
            float x, y, z;
            values >> x >> y >> z;
            material->Data[2] = FVector4(x, y, z, 0);
        }

        //Shininess Data
        else if (line.substr(0, 4) == "\tNs ")
        {
            std::stringstream values(line.substr(4));
            values >> material->Data[3][0];
        }

        //Opacity Data
        else if (line.substr(0, 3) == "\td ")
        {
            std::stringstream values(line.substr(3));
            values >> material->Data[3][1];
        }
    }
    return loaded;
}

void AssetLoader::LoadMeshVertices(S_Mesh* mesh, S_OBJData& data)
{
    std::unordered_map<S_Vertex, uint32_t, HASH_Vertex> uniqueVertices{};

    for (unsigned int i = 0; i < data.PositionIndices.size(); i++)
    {
        S_Vertex vertex = S_Vertex();

        vertex.Position = data.Position[data.PositionIndices[i] - 1];
        vertex.TextureCoordinates = data.TextureCoords[data.TextureIndices[i] - 1];
        vertex.Normal = data.Normals[data.NormalIndicies[i] - 1];

        if (uniqueVertices.count(vertex) == 0)
        {
            uniqueVertices[vertex] = static_cast<uint32_t>(mesh->Vertices.size());
            mesh->Vertices.push_back(vertex);
        }
        mesh->Indices.push_back(uniqueVertices[vertex]);
    }
}
