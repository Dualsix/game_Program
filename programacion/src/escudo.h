#pragma once
#include <string>
#include "ResourceManager.h"
#include "GameObjectMesh.h"




struct Shield {
	bool valid = false; //si esta vivo o no
	Vector3 position; //posicion actual
	float ttl; //tiempo de vida
	float t11, t12; //para calcular el tiempo de vida
	Vector3 vel; //posicion actual
};

class escudo : public GameObjectMesh {
	public: 
		bool first_time;
		bool activo;
		Mesh* escudo_mesh; //la mesh del escudo
		Shader* shader_escudo;//el shader que usamos para dibujar el escudo
		Shield barrera;
		escudo(Mesh* escudo_mesh, Shader* shader); //constructor
		void render(); //dibujamos el escudo
		void createEscudo(Vector3 pos, float ttl, Vector3 direccion); // crea el escudo
		void update(Vector3 position); //actualizamos la posicion
		void escalar(float x, float y, float z);
};
