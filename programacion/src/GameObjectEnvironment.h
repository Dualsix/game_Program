#pragma once

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "GameObject.h"
#include "camera.h"
#include"ResourceManager.h"
#include <cmath>
#include <iostream>


class GameObjectEnvironment : public GameObject {
public:
	
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	

	float sx, sy, sz;
	void escalar(float x, float y, float z);
	void render();
	void update(float elapsed_time);
};

