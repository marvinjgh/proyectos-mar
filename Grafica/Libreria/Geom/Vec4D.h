#pragma once
#include "Punto4D.h"

class Vec4D
{

public:

	float coord[4];
	
	Vec4D(void);
	Vec4D(float x, float y, float z, float w);
	Vec4D(const float* v);
	Vec4D(const Vec4D &v);
	Vec4D(const Punto4D &v);

	float Magnitud(void) const;

	void Normalizar(void);

	Vec4D operator+ (const Vec4D& v);
	Vec4D operator- (const Vec4D& v);
	Vec4D operator/ (float f);
	Vec4D operator+= (const Vec4D& v);
	Vec4D operator-= (const Vec4D& v);
	Vec4D operator*= (float f);
	Vec4D operator/= (float f);
  
	float  operator*  (const Vec4D& v) const;   // Producto Escalar
	Vec4D operator^  (const Vec4D& v) const;   // Producto Cruz (el ultimo valor es 1)
	Vec4D operator-  (void) const;
	Vec4D operator=  (const Vec4D& v);

	friend Vec4D operator*(float f, const Vec4D& v);
	friend Vec4D operator*(const Vec4D& v, float f);

	friend ostream& operator <<(ostream& os, const Vec4D& v);
};

