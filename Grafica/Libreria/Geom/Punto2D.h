#pragma once
#include "Punto.h"

class Punto2D: public Punto
{
private:
	
public:
	/*coordenada */
	float coord[2];
	/*Contructor base*/
	Punto2D(void);
	/*Contructor por valor*/
	Punto2D(float x, float y);
	/*Contructor por referencia*/
	Punto2D(const Punto2D& p);
	/*Destructor base*/
	~Punto2D(void);

	float distancia(Punto2D p);

	Punto2D operator+ (const Punto2D& p);
	Punto2D operator- (const Punto2D& p);
	Punto2D operator* (const Punto2D& p);

	Punto2D operator/ (float valor);

	Punto2D operator=  (const Punto2D& p);
	
	Punto2D operator+= (const Punto2D& p);
	Punto2D operator-= (const Punto2D& p);
	Punto2D operator*= (float f);
	Punto2D operator/= (float f);
	bool operator== (const Punto2D& p);
	
	friend Punto2D operator* (float valor, const Punto2D& p);
	friend Punto2D operator* (const Punto2D& p, float valor);

	friend ostream& operator<< (ostream& os, const Punto2D& p);
};

