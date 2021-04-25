#include <glad/glad.h>
#include "model.h"
#include "shader.h"
#include "texture.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include "../engine/engine.h"

bool Model::loadModelAsset(const std::string &AssetName)
{
    Engine &engine = Engine::get_singleton();
    std::string modelPath = engine.getAssetPathByName(AssetName);
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }

    processNode(scene->mRootNode, scene);
    return true;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        loadMeshVertex(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::loadMeshVertex(aiMesh *mesh, const aiScene *scene)
{
    std::vector<ModelVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ModelVertex vertex;
        Vector3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    // m_textures.insert(std::pair<std::string, std::vector<Texture *>>("texture_diffuse", diffuseMaps));
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    // m_textures.insert(std::pair<std::string, std::vector<Texture *>>("texture_specular", specularMaps));
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    // m_textures.insert(std::pair<std::string, std::vector<Texture *>>("texture_normal", normalMaps));
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    // m_textures.insert(std::pair<std::string, std::vector<Texture *>>("texture_height", heightMaps));
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    m_mesh_vertex.push_back(MeshVertex(vertices, indices, textures, texturesSortSetName(textures)));
}

std::vector<std::string> Model::texturesSortSetName(std::vector<Texture*> textures)
{
    std::vector<std::string> texturesName;
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        std::string number;
        std::string name = textures[i]->type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream
        texturesName.push_back((name + number).c_str());
    }
    return texturesName;
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < m_textures_cache.size(); j++)
        {
            if (std::strcmp(m_textures_cache[j]->path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_textures_cache[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Engine& engine = Engine::get_singleton();
            std::string filename = std::string(str.C_Str());
            Texture* texture = new Texture();
            texture->load(engine.getAssetPathByName(filename), true);
            texture->type = typeName;
            texture->path = str.C_Str();
            textures.push_back(texture);
            m_textures_cache.push_back(texture);
        }
    }
    return textures;
}