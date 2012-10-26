#pragma once
#include "Punto3D.h"
class Punto4D: public Punto
{
public:
	/*coordenada */
	float coord[4];
	/*Contructor base*/
	Punto4D(void);
	/*Contructor por valor*/
	Punto4D(float x, float y, float z, float w);
	/*Contructor por referencia*/
	Punto4D(const Punto4D& p);
	/*Destructor base*/
	~Punto4D(void);

	float distancia(Punto4D p);

	Punto4D operator+ (const Punto4D& p);
	Punto4D operator- (const Punto4D& p);
	Punto4D operator* (const Punto4D& p);
	Punto4D operator/ (float valor);
	Punto4D operator=  (const Punto4D& p);
	void operator+= (const Punto4D& p);
	void operator-= (const Punto4D& p);
	void operator*= (float f);
	void operator/= (float f);
	bool operator== (const Punto4D& p);
	
	friend Punto4D operator* (float valor, const Punto4D& p);
	friend Punto4D operator* (const Punto4D& p, float valor);
};