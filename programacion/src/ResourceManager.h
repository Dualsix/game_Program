#pragma once



#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"

#include <unordered_map>

class ResourceManager
{
private:
	//maps for storing handles to pointers to asset objects
	std::unordered_map<std::string, Mesh*>m_meshes;
	std::unordered_map<std::string, Texture*>m_textures;
	std::unordered_map<std::string, Shader*> m_shaders;

	//more lists (audio etc.) will go here later

public:
	//unused constructor and destructor
	/*ResourceManager();
	~ResourceManager();

	//use these functions to start and stop the ResourceManager
	void startUp();
	void shutDown();*/

	//load assets into memory
	void loadMesh(const std::string, const std::string);
	void loadTexture(const std::string, const std::string);
	void loadShader(const std::string, const std::string, const std::string);

	//unoad assets out of memory
	void unloadMesh(const std::string);
	void unloadTexture(const std::string);
	void unloadShader(const std::string);

	//getters for individual assets
	Mesh* getMesh(const std::string);
	Texture* getTexture(const std::string);
	Shader* getShader(const std::string);

};
