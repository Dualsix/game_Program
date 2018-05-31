#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "GameObject.h"
#include "camera.h"
#include"ResourceManager.h"
#include <cmath>
#include <iostream>
#include "GameObjectEnvironment.h"
#include "managers.h"



void GameObjectEnvironment::render() {
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	//draw
	model.setScale(sx, sy, sz);
	model.traslate(g_scene.camera->eye.x, g_scene.camera->eye.y, g_scene.camera->eye.z);
	//model.scale(2, 2, 2);
	Camera* cam = g_scene.camera;
	//Vector3 light_pos = g_scene.lights[0]->posllum;
	//create our mvp from hierarchy
	Matrix44 mvp = this->getGlobalMatrix() * cam->viewprojection_matrix;

	//create our mvp
	//Matrix44 mvp = model * g_scene.camera->viewprojection_matrix;



	//enable shader and set uniforms
	shader->enable();
	shader->setMatrix44("u_mvp", mvp);

	//pasamos texturas
	shader->setTexture("u_texture_diffuse", texture);
	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);
	//disable the shader
	shader->disable();


	//*** Drawing code ends here ***


	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}
void GameObjectEnvironment::update(float elapsed_time) {

}

void GameObjectEnvironment::escalar(float x, float y, float z) {
	sx = x; sy = y; sz = z;
	model.scale(x, y, z);
}

 
