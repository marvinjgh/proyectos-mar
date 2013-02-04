#pragma once
#include <mod\Modelo.h>
#include <fstream>
#include <algorithm>
#include <glm\gtx\vector_angle.hpp>


enum evento{VERT_MOV,VER_TRI, ARIS_TRI, ARIS_VER, ARIS_ACT, TRI_ACT, TRI_ARI, TRI_VER};

struct accion{
	evento e;
	vector<size_t> param;
	glm::vec4 pos;
};

struct backup{
	vector<accion> estado_anterior;
};

class Off : public Modelo
{
public:
	
	Off(void);
	//destructor
	virtual ~Off(void);
	virtual void cargarModelo(char* file);
	size_t findArista(Arista );
	void centrar(void);
	void updateBuffer(Sombreado);
	void pintar(size_t n);
	bool isLoad(void);
	//dado un triangulo y un vertice determina la posicion de 
	//dicho vertice en el arreglo de vertices en el triangulo
	size_t buscarVertice(size_t t,size_t v);
	void simplificar(float);
	void undo();
	void resolver(accion);

	void recalcular(vector<size_t>);

	void actualizar(size_t t,size_t a, size_t b, backup&);

	friend ostream& operator<< (ostream& os, const Off& m){
		FOR(i,m.aristas.size()){
			os<<"aristas\n";
			os<<"id "<< i << " "<<m.aristas.at(i);
		}

		return os<<"\n";
	}

	vector<backup> colisiones;
};

