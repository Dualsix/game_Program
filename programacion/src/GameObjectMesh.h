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
#include "ctime"
#include "game.h"

class GameObjectMesh : public GameObject {
public:
	//Attributes of this class
	Mesh* mesh;
	Texture* texture;
	Texture* texture1;
	Shader* shader;
	Shader* aabb_shader;
	Vector3 color;
	GameObjectMesh* player;
	int vida_0 = 100;
	int vida_1 = 30;
	int vida_2 = 10;
	int vida_3 = 100;
	float t1, t2, t14, t15, t16, t17;
	bool primero = true;
	bool segundo = true;
	bool boss_time = true;
	std::vector<Vector3> waypoints;
	int current_waypoint = 0;
	Game* game = Game::instance;
	Vector3 pos_prota;



	//overwritten virtual functions
	void render();
	void update(float dt);
	void onCollideBullet(float power, Vector3 collision_point); //lo que pasa cuando una bala da a una nave
};

