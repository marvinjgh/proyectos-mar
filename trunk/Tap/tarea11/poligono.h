#pragma once

#include <vector>

struct punto
{
	float x,y;
};

class Poligono
{
public:
	Poligono(int);
	~Poligono();
	void agregarPunto(float, float);
private:


	std::vector <punto> puntos;
};

Poligono::Poligono(int n)
{
	puntos.reserve(n);
}

Poligono::~Poligono()
{
}

void Poligono::agregarPunto(float x, float y){
	punto p;
	p.x=x;
	p.y=y;
	puntos.push_back(p);
}