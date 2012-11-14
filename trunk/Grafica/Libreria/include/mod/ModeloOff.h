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

double calcularAngulo(Vec3D a, Vec3D b);

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

	struct arista{
		int a, b, t1, t2;
	};

	ModeloOff(void);
	~ModeloOff(void);
	void cargarModelo(const char* file);
	void centrar(void);
	void updateBuffer(Sombreado x);
	void calcularNormal(triangulo* t);
	Vec3D calcularNormal(Punto3D a, Punto3D b, Punto3D c);
	void colapse();
	void agregarArista(arista* a, int t);

	size_t total;
	GLsizeiptr datasize;
	GLuint BufferObject;
	GLfloat* buffer;
	Punto3D minp,maxp;
	vector<triangulo*> faces;
	vector<vertice*> vert;
	vector<arista*> aristas;
	vector<arista*>::iterator ar;
	stack<GLuint> recover;
	

	Mat4x4 centro;


};