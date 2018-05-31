#pragma once

#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "GameObject.h"
#include "camera.h"
#include <cmath>
#include <iostream>
#include "GameObjectMesh.h"
#include "managers.h"
#include "bulletManager.h"


	
	//overwritten virtual functions
void GameObjectMesh::render() {
	Camera* cam = g_scene.camera;
	Vector3 light_pos = g_scene.lights[0]->posllum;
	//create our mvp from hierarchy
	Matrix44 mvp = this->getGlobalMatrix() * cam->viewprojection_matrix;
	
	//create our mvp
	//Matrix44 mvp = model * g_scene.camera->viewprojection_matrix;

	//create normal matrix
	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();
	this->mesh->aabb.center = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	
	//enable shader and set uniforms
	shader->enable();
	
	shader->setMatrix44("u_mvp", mvp);
	shader->setMatrix44("u_normal_matrix", normal_m);

	shader->setUniform3("u_camera_pos", g_scene.camera->eye);

	shader->setUniform3("u_light", g_scene.lights[0]->posllum);
	//pasamos texturas
	shader->setTexture("u_texture_diffuse", texture);
	shader->setTexture("u_texture_specular", texture1);
	
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);
	
	//disable the shader
	shader->disable();


	aabb_shader->enable();
	aabb_shader->setMatrix44("u_mvp", mvp);
	
	this->mesh->renderAABB(aabb_shader);

	aabb_shader->disable();
	//disable blending to draw text on top
	glDisable(GL_BLEND);

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
};

void GameObjectMesh::onCollideBullet(float power, Vector3 collision_point) {
	if (type == 2) {
		vida_2 = vida_2 - 10;
		if (vida_2 == 0) {
			this->parent->removeChild(this);
			this->colision = false;
			g_scene.count++;
		}
	}
	if (type == 1) {
		vida_1 = vida_1 - 10;
		if (vida_1 == 0) {
			this->parent->removeChild(this);
			this->colision = false;
			g_scene.count++;
		}
	}
	if (g_scene.count == 4) {
		g_scene.remove_root();
		g_scene.menus->current_entry = 7;
		g_scene.state = 0;

	}
	if (type == 3) {
		vida_3 = vida_3 - 10;
		g_scene.hud2->malo--;
		if (vida_3 == 0) {
			this->parent->removeChild(this);
			this->colision = false;
			g_scene.menus->current_entry = 7;
			g_scene.state = 0;
		}
	}
	
	if (type == 0) {

		if (g_scene.boss == true) {
			vida_0 = vida_0 - 20;
			g_scene.hud->heroe = g_scene.hud->heroe -2;
		}
		else {
			vida_0 = vida_0 - 10;
			g_scene.hud->heroe--;
		}
		if (vida_0 == 0) {
			this->colision = false;
			g_scene.menus->current_entry = 6;
			g_scene.state = 0;
			
		}
	}

}



void GameObjectMesh::update(float dt) {

	Matrix44 global_model = this->getGlobalMatrix();
	//vector de enfrente
	Vector3 f = global_model.frontVector();
	//vector de enfrente normalizado
	Vector3 fn = global_model.frontVector().normalize();
	//guardar posicion original
	Vector3 pos_ini(global_model.m[12], global_model.m[13], global_model.m[14]);

	//guardar distanca hacia waypoint[current_waypoint]
	float distance_way = (waypoints[current_waypoint] - pos_ini).length();
	
	if (type == 1 || type == 2) {
	

		//distancia hasta el target
		Vector3 to_target = waypoints[current_waypoint] - pos_ini;
		//ROTATE
		//buscamos cuanto hemos de girar para ir al punto
		float angle = acos(to_target.dot(fn));

		Vector3 rot_axis = to_target.cross(fn);
		Matrix44 inv = global_model;
		inv.inverse();
		rot_axis = inv.rotateVector(rot_axis);
		//rotamos la nave suavemente

		this->model.rotateLocal(-0.005, rot_axis);
		this->model.traslate(-fn.x*0.3, -fn.y*0.3, -fn.z*0.3);
		global_model = this->getGlobalMatrix();
	}
	else {
		
		Vector3 to_target = g_scene.pos_prota - pos_ini;
		float angle = acos(to_target.dot(fn));
		Vector3 rot_axis = to_target.cross(fn);
		Matrix44 inv = global_model;
		inv.inverse();
		rot_axis = inv.rotateVector(rot_axis);
		this->model.rotateLocal(-0.005, rot_axis);
		//this->model.traslate(-fn.x*0.3, -fn.y*0.3, -fn.z*0.3);
		global_model = this->getGlobalMatrix();
		if (boss_time) {
			t16 = clock();
			boss_time = false;
		}
		t17 = clock();
		double time = (double(t17 - t16) / CLOCKS_PER_SEC);
		if (time > 2) {
			current_waypoint++;
			if (current_waypoint >= waypoints.size()) {
				current_waypoint = 0;
			}
			boss_time = true;
		}

	}

		Vector3 pos_act = Vector3(global_model.m[12], global_model.m[13], global_model.m[14]);
		//movemos la caja con la nave

		//la distancia que nos hemos movido
		float distance_move = (pos_ini - pos_act).length();
		//si superamos la distancia de waypoint cambiamos el waypoint

	
	if (distance_move >= distance_way) {
		current_waypoint++;

		if (current_waypoint >= waypoints.size()) {
			current_waypoint = 0;
		}
	}



	if (pos_act.x > 0 && pos_act.z > 0) {
		zona = 1;
	}
	if (pos_act.x > 0 && pos_act.z < 0) {
	zona = 2;
	}
	if (pos_act.x < 0 && pos_act.z < 0) {
		zona = 3;
	}
	if (pos_act.x < 0 && pos_act.z > 0) {
		zona = 4;
	}
	if (primero) {
		t1 = clock();
		primero = false;
	}
	
	
	t2 = clock();
	double time = (double(t2 - t1) / CLOCKS_PER_SEC);
	if (time > 0.4) {
		if (type == 1 || type == 2) {
			game->bullet_manager1->createBullet(pos_act, fn * 5, 10, 2, this, true);
		}
		else {
			game->bullet_manager1->createBullet(pos_act, fn * 5, 20, 2, this, true);
		}
		primero = true;
	}
	
};

