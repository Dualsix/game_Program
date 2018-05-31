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

class GameObjectLight : public GameObject {
public:
	Vector3 posllum;

	void render();
	void update(float elapsed_time);
};
