#pragma once
#include <iostream>
#include "utils.h"
#include <cmath>

class Colisiones {
public:

	std::vector<Vector3> puntos;
	Vector3 centro;
	

	Colisiones();
	void addBox(Vector3 box);
	void checkColision();
};