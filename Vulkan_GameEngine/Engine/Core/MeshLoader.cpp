#include "MeshLoader.h"
#include "Renderers/RenderObject.h"
#include "DebugLogger.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

#include <iostream>
#include <sstream>
#include <fstream>

bool MeshLoader::LoadMesh(std::string meshPath, S_Mesh* mesh)
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

           /* if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(mesh->Vertices.size());
                mesh->Vertices.push_back(vertex);
            }

            mesh->Indices.push_back(uniqueVertices[vertex]);*/
            mesh->Vertices.push_back(vertex);
            mesh->Indices.push_back(mesh->Indices.size());
        }
    }

    return true;
}

bool MeshLoader::LoadModel(std::string modelPath, std::set<S_Mesh*>& outMeshSet)
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
            data.Vertices.push_back(FVector3(x, y, z));
        }

        //Normal Data
        if (line.substr(0, 3) == "vn ")
        {
            std::stringstream vertex(line.substr(3));
            float x, y, z;
            vertex >> x >> y >> z;
            data.Normals.push_back(FVector3(x, y, z));
        }

        //Texture Data
        if (line.substr(0, 3) == "vt ")
        {
            std::stringstream vertex(line.substr(3));
            float x, y, z;
            vertex >> x >> y >> z;
            data.TextureCoords.push_back(FVector3(x, y, z));
        }

        if (line.substr(0, 2) == "f ")//TODO finish this!
        {
        }

        //New Mesh
        else if (line.substr(0, 7) == "usemtl ")
        {
            if (data.Indices.size() > 0)
            {
                currentMesh = new S_Mesh();
                outMeshSet.insert(currentMesh);
                LoadMeshVertices(currentMesh, data);
                data.Indices.clear();
                data.NormalIndicies.clear();
                data.TextureIndices.clear();
            }
            loaded = true;
        }
    }
    if(currentMesh) LoadMeshVertices(currentMesh, data);
    currentMesh = nullptr;
    return loaded;
}

bool MeshLoader::LoadMaterial(std::string materialName)
{
    return true;
}

bool MeshLoader::LoadMaterialLibrary(std::string materialPath)
{
    std::ifstream materialFile(materialPath.c_str(), std::ios::in);
    if (!materialFile)
    {
        DebugLogger::Error("Failed to open MTL file at: " + materialPath, "Core/MeshLoader.cpp", __LINE__);
        return false;
    }

    std::string line;
    bool loaded = false;
    while (std::getline(materialFile, line))
    {
        if (line.substr(0, 7) == "newmtl ") LoadMaterial(line.substr(7));
        loaded = true;
    }
    
    return loaded;
}

void MeshLoader::LoadMeshVertices(S_Mesh* mesh, S_OBJData& data)
{
    std::unordered_map<S_Vertex, uint32_t, HASH_Vertex> uniqueVertices{};

    for (unsigned int i = 0; i < data.Indices.size(); i++)
    {
        S_Vertex vertex = S_Vertex();

        vertex.Position = data.Vertices[data.Indices[i]];
        vertex.TextureCoordinates = data.TextureCoords[data.TextureIndices[i]];
        vertex.Normal = data.Normals[data.NormalIndicies[i]];

        if (uniqueVertices.count(vertex) == 0)
        {
            uniqueVertices[vertex] = static_cast<uint32_t>(mesh->Vertices.size());
            mesh->Vertices.push_back(vertex);
        }
        mesh->Indices.push_back(uniqueVertices[vertex]);
    }
}
