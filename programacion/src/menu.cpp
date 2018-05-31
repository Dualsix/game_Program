#include "menu.h"
#include "shader.h"
#include "bass.h"
#include "game.h"
#include "managers.h"
#include <thread>


Menu::Menu(int sw, int sh) {

	cam = new Camera();
	cam->setOrthographic(0, sw, 0, sh, 0.1, 2);
	cam->updateViewMatrix();
	//create mesh and resize verts and uvs
	mesh = new Mesh();
	mesh->vertices.resize(4);
	mesh->uvs.resize(4);

	//fill verts and uvs
	mesh->vertices[0] = Vector3(0, 0, 1); mesh->uvs[0] = Vector2(0, 0);
	mesh->vertices[1] = Vector3(sw, 0, 1); mesh->uvs[1] = Vector2(1, 0);
	mesh->vertices[2] = Vector3(sw, sh, 1); mesh->uvs[2] = Vector2(1, 1);
	mesh->vertices[3] = Vector3(0, sh, 1); mesh->uvs[3] = Vector2(0, 1);

	//create indices
	mesh->indices.resize(6);
	mesh->indices[0] = 0; mesh->indices[1] = 1; mesh->indices[2] = 2;
	mesh->indices[3] = 0; mesh->indices[4] = 2; mesh->indices[5] = 3;

	//send to GPU
	mesh->genBuffers();
	current_entry = 0;
	shader_menu = new Shader();
	if (!shader_menu->load("data/shaders/simple_menu.vert", "data/shaders/simple_menu.frag")){
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}
	s = e = c = r = e2 = t = false;
	secret = false;
	//cargar texturas
	this->loadTex();
}

Menu::~Menu() {

}

void Menu::render() {
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	this->shader_menu->enable();
	this->shader_menu->setMatrix44("u_mvp", this->cam->viewprojection_matrix);
	this->shader_menu->setTexture("u_texture_diffuse", this->entries[current_entry]);

	//mesh->render() binds the relevant attributes, and drawing code
	this->mesh->render(GL_TRIANGLES, this->shader_menu);
	//disable the shader
	this->shader_menu->disable();
	//disable blending to draw text on top
	glDisable(GL_BLEND);
}

void Menu::loadTex() {
	entries.clear();

	Texture* aux = new Texture();
	aux->load("data/assets/imagenes/menu1.tga");
	entries.push_back(aux);

	Texture* aux1 = new Texture();
	aux1->load("data/assets/imagenes/menu2.tga");
	entries.push_back(aux1);

	Texture* aux2 = new Texture();
	aux2->load("data/assets/imagenes/menu3.tga");
	entries.push_back(aux2);

	Texture* aux3 = new Texture();
	aux3->load("data/assets/imagenes/INSTRUCTION.tga");
	entries.push_back(aux3);

	Texture* aux4 = new Texture();
	aux4->load("data/assets/imagenes/CARGANDO1.tga");
	entries.push_back(aux4);

	Texture* aux5 = new Texture();
	aux5->load("data/assets/imagenes/lvl21.tga");
	entries.push_back(aux5);

	Texture* aux6 = new Texture();
	aux6->load("data/assets/imagenes/gameOver.tga");
	entries.push_back(aux6);
	
	Texture* aux7 = new Texture();
	aux7->load("data/assets/imagenes/win.tga");
	entries.push_back(aux7);

	Texture* aux8 = new Texture();
	aux8->load("data/assets/imagenes/scape1.tga");
	entries.push_back(aux8);

	Texture* aux9 = new Texture();
	aux9->load("data/assets/imagenes/scape2.tga");
	entries.push_back(aux9);


}

void Menu::menu_carga() {
	for (int i = 4; i < 12; i++) {
		current_entry = i;
		render();
		if (i == 12) i = 4;
	}
}

