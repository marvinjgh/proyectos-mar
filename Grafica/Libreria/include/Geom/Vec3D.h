#pragma once
#include "Punto3D.h"

class Vec3D
{

public:

	float coord[3];
	
	Vec3D(void);
	Vec3D(float x, float y, float z);
	Vec3D(const float* v);
	Vec3D(const Vec3D &v);
	Vec3D(const Punto3D &v);

	float Magnitud(void) const;

	void Normalizar(void);

	Vec3D operator+ (const Vec3D& v);
	Vec3D operator- (const Vec3D& v);
	Vec3D operator/ (float f);
	Vec3D operator+= (const Vec3D& v);
	Vec3D operator-= (const Vec3D& v);
	Vec3D operator*= (float f);
	Vec3D operator/= (float f);
  
	float  operator*  (const Vec3D& v) const;   // Producto Escalar
	Vec3D operator^  (const Vec3D& v) const;   // Producto Cruz (el ultimo valor es 1)
	Vec3D operator-  (void) const;
	Vec3D operator=  (const Vec3D& v);

	friend Vec3D operator*(float f, const Vec3D& v);
	friend Vec3D operator*(const Vec3D& v, float f);

	friend ostream& operator <<(ostream& os, const Vec3D& v);
};

