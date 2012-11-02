#pragma once
#include "..\Geom\Defs.h"
#include "..\Geom\Punto4D.h"
#include "..\Geom\Vec3D.h"
#include <fstream>
#include <string>
#include <cfloat>

class ModeloOff
{
public:
	struct Vector_off{
		Punto3D posicion;
		Vec3D normal;
	} *vertices;
	
	struct Triangulo_off
	{
		GLuint vertices[3];
		Vec3D normal;
	} *triangulos;
	
	ModeloOff(void);
	~ModeloOff(void);
	void cargarOff(const char* file);
	void calcularNormal(GLuint t);

	GLuint BufferObject, index;
	GLuint nVert, nTrian;
	Punto3D minp,maxp;
};

