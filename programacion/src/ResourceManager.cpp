#include"ResourceManager.h"





//load a mesh into memory, and associate it with a handle
void ResourceManager::loadMesh(const std::string handle, const std::string path) {
	
	Mesh *mesh = new Mesh();
	mesh->loadASE(path.c_str());
	m_meshes[handle] = mesh;

}

void ResourceManager::loadTexture(const std::string handle, const std::string path) {

	Texture* texture = new Texture();
	texture->load(path.c_str());
	m_textures[handle] = texture;
}

void ResourceManager::loadShader(const std::string handle, const std::string path, const std::string path2) {

	Shader* shader = new Shader();
	if (!shader->load(path, path2))
	{
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}
	m_shaders[handle] = shader;
}

//unload a mesh from memory, given its handle
void ResourceManager::unloadMesh(const std::string handle) {

	//try to find mesh in map
	std::unordered_map<std::string, Mesh*> ::iterator itr = m_meshes.find(handle);
	//check it's there
	if (itr != m_meshes.end()) {
		//found it - delete it and erased it
		delete itr->second;
		m_meshes.erase(itr);
	}
	else {
		//else print error and quit
		std::cout << "could not find asset with handle" << handle << std::endl;
		exit(0);
	}
}

void ResourceManager::unloadTexture(const std::string handle) {

	//try to find mesh in map
	std::unordered_map<std::string, Texture*> ::iterator itr = m_textures.find(handle);
	//check it's there
	if (itr != m_textures.end()) {
		//found it - delete it and erased it
		delete itr->second;
		m_textures.erase(itr);
	}
	else {
		//else print error and quit
		std::cout << "could not find asset with handle" << handle << std::endl;
		exit(0);
	}
}

void ResourceManager::unloadShader(const std::string handle) {

	//try to find mesh in map
	std::unordered_map<std::string, Shader*> ::iterator itr = m_shaders.find(handle);
	//check it's there
	if (itr != m_shaders.end()) {
		//found it - delete it and erased it
		delete itr->second;
		m_shaders.erase(itr);
	}
	else {
		//else print error and quit
		std::cout << "could not find asset with handle" << handle << std::endl;
		exit(0);
	}
}

//Return a pointer to a mesh object, given a handle
Mesh* ResourceManager::getMesh(const std::string name) {

	//...you should probably add some error checking here...!
	return m_meshes.at(name);
}

Texture* ResourceManager::getTexture(const std::string name) {

	//...you should probably add some error checking here...!
	return m_textures.at(name);
}

Shader* ResourceManager::getShader(const std::string name) {

	//...you should probably add some error checking here...!
	return m_shaders.at(name);
}
