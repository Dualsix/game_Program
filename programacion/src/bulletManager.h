#pragma once
#include "framework.h"
#include "GameObject.h"
#include "mesh.h"
#include "ResourceManager.h"
#include "managers.h"
#include <string>
#include "GameObjectMesh.h"
#include<stdlib.h>
#include<time.h>



struct Bullet {
	bool valid; //si esta vivo o no
	Vector3 position; //posicion actual
	Vector3 old_position; //posicion antes del update
	Vector3 velocity; //velocidad en vector del proyectil
	float ttl; //tiempo de vida
	float power; //daño que causa la bala
	GameObject* owner; //objeto	que dispara la bala
	float t0, t1; //para calcular el tiempo de vida
	int zona;
};

class bulletManager {
	public:
		Mesh* bullet_mesh; //la mesh de la bala
		std::vector<Bullet> bullets_vector; //vector de las balas que puede lanzar el objeto
		Shader* shader;//el shader que usamos para dibujar la bala
		Shader* shader2; //el shader para dibujar la bala de los enemigos
		bool can_fire = true; //opcional - para controlar la rafaga de disparo
		Bullet bala;
		bool enemigo;
		std::vector<GameObject*> objetos; //todos los objetos colisionables con las balas
		Game* game = Game::instance;
		bulletManager(Mesh* theMesh, int max_bullet, Shader* theshader, Shader* theshaderenemy); //constructor

		Mesh* all_bullets_mesh;

		void render(); //dibujamos todas las ballas del array
		void update(float elapsed_time); //actualizamos la posicion
		void checkCollisions(std::vector<GameObject*>& Colliders);
		void createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner, bool enemy); //crea la bala que se dispara
};