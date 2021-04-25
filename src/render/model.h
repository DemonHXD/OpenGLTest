#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../common/math.h"

class Texture;
class Model
{

public:
    struct ModelVertex
    {
        // position
        Vector3 Position;
        // normal
        Vector3 Normal;
        // texCoords
        Vector2 TexCoords;
        // tangent
        Vector3 Tangent;
        // bitangent
        Vector3 Bitangent;
    };
    struct MeshVertex
    {
        std::vector<ModelVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture *> textures;
        std::vector<std::string> texturesName;
        
        MeshVertex(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices,
            std::vector<Texture *> textures, std::vector<std::string> texturesName)
            : vertices(vertices),
            indices(indices),
            textures(textures),
            texturesName(texturesName)
            {}
    };

public:
    bool loadModelAsset(const std::string &path);

    std::vector<MeshVertex> getMeshVertex() const
    {
        return m_mesh_vertex;
    }

private:
    void processNode(aiNode *node, const aiScene *scene);
    void loadMeshVertex(aiMesh *mesh, const aiScene *scene);
    std::vector<std::string> texturesSortSetName(std::vector<Texture *> textures);
    std::vector<Texture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    std::vector<MeshVertex> m_mesh_vertex;
    std::vector<Texture *> m_textures_cache;
};
#endif