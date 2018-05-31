#pragma once

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "GameObjectMesh.h"
#include "camera.h"
#include <cmath>
#include <iostream>
#include "game.h"
#include "ctime"
#include "escudo.h"



class GameObjectPlayer : public GameObjectMesh {
public:
	//Attributes of this class
	
	Shader* aabb_shader;
	Vector3 color;
	bool invencible = false;
	Game* game = Game::instance;
	float t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;//para el calculo de los segundos de disparo
	bool primero = true; //primero, segundo y tercero es para el control de tiempo de disparo
	bool segundo = false;
	bool tercero = false;
	bool cuarto = false;
	bool quinto = false;
	int rafaga = 0;
	
	//overwritten virtual functions
	void render();
	void update(float dt);
	
};
