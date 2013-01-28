#pragma once

#include "Vertice.h"

class Arista
{
public:
	Arista(void);
	Arista(size_t ,size_t );
	~Arista(void);
	bool operator==(Arista );
	void setVert_a(size_t );
	void setVert_b(size_t );
	void setTrian_1(size_t );
	void setTrian_2(size_t );
	size_t getVert_a();
	size_t getVert_b();
	size_t getTrian_1();
	size_t getTrian_2();
	bool estaActivo();
	void cambiarEstado();
	friend ostream& operator<< (ostream& os, const Arista& a);

private:

	bool activo;
	size_t vert_a, vert_b, trian_1, trian_2;
};

