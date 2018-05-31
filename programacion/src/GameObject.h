#pragma once

#include "utils.h"
#include "shader.h"
#include "camera.h"



class GameObject
{
public:
	int zona;
	//some attributes
	GameObject* parent;
	std::vector<GameObject*> children;
	std::string name;
	Matrix44 model;
	GameObject* prota;
	bool colision;//si es colisionable o no
	int type;
	//virtual methods which will be overwritten
	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	void clear();
	virtual void render();
	virtual void update(float elapsed_time);
	Matrix44 getGlobalMatrix(); //returns transform in world coordinates
};

