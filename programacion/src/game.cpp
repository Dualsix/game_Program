#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "managers.h"
#include "GameObject.h"
#include "GameObjectMesh.h"
#include "GameObjectEnvironment.h"
#include "GameObjectLight.h"
#include "LevelParser.h"
#include <cmath>
#include "bulletManager.h"
#include "escudo.h"
#include "bass.h"


RenderToTexture* rt = NULL;
Game* Game::instance = NULL;
float debug_camera = false;
int num = 500;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = false;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
    glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	g_scene.state = 0;
	g_scene.boss = false;


	//OpenGL flags
	
	if(g_scene.state == 0) {
		g_scene.menus = new Menu(window_width, window_height);
		g_scene.hud = new HUD(window_width, window_height);
		g_scene.hud2 = new HUD(window_width, window_height);
		g_scene.first_time2 = true;
	}


	//hide the cursor
	SDL_ShowCursor(mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_scene.state == 1) {
		if (g_scene.state == 1 && !g_scene.boss && g_scene.first_time1) {
			LevelParser::loadLevel("data/assets/level_2.xml");
			g_scene.first_time = true;
			g_scene.first_time1 = false;
			
		}
		if (g_scene.state == 1 && g_scene.boss && g_scene.first_time1) {
			LevelParser::loadLevel("data/assets/level.xml");
			g_scene.first_time = true;
			g_scene.first_time1 = false;
		}
		if (g_scene.boss == false && g_scene.first_time) {
			BASS_Free();
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/New World Symphony - 4th Movement.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			g_scene.first_time = false;
		}
		if (g_scene.boss == true && g_scene.first_time) {
			BASS_Free();
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/Deathwing Dragonlord's theme - Dragon Soul.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			g_scene.first_time = false;
		}
	//	g_scene.hud->render();
		g_scene.camera->setPerspective(70.0f, (float)window_width / (float)window_height, 0.1f, 10000.0f);
		//update view and projection matrices, and push gl matrices for fixed pipeline rendering
		g_scene.camera->set();
		g_scene.render();
		bullet_manager->render();
		bullet_manager1->render();
		shield->render();
		
	}
	else {
		int center_x = (int)floor(Game::instance->window_width*0.5); //conseguir centro de pantalla
		int center_y = (int)floor(Game::instance->window_height*0.5); //conseguir centro de pantalla
		SDL_WarpMouseInWindow(Game::instance->window, center_x, center_y);

		Game::instance->mouse_position.x = (float)center_x; //mantener en centro el raton
		Game::instance->mouse_position.y = (float)center_y; //mantener en centro el raton
		if (g_scene.first_time2) {
			BASS_Free();
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/World of Warcraft- Legion - Anduins Theme.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			g_scene.first_time2 = false;
		}
		g_scene.menus->render();
	}
	if (g_scene.pause == false) g_scene.menus->render();
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant
	
	
	//to navigate with the mouse fixed in the middle
	//para k no entre mil veces a p
	if (num < 100) {
		num++;
	}
	else if(keystate[SDL_SCANCODE_P]) {
		if (debug_camera) {
			debug_camera = false;
		}
		else {
			debug_camera = true;
		}
		num = 0;
	}
	if (keystate[SDL_SCANCODE_ESCAPE] && g_scene.pause) {
		g_scene.pause = false;
		Sleep(200);
	}
	if (!debug_camera && g_scene.state == 0 && g_scene.pause) {
		g_scene.menus->update(seconds_elapsed);
	}
	if (!debug_camera && g_scene.state == 1 && !g_scene.first_time1 && g_scene.pause) {
		g_scene.root->update(seconds_elapsed);
		bullet_manager->update(seconds_elapsed);
		bullet_manager1->update(seconds_elapsed);
		
	}
	if (debug_camera) {
		int center_x = (int)floor(window_width*0.5); //conseguir centro de pantalla
		int center_y = (int)floor(window_height*0.5); //conseguir centro de pantalla
		SDL_WarpMouseInWindow(window, center_x, center_y);

		mouse_position.x = (float)center_x; //mantener en centro el raton
		mouse_position.y = (float)center_y; //mantener en centro el raton
		//mouse input to rotate the cam
		if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			g_scene.camera->rotate(mouse_delta.x * 0.005f, Vector3(0, -1, 0));
			g_scene.camera->rotate(mouse_delta.y * 0.005f, Vector3(-1, 0, 0));
		}
		//async input to move the camera around
		if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) g_scene.camera->move(Vector3(0,0,1) * (float)speed);
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) g_scene.camera->move(Vector3(0,0,-1) * (float)speed);
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) g_scene.camera->move(Vector3(1,0,0) * (float)speed);
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) g_scene.camera->move(Vector3(-1,0,0) * (float)speed);
		if (mouse_locked)
		{
			int center_x = (int)floor(window_width*0.5f);
			int center_y = (int)floor(window_height*0.5f);
			//center_x = center_y = 50;
			SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
																	 //SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen

			this->mouse_position.x = (float)center_x;
			this->mouse_position.y = (float)center_y;
		}
	}
	if (!g_scene.pause && g_scene.state != 0) {
		if (g_scene.first_pause) {
			g_scene.menus->current_entry = 8;
			g_scene.first_pause = false;
		}
		g_scene.menus->update(seconds_elapsed);
	}
	if (!g_scene.pause && g_scene.state == 0) {
		g_scene.pause = true;
	}
    
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: ; //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	
	glViewport( 0,0, width, height );
	g_scene.camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

