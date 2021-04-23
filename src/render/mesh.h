// #ifndef _MESH_H_
// #define _MESH_H_

// #include <string>
// #include <vector>
// #include "../common/math.h"

// class Shader;
// class Texture;

// struct Vertex
// {
//     // position
//     Vector3 Position;
//     // normal
//     Vector3 Normal;
//     // texCoords
//     Vector2 TexCoords;
//     // tangent
//     Vector3 Tangent;
//     // bitangent
//     Vector3 Bitangent;
// };

// class Mesh
// {
// public:
//     std::vector<Vertex> vertices;
//     std::vector<unsigned int> indices;
//     std::vector<Texture> textures;
//     unsigned int VAO;

//     Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
//     void Draw(Shader* &shader);

// private:
//     unsigned int VBO, EBO;

//     void setupMesh();
// };
// #endif