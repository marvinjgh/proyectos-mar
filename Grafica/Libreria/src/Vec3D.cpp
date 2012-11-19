#include "..\include\Geom\Vec3D.h"


Vec3D::Vec3D(void)
{
	coord[0]=0;
	coord[1]=0;
	coord[2]=0;
}

Vec3D::Vec3D(float x, float y, float z)
{
	coord[0]=x;
	coord[1]=y;
	coord[2]=z;
}

Vec3D::Vec3D(const float* v)
{
	coord[0]=v[0];
	coord[1]=v[1];
	coord[2]=v[2];

}

Vec3D::Vec3D(const Vec3D& v)
{
	coord[0]=v.coord[0];
	coord[1]=v.coord[1];
	coord[2]=v.coord[2];

}

Vec3D::Vec3D(const Punto3D& p)
{
	coord[0]=p.coord[0];
	coord[1]=p.coord[1];
	coord[2]=p.coord[2];

}

Vec3D::Vec3D(const Punto3D &p0,const Punto3D &p1){
	coord[0] = p1.coord[0]-p0.coord[0];
	coord[1] = p1.coord[1]-p0.coord[1];
	coord[2] = p1.coord[2]-p0.coord[2];
}

float Vec3D::Magnitud(void) const
{
	return(sqrt(coord[0]*coord[0] + coord[1]*coord[1] + coord[2]*coord[2]));
}


void Vec3D::Normalizar(void)
{
	float n;

	n = Magnitud();
	if (n<=0.0) return;
	coord[0] = coord[0]/n;
	coord[1] = coord[1]/n;
	coord[2] = coord[2]/n;
}

Vec3D Vec3D::operator+(const Vec3D& v)
{
	Vec3D Tmp(coord[0] + v.coord[0], coord[1] + v.coord[1],	coord[2] + v.coord[2]);

	return Tmp;
}

Vec3D Vec3D::operator-(const Vec3D& v)
{
	Vec3D Tmp(coord[0] - v.coord[0], coord[1] - v.coord[1],	coord[2] - v.coord[2]);

	return Tmp;
}

Vec3D Vec3D::operator+=(const Vec3D& v)
{
	coord[0] += v.coord[0]; 
	coord[1] += v.coord[1]; 
	coord[2] += v.coord[2];
	return *this;
}

Vec3D Vec3D::operator-=(const Vec3D& v)
{
	coord[0] -= v.coord[0]; 
	coord[1] -= v.coord[1]; 
	coord[2] -= v.coord[2];
	return *this;
}

Vec3D Vec3D::operator*=(float f)
{
	coord[0] *= f; 
	coord[1] *= f; 
	coord[2] *= f;
	return *this;
}

Vec3D Vec3D::operator/(float f)
{
	Vec3D Tmp(coord[0]/f, coord[1]/f, coord[2]/f);

	return Tmp;
}

Vec3D Vec3D::operator/=(float f)
{
	coord[0] /= f; 
	coord[1] /= f; 
	coord[2] /= f;
	return *this;
}

float Vec3D::operator*(const Vec3D& v) const // Producto escalar
{
	return(coord[0]*v.coord[0] + coord[1]*v.coord[1] + coord[2]*v.coord[2] );
}

Vec3D Vec3D::operator^(const Vec3D& v) const // Producto Cruz (el ultimo valor es 1)
{
	Vec3D Tmp(coord[1]*v.coord[2] - v.coord[1]*coord[2], 
		coord[2]*v.coord[0] - v.coord[2]*coord[0], 
		coord[0]*v.coord[1] - v.coord[0]*coord[1]);

	return Tmp;
}

Vec3D Vec3D::operator-(void) const
{
	return Vec3D(-coord[0],-coord[1],-coord[2]);
}

Vec3D Vec3D::operator=(const Vec3D& v)
{
	coord[0] = v.coord[0];
	coord[1] = v.coord[1];
	coord[2] = v.coord[2];
	return(*this);
}

Vec3D operator*(float f, const Vec3D& v4Vec)
{
	Vec3D v3Tmp(f*v4Vec.coord[0], f*v4Vec.coord[1], f*v4Vec.coord[2]);

	return v3Tmp;
}

Vec3D operator*(const Vec3D& v4Vec, float f)
{
	Vec3D v3Tmp(f*v4Vec.coord[0], f*v4Vec.coord[1], f*v4Vec.coord[2]);

	return v3Tmp;
}

ostream& operator <<(ostream& os, const Vec3D& v)
{
	return os << "Vec3D(" << v.coord[0] << ", " << v.coord[1] << ", " << v.coord[2] << ")\n";
}
