#pragma once

#include "..\glsl\Shader.h"
#include "Triangulo.h"
//std
#include <vector>
//glm
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <algorithm>

class Modelo
{
public:
	Modelo(void);
	virtual ~Modelo(void);

	virtual void cargarModelo(char* file);
	virtual void centrar(void);
	virtual void updateBuffer(Sombreado );
	void min(glm::vec3& ,float* );
	void max(glm::vec3& ,float* );

	size_t nTri, nVer, nAri, dimx, dimy, dimz;
	
	vector<GLuint> texturas;
	
	glm::vec3 minp,maxp;
	
	vector<Triangulo> triangulos;
	vector<Vertice> vertices;
	vector<Arista> aristas;

	glm::mat4 centro;
	
	//VBO
	GLuint BufferObject;
	GLsizeiptr datasize;
	
};

