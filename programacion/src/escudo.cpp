#include "escudo.h"
#include "ctime"
#include "managers.h"
#include "bass.h"


escudo::escudo(Mesh* mesh, Shader* shaders) {
	escudo_mesh = mesh;
	shader_escudo = shaders;
	colision = false;
}

void escudo::createEscudo(Vector3 pos, float ttl, Vector3 direccion) {
	if (!barrera.valid) {
		barrera.t11 = clock();
		colision = true;
		activo = true;
		barrera.valid = true;
		barrera.position = pos;
		barrera.ttl = ttl;
		barrera.vel = direccion;
		g_scene.first_time = true;
		if (first_time) {
			//Inicializamos el bass (id, muestras por segundo...)
			BASS_Init(1, 44100, 0, 0, NULL);
			//cargamos un sample (memoria, filename, offset, length, max, flag)
			//use BASS_SAMPLE_LOOP in the last param to have a looped sound
			HSAMPLE hsample = BASS_SampleLoad(false, "data/assets/Musica/escudo.mp3", 0, 0, 3, 0);
			//Load sample into a channel - this could be a class property
			HCHANNEL musicChannel = BASS_SampleGetChannel(hsample, false);
			//play the channel
			BASS_ChannelPlay(musicChannel, true);
			g_scene.first_time = false;
		}
	}
}

void escudo::render() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Matrix44 m;
	Camera* cam = g_scene.camera;
	if (barrera.valid) {
		float transparency = 0.2;
		m.scale(0.15, 0.15, 0.15);
		m.traslate(barrera.position.x, barrera.position.y, barrera.position.z);
		//create our mvp from hierarchy
		Matrix44 mvp = m * cam->viewprojection_matrix;
		//enable shader and set uniforms
		shader_escudo->enable();
		shader_escudo->setUniform1("transp", transparency);
		shader_escudo->setMatrix44("u_mvp", mvp);
		this->escudo_mesh->genBuffers();
		//mesh->render() binds the relevant attributes, and drawing code
		this->escudo_mesh->render(GL_TRIANGLES, shader_escudo);
		//disable the shader
		shader_escudo->disable();
		//disable blending to draw text on top
		glDisable(GL_BLEND);
	}
}

void escudo::update(Vector3 position) {
	barrera.position = position;
	if (barrera.position.x > 0 && barrera.position.z > 0) {
		zona = 1;
	}
	if (barrera.position.x > 0 && barrera.position.z < 0) {
		zona = 2;
	}
	if (barrera.position.x < 0 && barrera.position.z < 0) {
		zona = 3;
	}
	if (barrera.position.x < 0 && barrera.position.z > 0) {
		zona = 4;
	}
	barrera.t12 = clock();
	double time = (double(barrera.t12 - barrera.t11) / CLOCKS_PER_SEC);
	if (time > barrera.ttl) {
		barrera.valid = false;
		colision = false;
		activo = false;
	}
}

void escudo::escalar(float x, float y, float z) {
	this->model.scale(x, y, z);
}