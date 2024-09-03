#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <filesystem>
#include "Model.h"
#include "stb_image.h"

void Model::Draw(Shader& shader)
{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
				meshes[i].Draw(shader);
		}
}

void Model::LoadModel(std::string path)
{
		Assimp::Importer import;

		aiPostProcessSteps needToFlipUv = aiPostProcessSteps::aiProcess_Triangulate;
		if (flippedUV) needToFlipUv = aiProcess_FlipUVs;

		const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | needToFlipUv);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(ProcessMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
				ProcessNode(node->mChildren[i], scene);
		}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		aiColor4D color;
		aiReturn retValue = AI_SUCCESS;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if ((retValue = aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color)) != AI_SUCCESS)
				color = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
				Vertex vertex;
				glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
				// positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				
				vertex.Location = vector;
				vertex.Color = glm::vec3(color.r, color.g, color.b);

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
						// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
						// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
						vec.x = mesh->mTextureCoords[0][i].x;
						vec.y = mesh->mTextureCoords[0][i].y;
						vertex.TextureCoords = vec;

				}
				else
						vertex.TextureCoords = glm::vec2(0.0f, 0.0f);

				vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (unsigned int j = 0; j < face.mNumIndices; j++)
						indices.push_back(face.mIndices[j]);
		}
		// process materials

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		
		return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
				aiString str;
				mat->GetTexture(type, i, &str);
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
						if (std::strcmp(textures_loaded[j].Path.data(), str.C_Str()) == 0)
						{
								textures.push_back(textures_loaded[j]);
								skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
								break;
						}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
						Texture texture;
						texture.Id = TextureFromFile((directory + "/" + std::string(str.C_Str())).c_str());
						texture.Type = typeName;
						texture.Path = str.C_Str();
						textures.push_back(texture);
						textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
				}
		}
		return textures;
}

unsigned int TextureFromFile(const char* path)
{
		std::string filename = std::string(path);
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
				GLenum format;
				if (nrComponents == 1)
						format = GL_RED;
				else if (nrComponents == 3)
						format = GL_RGB;
				else if (nrComponents == 4)
						format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
		}
		else
		{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
		}

		return textureID;
}

unsigned int LoadCubemap(std::vector<std::string> faces)
{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
				unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
						stbi_image_free(data);
				}
				else
				{
						std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
						stbi_image_free(data);
				}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
}
