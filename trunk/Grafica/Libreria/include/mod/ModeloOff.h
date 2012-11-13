#pragma once
#include "..\glsl\Shader.h"
#include "..\Geom\Vec3D.h"
#include "..\Geom\mat4x4.h"
#include <fstream>
#include <vector>
#include <stack>
#include <cfloat>
#include <omp.h>

enum Sombreado 
{ FLAT , GOURAUD };

class ModeloOff
{
public:
	struct triangulo{
		GLuint vertices[3];
		Vec3D normal;
		bool activo;
	};

	struct vertice{
		Punto3D posicion;
		Vec3D normal;
		vector<int> trians;
		bool activo;
	};

	ModeloOff(void);
	~ModeloOff(void);
	void cargarModelo(const char* file);
	void centrar(void);
	void updateBuffer(Sombreado x);
	void calcularNormal(triangulo* t);
	Vec3D calcularNormal(Punto3D a, Punto3D b, Punto3D c);
	void colapse();

	size_t total;
	GLsizeiptr datasize;
	GLuint BufferObject;
	GLfloat* buffer;
	Punto3D minp,maxp;
	vector<triangulo*> faces;
	vector<vertice*> vert;
	stack<GLuint> recover;
	

	Mat4x4 centro;


};