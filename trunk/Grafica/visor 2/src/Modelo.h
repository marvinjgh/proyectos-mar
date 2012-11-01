#pragma once
#include "glsl\Shader.h"
#include <Geom\Vec3D.h>
#include <Geom\Vec4D.h>
#include <Geom\mat4x4.h>
#include <fstream>
#include <vector>
#include <cfloat>
#include <FreeImage.h>

class Modelo
{
public:
	Modelo(void);
	~Modelo(void);
	void cargarModelo(const char* file);
	void centrar(void);
	struct triangulo{
		size_t ver[3], nor[3], tex[3];
		float s;
	};
	void añadirFace(size_t* v,size_t* n,size_t* t,float s);
	void mtlFileRead(char *fn);
	bool loadTexture (char *path ,GLenum minFilter, GLenum magFilter, GLenum wrapMode);

	size_t total, datasize;
	GLuint BufferObject;
	vector<GLuint> texturas;
	Punto3D minp,maxp;
	vector<triangulo*> faces;
	Mat4x4 centro;
};

