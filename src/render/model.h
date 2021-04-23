#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../common/math.h"
#include "render_object.h"

class Texture;
class Model
{
public:
    bool loadModelAsset(const std::string &path);

    std::vector<RenderObject::ModelVertex> getModelVertex() const
    {
        return m_vertices;
    }

    std::vector<unsigned int> getModelIndices() const
    {
        return m_indices;
    }

    std::vector<Texture *> getModelTextures() const
    {
        return m_textures;
    }

    std::vector<std::string> getModelTexturesName() const
    {
        return m_textures_name;
    }

private:
    void processNode(aiNode *node, const aiScene *scene);
    void loadMeshVertex(aiMesh *mesh, const aiScene *scene);
    void texturesSortSetName();
    std::vector<Texture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    // std::string m_directory;
    std::vector<RenderObject::ModelVertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture *> m_textures_cache;
    std::vector<Texture *> m_textures;
    std::vector<std::string> m_textures_name;
};
#endif