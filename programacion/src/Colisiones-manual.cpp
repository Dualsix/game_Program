#include "Colisiones-manual.h"
#include "managers.h"

Colisiones::Colisiones() {
	for (int i = 0; i < 8; i++) {
		puntos.push_back(Vector3(0, 0, 0));
	}
}

void Colisiones::addBox(Vector3 box) {
	float max_x = puntos[0].x;
	float max_y = puntos[0].y;
	float max_z = puntos[0].z;
	float min_x = puntos[7].x;
	float min_y = puntos[7].y;
	float min_z = puntos[7].z;

	if (box.x > max_x) {
		max_x = box.x;
	}
	if (box.x < min_x) {
		min_x = box.x;
	}

	if (box.y > max_y) {
		max_y = box.y;
	}
	if (box.y < min_y) {
		min_y = box.y;
	}

	if (box.z > max_z) {
		max_z = box.z;
	}
	if (box.z < min_z) {
		min_z = box.z;
	}
	puntos[0] = Vector3(max_x, max_y, max_z);
	puntos[1] = Vector3(min_x, max_y, max_z);
	puntos[2] = Vector3(max_x, min_y, max_z);
	puntos[3] = Vector3(min_x, min_y, max_z);
	puntos[4] = Vector3(max_x, max_y, min_z);
	puntos[5] = Vector3(min_x, max_y, min_z);
	puntos[6] = Vector3(max_x, min_y, min_z);
	puntos[7] = Vector3(min_x, min_y, min_z);

}

void Colisiones::checkColision() {
	//for (int i = 1; i < g_scene.root->children.size(); i++) {
	//	for (int j = 0; j < 8; j++) {
	//		Vector3 vertex = Vector3(g_scene.root->children[i]->mesh->puntos);
	//		// miramos arriba-derecha-fuera
	//		if ((vertex.x < puntos[0].x) && (vertex.y < puntos[0].y) && (vertex.z < puntos[0].z)) {

	//		}
	//		//miramos arriba-izquierda-fuera
	//		else if ((vertex.x > puntos[1].x) && (vertex.y < puntos[1].y) && (vertex.z < puntos[1].z)) {

	//		}
	//		//miramos abajo-derecha-fuera
	//		else if ((vertex.x < puntos[2].x) && (vertex.y > puntos[2].y) && (vertex.z < puntos[2].z)) {

	//		}
	//		//miramos abajo-izquierda-fuera
	//		else if ((vertex.x > puntos[3].x) && (vertex.y > puntos[3].y) && (vertex.z < puntos[3].z)) {

	//		}
	//		//miramos arriba-derecha-dentro
	//		else if ((vertex.x < puntos[4].x) && (vertex.y < puntos[4].y) && (vertex.z > puntos[4].z)) {

	//		}
	//		//miramos arriba-izquierda-dentro
	//		else if ((vertex.x > puntos[5].x) && (vertex.y < puntos[5].y) && (vertex.z > puntos[5].z)) {

	//		}
	//		//miramos abajo-derecha-dentro
	//		else if ((vertex.x < puntos[6].x) && (vertex.y > puntos[6].y) && (vertex.z > puntos[6].z)) {

	//		}
	//		//miramos abajo-izquierda-dentro
	//		else if ((vertex.x > puntos[7].x) && (vertex.y > puntos[7].y) && (vertex.z > puntos[7].z)) {

	//		}
	//	}
	//}
}

