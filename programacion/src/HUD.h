#pragma once

#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"


class HUD {
	public:
		HUD(int sw, int sh);
		~HUD();

		Camera* cam;
		Shader* tex_shader;
		int heroe;
		int malo;
		bool normal = true;
		bool secreto = false;
		//minimap quad
		Mesh* minimap_back_mesh1;
		Mesh* minimap_back_mesh;
		Mesh* poiter_mesh;
		std::vector<Texture*> vida;
		Texture* pointer_tex;
		Texture* minimap_back_tex;

		void render();
		void update(float);
		void load_texture();
		

};