void Menu::update(float) {
	Game* game = Game::instance;
	if (game->keystate[SDL_SCANCODE_RETURN] && (current_entry == 8)) {
		//Inicializamos el bass (id, muestras por segundo...)
		BASS_Init(1, 44100, 0, 0, NULL);
		//cargamos un sample (memoria, filename, offset, length, max, flag)
		//use BASS_SAMPLE_LOOP in the last param to have a looped sound
		HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_SELECT_8.mp3", 0, 0, 3, 0);
		//Load sample into a channel - this could be a class property
		HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
		//play the channel
		BASS_ChannelPlay(musicChannel, true);
		g_scene.pause = true;
		Sleep(200);
	}

	if (game->keystate[SDL_SCANCODE_RETURN] && (current_entry == 9)) {
		//Inicializamos el bass (id, muestras por segundo...)
		BASS_Init(1, 44100, 0, 0, NULL);
		//cargamos un sample (memoria, filename, offset, length, max, flag)
		//use BASS_SAMPLE_LOOP in the last param to have a looped sound
		HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_SELECT_8.mp3", 0, 0, 3, 0);
		//Load sample into a channel - this could be a class property
		HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
		//play the channel
		BASS_ChannelPlay(musicChannel, true);
		exit(0);
	}

	if ((game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) && current_entry == 9) {
		if (current_entry == 9) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/error.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			Sleep(200);	
		}
	}

	

	if ((game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) && current_entry == 8) {
		if (current_entry == 8) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/error.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			Sleep(200);

		}
	}

	if ((game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) && current_entry == 8) {
		if (current_entry < 9) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_WRONG_4.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			Sleep(200);
			current_entry++;

		}
	}

	if ((game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) && current_entry == 9) {
		if (current_entry == 9) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_WRONG_4.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			Sleep(200);
			current_entry--;

		}
	}

	if (current_entry == 6 || current_entry == 7) {
		Sleep(5000);
		exit(0);
	}

	if (current_entry == 4 || current_entry == 5) {
		g_scene.state = 1;
		if (secret) {
			g_scene.boss = true;
		}
	}
	if ((game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) && current_entry < 3) {
		if (current_entry == 0) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/error.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			Sleep(200);

		}
	}


	if ((game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) && current_entry < 3) {
		if (current_entry < 3) {
			if (current_entry == 2) {
				//Inicializamos el bass (id, muestras por segundo...)
				BASS_Init(1, 44100, 0, 0, NULL);
				//cargamos un sample (memoria, filename, offset, length, max, flag)
				//use BASS_SAMPLE_LOOP in the last param to have a looped sound
				HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/error.mp3", 0, 0, 3, 0);
				//Load sample into a channel - this could be a class property
				HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
				//play the channel
				BASS_ChannelPlay(musicChannel, true);
				Sleep(200);
				s = true;

			}
		}
	}

	
	if ((game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) && current_entry < 3) {
		if (current_entry > 0) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_WRONG_4.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			Sleep(200);
			current_entry--;

		}
	}

	if ((game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) && current_entry < 3) {
			if (current_entry < 2) {
				//Inicializamos el bass (id, muestras por segundo...)
				BASS_Init(1, 44100, 0, 0, NULL);
				//cargamos un sample (memoria, filename, offset, length, max, flag)
				//use BASS_SAMPLE_LOOP in the last param to have a looped sound
				HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_WRONG_4.mp3", 0, 0, 3, 0);
				//Load sample into a channel - this could be a class property
				HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
				//play the channel
				BASS_ChannelPlay(musicChannel, true);
				Sleep(200);
				current_entry++;
				s = true;
		}
	}
	if (game->keystate[SDL_SCANCODE_E]){
		e = true;
	}
	if (game->keystate[SDL_SCANCODE_C]) {
		c = true;
	}
	if (game->keystate[SDL_SCANCODE_R]) {
		r = true;
	}
	if (game->keystate[SDL_SCANCODE_E]) {
		e2 = true;
	}
	if (game->keystate[SDL_SCANCODE_T]) {
		t = true;
	}
	if (s && e && c && r && e && t) {
		secret = true;
	}
	if (game->keystate[SDL_SCANCODE_RETURN] && (current_entry == 0)) {
		//Inicializamos el bass (id, muestras por segundo...)
		BASS_Init(1, 44100, 0, 0, NULL);
		//cargamos un sample (memoria, filename, offset, length, max, flag)
		//use BASS_SAMPLE_LOOP in the last param to have a looped sound
		HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_SELECT_8.mp3", 0, 0, 3, 0);
		//Load sample into a channel - this could be a class property
		HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
		//play the channel
		BASS_ChannelPlay(musicChannel, true);
		if (!secret) {
			current_entry = 4;
		}
		else current_entry = 5;
	}
	if (game->keystate[SDL_SCANCODE_RETURN] && (current_entry == 2)) {
		//Inicializamos el bass (id, muestras por segundo...)
		BASS_Init(1, 44100, 0, 0, NULL);
		//cargamos un sample (memoria, filename, offset, length, max, flag)
		//use BASS_SAMPLE_LOOP in the last param to have a looped sound
		HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_SELECT_8.mp3", 0, 0, 3, 0);
		//Load sample into a channel - this could be a class property
		HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
		//play the channel
		BASS_ChannelPlay(musicChannel, true);
		exit(0);
	}
	if (game->keystate[SDL_SCANCODE_RETURN] && (current_entry == 3)) {
		//Inicializamos el bass (id, muestras por segundo...)
		BASS_Init(1, 44100, 0, 0, NULL);
		//cargamos un sample (memoria, filename, offset, length, max, flag)
		//use BASS_SAMPLE_LOOP in the last param to have a looped sound
		HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_SELECT_8.mp3", 0, 0, 3, 0);
		//Load sample into a channel - this could be a class property
		HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
		//play the channel
		BASS_ChannelPlay(musicChannel, true);
		current_entry = 0;
		Sleep(200);
	}
	if (game->keystate[SDL_SCANCODE_RETURN] && (current_entry == 1)) {
		//Inicializamos el bass (id, muestras por segundo...)
		BASS_Init(1, 44100, 0, 0, NULL);
		//cargamos un sample (memoria, filename, offset, length, max, flag)
		//use BASS_SAMPLE_LOOP in the last param to have a looped sound
		HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/SD_MENU_SELECT_8.mp3", 0, 0, 3, 0);
		//Load sample into a channel - this could be a class property
		HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
		//play the channel
		BASS_ChannelPlay(musicChannel, true);
		current_entry = 3;
		Sleep(200);
	}
}

