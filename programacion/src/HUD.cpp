#include "HUD.h"
#include "managers.h"

HUD::HUD(int sw, int sh){
	cam = new Camera();
	cam->setOrthographic(0, sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();

	//create mesh and resize verts
	minimap_back_mesh = new Mesh();
	minimap_back_mesh->vertices.resize(4);
	minimap_back_mesh->uvs.resize(4);

	minimap_back_mesh->vertices[0] = Vector3(0, 0, 1); minimap_back_mesh->uvs[0] = Vector2(0, 0);
	minimap_back_mesh->vertices[1] = Vector3(sw, 0, 1); minimap_back_mesh->uvs[1] = Vector2(1, 0);
	minimap_back_mesh->vertices[2] = Vector3(sw, sh , 1); minimap_back_mesh->uvs[2] = Vector2(1, 1);
	minimap_back_mesh->vertices[3] = Vector3(0, sh , 1); minimap_back_mesh->uvs[3] = Vector2(0, 1);
	

	//create indices 
	minimap_back_mesh->indices.resize(6);
	minimap_back_mesh->indices[0] = 0; minimap_back_mesh->indices[1] = 1; minimap_back_mesh->indices[2] = 2;
	minimap_back_mesh->indices[3] = 0; minimap_back_mesh->indices[4] = 2; minimap_back_mesh->indices[5] = 3;

	//send to GPU
	minimap_back_mesh->genBuffers();


	tex_shader = new Shader();
	tex_shader->load("data/shaders/simple_HUD.vert", "data/shaders/simple_HUD.frag");
	load_texture();
	heroe = 10;
	malo = vida.size()-1;

}

void HUD::load_texture() {
	vida.clear();

	Texture* aux = new Texture();
	aux->load("data/assets/imagenes/0vida.tga");
	vida.push_back(aux);

	Texture* aux1 = new Texture();
	aux1->load("data/assets/imagenes/10vida.tga");
	vida.push_back(aux1);

	Texture* aux2 = new Texture();
	aux2->load("data/assets/imagenes/20vida.tga");
	vida.push_back(aux2);

	Texture* aux3 = new Texture();
	aux3->load("data/assets/imagenes/30vida.tga");
	vida.push_back(aux3);

	Texture* aux4 = new Texture();
	aux4->load("data/assets/imagenes/40vida.tga");
	vida.push_back(aux4);

	Texture* aux5 = new Texture();
	aux5->load("data/assets/imagenes/50vida.tga");
	vida.push_back(aux5);

	Texture* aux6 = new Texture();
	aux6->load("data/assets/imagenes/60vida.tga");
	vida.push_back(aux6);

	Texture* aux7 = new Texture();
	aux7->load("data/assets/imagenes/70vida.tga");
	vida.push_back(aux7);

	Texture* aux8 = new Texture();
	aux8->load("data/assets/imagenes/80vida.tga");
	vida.push_back(aux8);

	Texture* aux9 = new Texture();
	aux9->load("data/assets/imagenes/90vida.tga");
	vida.push_back(aux9);

	Texture* aux10 = new Texture();
	aux10->load("data/assets/imagenes/100vida.tga");
	vida.push_back(aux10);

	Texture* aux11 = new Texture();
	aux11->load("data/assets/imagenes/0boss.tga");
	vida.push_back(aux11);

	Texture* aux12 = new Texture();
	aux12->load("data/assets/imagenes/10boss.tga");
	vida.push_back(aux12);

	Texture* aux13 = new Texture();
	aux13->load("data/assets/imagenes/20boss.tga");
	vida.push_back(aux13);

	Texture* aux14 = new Texture();
	aux14->load("data/assets/imagenes/30boss.tga");
	vida.push_back(aux14);

	Texture* aux15 = new Texture();
	aux15->load("data/assets/imagenes/40boss.tga");
	vida.push_back(aux15);

	Texture* aux16 = new Texture();
	aux16->load("data/assets/imagenes/50boss.tga");
	vida.push_back(aux16);

	Texture* aux17 = new Texture();
	aux17->load("data/assets/imagenes/60boss.tga");
	vida.push_back(aux17);

	Texture* aux18 = new Texture();
	aux18->load("data/assets/imagenes/70boss.tga");
	vida.push_back(aux18);

	Texture* aux19 = new Texture();
	aux19->load("data/assets/imagenes/80boss.tga");
	vida.push_back(aux19);

	Texture* aux20 = new Texture();
	aux20->load("data/assets/imagenes/90boss.tga");
	vida.push_back(aux20);

	Texture* aux21 = new Texture();
	aux21->load("data/assets/imagenes/100boss.tga");
	vida.push_back(aux21);
}

void HUD::render() {
	//float transp = 0.2;
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//set uniforms and draw texture quad 
	tex_shader->enable();
	Texture* aux;
	if (secreto) {
		aux = vida[malo];
	}
	else {
		aux = vida[heroe];
	}
	tex_shader->setTexture("u_texture_diffuse", aux);
	
	//tex_shader->setUniform1("transp", transp);
	tex_shader->setMatrix44("u_mvp", cam->viewprojection_matrix);
	minimap_back_mesh->render(GL_TRIANGLES, tex_shader);
	//poiter_mesh->render(GL_TRIANGLES, tex_shader);
	tex_shader->disable();

	//re-enable depth test (por si las moscas)
	glEnable(GL_DEPTH_TEST);


}
