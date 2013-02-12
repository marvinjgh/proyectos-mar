#pragma once

#include "def.h"



class Poligono
{
public:
	Poligono(int);
	~Poligono();
	void agregarPunto(int);
	void pintar();
	int interseccion(punto o,punto dir);
private:
	std::vector <int> pun;
};

Poligono::Poligono(int n)
{
	pun.reserve(n);
}

Poligono::~Poligono()
{
}

void Poligono::agregarPunto(int p){
	pun.push_back(p);
}

void Poligono::pintar(){
	printf("Poligono tiene %d puntos ",puntos.size());
	FOR(i,puntos.size()){
		printf("punto (%f,%f) ",puntos.at(pun.at(i)).x,puntos.at(pun.at(i)).y);
	}
}

int interseccion(punto o,punto dir);