#include <vector>
#include <cstdio>

int main(){

	std::vector<int> vec;

	vec.reserve(10);

	vec.push_back(0);
	vec.push_back(0);
	vec[1]=10;
	
	printf("%d",vec[1]);
	getchar();
	return 0;
}

/*class Vertice
{
	glm::vec3 posicion;
	glm::vec3 normal;
	glm::vec2 coor_tex;
	std::vector<size_t> triangulos;
	Vertice(){
		posicion=glm::vec3(-1);
		normal = glm::vec3(0);
		coor_tex = glm::vec2(0);
	}
	Vertice(float x, float y, float z);
};*/