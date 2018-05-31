#include "bulletManager.h"
#include "ctime"
#include "game.h"
#include "escudo.h"

bulletManager:: bulletManager(Mesh* theMesh, int max_bullet, Shader* theshader, Shader* theshaderenemy){
	bullet_mesh = theMesh;
	bullets_vector.resize(max_bullet);
	shader = theshader;
	shader2 = theshaderenemy;
	all_bullets_mesh = new Mesh();
};

void bulletManager::render() {
	Matrix44 m;
	Camera* cam = g_scene.camera;

	all_bullets_mesh->clear();

	bool empty = true;
	int aux, cont;
	aux = cont = 0;
	for (int i = 0; i < bullets_vector.size(); i++) {
		if (bullets_vector[i].valid) {
			m.setTranslation(bullets_vector[i].position.x, bullets_vector[i].position.y, bullets_vector[i].position.z);
			for (int j = 0; j < this->bullet_mesh->indices.size(); j++) {
				all_bullets_mesh->indices.push_back(this->bullet_mesh->indices[j] + aux);

			}
			
			for (int k = 0; k < this->bullet_mesh->vertices.size(); k++) {
				all_bullets_mesh->vertices.push_back(m*this->bullet_mesh->vertices[k]);
				cont++;
			}
			empty = false;
			aux = cont;
		}
	}
	if (!empty) {
		all_bullets_mesh->genBuffers();
	}


	if (!empty && !enemigo) {
		//reiniciar la m
		m.setScale(1.0, 1.0, 1.0);
		//create our mvp from hierarchy
		Matrix44 mvp =  cam->viewprojection_matrix;

		//enable shader and set uniforms
		shader->enable();
		float transparency = 1;
		shader->setMatrix44("u_mvp", mvp);
		shader->setUniform1("transp", transparency);
		
		//mesh->render() binds the relevant attributes, and drawing code
		all_bullets_mesh->render(GL_TRIANGLES, shader);

		//disable the shader
		shader->disable();
		//disable blending to draw text on top
		
	}

	if (!empty && enemigo) {
		//reiniciar la m
		m.setScale(1.0, 1.0, 1.0);
		//create our mvp from hierarchy
		Matrix44 mvp =  cam->viewprojection_matrix;
		
		//enable shader and set uniforms
		shader2->enable();
		shader2->setMatrix44("u_mvp", mvp);
		
		//mesh->render() binds the relevant attributes, and drawing code
		all_bullets_mesh->render(GL_TRIANGLES, shader2);
		//disable the shader
		shader2->disable();
		//disable blending to draw text on top
		
	}
	glDisable(GL_BLEND);
};

void bulletManager::update(float elapsed_time) {
	for (int j = 0; j < bullets_vector.size(); j++) {
		if (bullets_vector[j].valid) {
			bullets_vector[j].old_position = bullets_vector[j].position;
			bullets_vector[j].position.x = bullets_vector[j].position.x - bullets_vector[j].velocity.x * elapsed_time * 50;
			bullets_vector[j].position.y = bullets_vector[j].position.y - (bullets_vector[j].velocity.y /*+ 0.05*/) * elapsed_time * 50; //con gravedad
			bullets_vector[j].position.z = bullets_vector[j].position.z - bullets_vector[j].velocity.z * elapsed_time * 50;
			//calcula el tiempo de vida de la bala
			bullets_vector[j].t1 = clock();

			double time = (double(bullets_vector[j].t1 - bullets_vector[j].t0) / CLOCKS_PER_SEC);
			if (time > bullets_vector[j].ttl) {
				bullets_vector[j].valid = false;
			}
			if (bullets_vector[j].position.x > 0 && bullets_vector[j].position.z > 0) {
				bullets_vector[j].zona = 1;
			}
			if (bullets_vector[j].position.x > 0 && bullets_vector[j].position.z < 0) {
				bullets_vector[j].zona = 2;
			}
			if (bullets_vector[j].position.x < 0 && bullets_vector[j].position.z < 0) {
				bullets_vector[j].zona = 3;
			}
			if (bullets_vector[j].position.x < 0 && bullets_vector[j].position.z > 0) {
				bullets_vector[j].zona = 4;
			}
			checkCollisions(objetos);
		}
	}
};

void bulletManager::checkCollisions(std::vector<GameObject*>& Colliders) {
	//loop all bullets
	for (size_t i = 0; i < bullets_vector.size(); i++) {
		//skip currentbullet if it's dead
		if (!bullets_vector[i].valid) continue;
		//loop all colliders
		for (size_t j = 0; j < Colliders.size(); j++) {
			if (bullets_vector[i].zona != Colliders[j]->zona) continue;
			if (bullets_vector[i].owner == Colliders[j]) continue;
			if (!Colliders[j]->colision) continue; //si ya ha colisionado pasas al siguiente
			//avoid colliding bullet with its parent!
			
			if ((bullets_vector[i].owner->type == 1 || bullets_vector[i].owner->type == 2 || bullets_vector[i].owner->type == 3) && (Colliders[j]->type == 1 || Colliders[j]->type == 2 || Colliders[j]->type == 3))continue;
			
			//dynamic cast GameObject to GameObjectMesh
			GameObjectMesh* go = dynamic_cast<GameObjectMesh*>(Colliders[j]);
			//update collision model model matrix (as diferent gameobjects may use de same mesh
   			go->mesh->collision_model->setTransform(go->model.m);
			//test ray collision
			if (go->mesh->collision_model->rayCollision(bullets_vector[i].old_position.v, bullets_vector[i].position.v, false, 0, 1)) {
				
				//destroy bullet
				if (game->shield->activo == true) {
					bullets_vector[i].valid = false;
					continue;
				}
				bullets_vector[i].valid = false;
				//get collision point, in model space
				Vector3 collision_point;
				go->mesh->collision_model->getCollisionPoint(collision_point.v, true);
				
				//call collision function of Gameobject
				go->onCollideBullet(bullets_vector[i].power, collision_point );
				//skip rest of objects
				continue;
			}
		}

	}


}

void bulletManager::createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner, bool enemy) {
	enemigo = enemy;
	for (int i = 0; i < bullets_vector.size(); i++) {
		if (!bullets_vector[i].valid) {	//creo la bala solo si no es valida
			bala.position = pos;
			bala.velocity = vel;
			bala.power = power;
			bala.ttl = ttl;
			bala.owner = owner;
			bala.valid = true;
			bala.t0 = clock();
			bullets_vector[i] = bala;
			break;
		}
	}
};