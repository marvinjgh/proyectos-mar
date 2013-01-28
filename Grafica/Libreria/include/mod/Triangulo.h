#pragma once

#include "Arista.h"

class Triangulo
{
public:
	//constructor
	Triangulo(void);
	//constructor dado 3 vertices
	Triangulo(size_t,size_t,size_t);
	//destructor
	~Triangulo(void);
	void setArista(size_t , size_t );
	size_t getArista(size_t );
	void setNormal(glm::vec3 &);
	glm::vec3 & getNormal();
	size_t getVertice(size_t);
	void setVertice(size_t, size_t);
	bool estaActivo();
	void cambiarEstado();

	friend ostream& operator<< (ostream& os, const Triangulo& p);

private:
	size_t vert[3], aris[3];
	glm::vec3 normal;
	bool activo;
};

