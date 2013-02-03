#pragma once
#include "..\Geom\Defs.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

class Vertice
{
public:

	Vertice(void);
	Vertice(float*);
	Vertice(float , float , float );
	~Vertice(void);
	glm::vec4& getPosicion();
	glm::vec3& getNormal();
	size_t getTriangulo(size_t);
	vector<size_t> getTriangulos();
	void setPosicion(float , float , float );
	void setPosicion(glm::vec4 &);
	void setNormal(glm::vec3 &);
	size_t getSizetrian();
	void agregarTriangulo(size_t);
	void setTrian(vector<size_t>);
private:
	glm::vec4 posicion;
	glm::vec3 normal;

	vector<size_t> trian;
};

