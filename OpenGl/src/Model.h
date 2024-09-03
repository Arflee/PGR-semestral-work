#pragma once
#include "Mesh.h"
#include <map>

unsigned int TextureFromFile(const char* path);
unsigned int LoadCubemap(std::vector<std::string> faces);

class Model
{
public:
    Model() = default;
    Model(const char* path, bool flippedUV) : flippedUV(flippedUV)
    {
        LoadModel(path);
    }
    void Draw(Shader& shader);

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    bool flippedUV;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};