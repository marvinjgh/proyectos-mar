#pragma once
#include "Punto3D.h"

struct Vertice{
	Punto3D p;
	Punto3D n; 
};

struct Celda{
	Punto3D p[8];
	Punto3D g[8];
	float val[8];
};
struct Triangulo{
	size_t p[3];	/* Vertices */
};

class Array_bin
{
public:
	Array_bin(void);
	~Array_bin(void);
	void insertar(long long id, size_t v);
	void limpiar(void);
	size_t busqueda(long long id);

private:
	size_t busqueda_bin(long long dato, size_t star, size_t end);
	struct data
	{
		long long id;
		size_t v;
		data(long long i, size_t v):id(i),v(v){}
		data():id(0),v(NULL){}
	};

	size_t max;
	size_t size;
	data* array;
};

