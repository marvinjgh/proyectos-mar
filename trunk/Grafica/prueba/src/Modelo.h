#pragma once
#include "glsl\Shader.h"
#include <fstream>
#include <vector>
#include <cfloat>
#include <FreeImage.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

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

	size_t total, datasize;
	GLuint BufferObject;
	vector<GLuint> texturas;
	glm::vec3 minp,maxp;
	vector<triangulo*> faces;
	glm::mat4 centro;
};

void min(glm::vec3*,float*);
void max(glm::vec3*,float*);