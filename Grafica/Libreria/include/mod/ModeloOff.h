#pragma once
#include "..\glsl\Shader.h"
#include "..\Geom\Vec3D.h"
#include "..\Geom\mat4x4.h"
#include <fstream>
#include <vector>
#include <stack>
#include <cfloat>

double calcularAngulo(Vec3D a, Vec3D b);

class ModeloOff
{
public:
	struct triangulo{
		int vertices[3];
		int aristas[3];
		Vec3D normal;
		bool activo;
	};

	struct vertice{
		Punto3D posicion;
		Vec3D normal;
		vector<int> trians;
		vector<int> aris;
		bool activo;
	};

	struct arista{
		int a, b, t1, t2;
		bool activo;
	};

	struct backup{
		vertice a;
		arista* b;
	};

	ModeloOff(void);
	~ModeloOff(void);
	void cargarModelo(const char* file);
	void centrar(void);
	void updateBuffer(Sombreado x);
	void calcularNormal(triangulo* t);
	Vec3D calcularNormal(Punto3D a, Punto3D b, Punto3D c);
	void colapse(double offSetAngle);
	int agregarArista(arista* a, int t);
	int buscarArista(int a, int b);
	
	size_t tra, vea;
	size_t total;
	GLsizeiptr datasize;
	GLuint BufferObject;
	GLfloat* buffer;
	Punto3D minp,maxp;
	
	vector<triangulo*> triangulos;
	vector<vertice*> vertices;
	vector<arista*> aristas;
	
	Mat4x4 centro;


};