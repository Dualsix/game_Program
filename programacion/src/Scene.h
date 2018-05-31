#pragma once

#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "HUD.h"
#include "GameObject.h"
#include "GameObjectLight.h"

class Scene {
public:
	Scene();
	~Scene();

	//scene graph starts from here
	GameObject* root;

	//the camera used to render the scene
	int count = 0;
	Camera* camera;
	Menu* menus;
	int state;
	bool boss;
	bool pause = true;
	bool first_pause = true;
	bool first_time = true;
	bool first_time1 = true;
	bool first_time2 = true;
	HUD* hud;
	HUD* hud2;
	Vector3 pos_prota;
	//a list of old lights - could be within graph or not
	std::vector<GameObjectLight*> lights;
	//standard render and update functions
	void render();
	void update(float dt);
	void remove_root();

};