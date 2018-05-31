#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "managers.h"
#include "GameObject.h"
#include "GameObjectLight.h"
#include "Scene.h"
#include "GameObjectMesh.h"


Scene::Scene() {
	this->root = new GameObject();	
}

Scene::~Scene() {
	this->root->clear();
	delete this->root;
}

void Scene::remove_root() {
	for (int i = 0; i < this->root->children.size(); i++) {
		this->root->removeChild(this->root->children[i]);
	}
}

void Scene::render() {
	this->root->render();
	

	if (boss) {
		hud2->normal = true;
		hud2->secreto = true;
		hud2->render();
	}
	hud->normal = true;
	hud->secreto = false;
	hud->render();
}

void Scene::update(float dt) {
	this->root->update(dt);
}

