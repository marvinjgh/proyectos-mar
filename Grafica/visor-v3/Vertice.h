#pragma once
#include <glm\glm.hpp>
#include <vector>

class Vertice
{
public:
	glm::vec3 posicion;
	glm::vec3 normal;
	glm::vec2 coor_tex;
	std::vector<size_t> triangulos;
	Vertice(void)
	{
		posicion=glm::vec3(-1);
		normal = glm::vec3(0);
		coor_tex = glm::vec2(0);
	}

	~Vertice(void)
	{
	}
	
	Vertice(float x, float y, float z){
		posicion = glm::vec3(x,y,z);
	}

	void agregarTrian(size_t t){
		triangulos.push_back(t);
	}

};
