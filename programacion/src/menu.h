#pragma once
#include "mesh.h"
#include "texture.h"
#include "Camera.h"


class Menu {
	public:
		Mesh* mesh;
		Camera* cam;
		std::vector<Texture*> entries;
		int current_entry;
		Shader* shader_menu;
		bool secret = false;
		bool s, e, c, r, e2, t;

		Menu(int sw, int sh);
		~Menu();
		void render();
		void update(float);
		void loadTex();
		void menu_carga();
};