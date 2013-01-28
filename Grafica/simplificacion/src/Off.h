#pragma once
#include <mod\Modelo.h>
#include <fstream>
#include <algorithm>
#include <glm\gtx\vector_angle.hpp>

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

	size_t* actualizar(size_t t,size_t a, size_t b);
	size_t siguiente(size_t ,size_t );

	friend ostream& operator<< (ostream& os, const Off& m){
		FOR(i,m.triangulos.size()){
			os<<"id "<< i << " "<<m.triangulos.at(i);
		}

		return os<<"\n";
	}
};

