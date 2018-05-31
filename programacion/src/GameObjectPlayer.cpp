#include "GameObjectPlayer.h"
#include <iostream>
#include "GameObjectMesh.h"
#include "managers.h"
#include "framework.h"
#include "bulletManager.h"


//accesses the one and only instance of Game

void GameObjectPlayer::render() {
	pos_prota = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	Camera* cam = g_scene.camera;
	Vector3 light_pos = g_scene.lights[0]->posllum;
	//create our mvp from hierarchy
	Matrix44 mvp = this->getGlobalMatrix() * cam->viewprojection_matrix;

	//create our mvp
	//Matrix44 mvp = model * g_scene.camera->viewprojection_matrix;
	this->mesh->aabb.center = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	//create normal matrix
	Matrix44 inverse_m = model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();

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

	this->mesh->renderline(aabb_shader);

	aabb_shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
};



void GameObjectPlayer::update(float dt) {
	Vector3 fn = this->model.frontVector().normalize();
	Vector3 pos = Vector3(this->model.m[12], this->model.m[13], this->model.m[14]);
	g_scene.pos_prota = pos;
	float porcentaje = 0.1;

	Vector3 up = Vector3(this->model.m[4], this->model.m[5], this->model.m[6]);
	g_scene.camera->center = pos;
	g_scene.camera->up = up.normalize();

	
	//si pulsas shift corres mas
	float speed = 1;
	if (game->keystate[SDL_SCANCODE_LSHIFT]) {
		speed = dt*30;
	}
	else {
		speed = dt*15;
	}
	
	//roll derecho
	int center_x = (int)floor(Game::instance->window_width*0.5); //conseguir centro de pantalla
	int center_y = (int)floor(Game::instance->window_height*0.5); //conseguir centro de pantalla
	SDL_WarpMouseInWindow(Game::instance->window, center_x, center_y);

	Game::instance->mouse_position.x = (float)center_x; //mantener en centro el raton
	Game::instance->mouse_position.y = (float)center_y; //mantener en centro el raton

	this->model.rotateLocal(Game::instance->mouse_delta.x*0.003f, Vector3(0, -1, 0)); //mover la pantalla con el raton
	this->model.rotateLocal(Game::instance->mouse_delta.y*0.003f, Vector3(-1, 0, 0)); //mover la pantalla con el raton

	if (game->keystate[SDL_SCANCODE_E]){
		this->model.rotateLocal(0.03, Vector3(0, 0, 1));

	}
	//roll izquierdo
	if (game->keystate[SDL_SCANCODE_Q]) {
		this->model.rotateLocal(-0.03, Vector3(0, 0, 1));

	}
	if (pos.x > 0 && pos.z > 0) {
		zona = 1;
	}
	if (pos.x > 0 && pos.z < 0) {
		zona = 2;
	}
	if (pos.x < 0 && pos.z < 0) {
		zona = 3;
	}
	if (pos.x < 0 && pos.z > 0) {
		zona = 4;
	}
	//disparo normal
	if (primero) {
		t3 = clock();
		primero = false;
	}
	if ((game->mouse_state & SDL_BUTTON_LEFT) && !game->keystate[SDL_SCANCODE_W] && !game->keystate[SDL_SCANCODE_V]) {
		t4 = clock();
		double time = (double(t4 - t3) / CLOCKS_PER_SEC);
		if (time > 0.2) {
			game->bullet_manager->createBullet(pos, fn * 5, 10, 2, this, false);
			primero = true;
		}
	}
	//disparo en rafaga
	if (!segundo) {
		t5 = clock();
		segundo = true;
		rafaga = 0;
	}
	if ((game->mouse_state & SDL_BUTTON_LEFT) && game->keystate[SDL_SCANCODE_W] && !game->keystate[SDL_SCANCODE_V]) {
		t6 = clock();
		double time2 = (double(t6 - t5) / CLOCKS_PER_SEC);
		if (time2 > 0.6) {
			if (!tercero) {
				t7 = clock();
				tercero = true;
			}
			t8 = clock();
			double time3 = (double(t8 - t7) / CLOCKS_PER_SEC);
			if (time3 > 0.2) {
				game->bullet_manager->createBullet(pos, fn * 5, 10, 2, this, false);
				tercero = false;
				rafaga++;
			}
		}
		if (rafaga >= 3) {
			segundo = false;
		}
	}
	//escudo
	if (!cuarto) {
		t9 = clock();
		cuarto = true;
	}
	if (!(game->mouse_state & SDL_BUTTON_LEFT) && game->keystate[SDL_SCANCODE_SPACE] && !game->keystate[SDL_SCANCODE_W]) {
		t10 = clock();
		double time4 = (double(t10 - t9) / CLOCKS_PER_SEC);
		if (time4 > 5) {
			game->shield->createEscudo(pos, 2, fn);
			cuarto = false;
		}
	}
	
	
	game->shield->update(pos);
	if (g_scene.boss == false) {
		if (pos.x > 1000 || pos.x < -1000) {
			if (((pos.x - fn.x * 5 * dt * 15) < 1000) && ((pos.x - fn.x * 5 * dt * 15) > -1000)) {
				this->model.traslate(-fn.x * 5 * speed, -fn.y * 5 * speed, -fn.z * 5 * speed);
			}
			else {
				this->model.traslate(0, -fn.y * 5 * speed, -fn.z * 5 * speed);
			}

		}
		else if (pos.y > 1000 || pos.y < -1000) {
			if (((pos.y - fn.y * 5 * dt * 15) < 1000) && ((pos.y - fn.y * 5 * dt * 15) > -1000)) {
				this->model.traslate(-fn.x * 5 * speed, -fn.y * 5 * speed, -fn.z * 5 * speed);
			}
			else {
				this->model.traslate(-fn.x * 5 * speed, 0, -fn.z * 5 * speed);
			}
		}
		else if (pos.z > 1000 || pos.z < -1000) {
			if (((pos.z - fn.z * 5 * dt * 15) < 1000) && ((pos.z - fn.z * 5 * dt * 15) > -1000)) {
				this->model.traslate(-fn.x * 5 * speed, -fn.y * 5 * speed, -fn.z * 5 * speed);
			}
			else {
				this->model.traslate(-fn.x * 5 * speed, -fn.y * 5 * speed, 0);
			}
		}
		else if ((pos.x < 1000 || pos.x > -1000) && (pos.y < 1000 || pos.y > -1000) && (pos.z < 1000 || pos.z > -1000)) {
			this->model.traslate(-fn.x * 5 * speed, -fn.y * 5 * speed, -fn.z * 5 * speed);
		}
	}
	else {
		this->model.traslate(-fn.x * 5 * speed, -fn.y * 5 * speed, -fn.z * 5 * speed);
	}


	Vector3 ideal_pos = Vector3(pos.x + fn.x * 50+ up.x*10, pos.y + fn.y * 50 + up.y * 10, pos.z + fn.z * 50+ up.z*10);
	ideal_pos = g_scene.camera->eye.lerp(ideal_pos, dt * 2);
	Vector3 ideal_target = Vector3(pos.x + fn.x*-50, pos.y + fn.y*-50 , pos.z + fn.z*-50);
	g_scene.camera->lookAt(ideal_pos, ideal_target, up);



};