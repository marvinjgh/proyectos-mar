#include "Vec4D.h"


Vec4D::Vec4D(void)
{
	coord[0]=0;
	coord[1]=0;
	coord[2]=0;
	coord[3]=0;
}

Vec4D::Vec4D(double x, double y, double z, double w)
{
	coord[0]=x;
	coord[1]=y;
	coord[2]=z;
	coord[3]=w;
}

Vec4D::Vec4D(const double* v)
{
	coord[0]=v[0];
	coord[1]=v[1];
	coord[2]=v[2];
	coord[3]=v[3];
	
}

Vec4D::Vec4D(const Vec4D& v)
{
	coord[0]=v.coord[0];
	coord[1]=v.coord[1];
	coord[2]=v.coord[2];
	coord[3]=v.coord[3];
	
}

Vec4D::Vec4D(const Punto4D& p)
{
	coord[0]=p.coord[0];
	coord[1]=p.coord[1];
	coord[2]=p.coord[2];
	coord[3]=p.coord[3];
	
}

double Vec4D::Magnitud(void) const
{
  return(sqrt(coord[0]*coord[0] + coord[1]*coord[1] + coord[2]*coord[2] + coord[3]*coord[3]));
}


void Vec4D::Normalizar(void)
{
  double n;

  n = Magnitud();
  coord[0] = coord[0]/n;
  coord[1] = coord[1]/n;
  coord[2] = coord[2]/n;
  coord[3] = coord[3]/n;
}

Vec4D Vec4D::operator+(const Vec4D& v)
{
	Vec4D Tmp(coord[0] + v.coord[0], coord[1] + v.coord[1],	coord[2] + v.coord[2],	coord[3] + v.coord[3]);

  return Tmp;
}

Vec4D Vec4D::operator-(const Vec4D& v)
{
	Vec4D Tmp(coord[0] - v.coord[0], coord[1] - v.coord[1],	coord[2] - v.coord[2],	coord[3] - v.coord[3]);

  return Tmp;
}

Vec4D Vec4D::operator+=(const Vec4D& v)
{
  coord[0] += v.coord[0]; 
  coord[1] += v.coord[1]; 
  coord[2] += v.coord[2];
  coord[3] += v.coord[3]; 
  return *this;
}

Vec4D Vec4D::operator-=(const Vec4D& v)
{
  coord[0] -= v.coord[0]; 
  coord[1] -= v.coord[1]; 
  coord[2] -= v.coord[2];
  coord[3] -= v.coord[3]; 
  return *this;
}

Vec4D Vec4D::operator*=(double f)
{
  coord[0] *= f; 
  coord[1] *= f; 
  coord[2] *= f;
  coord[3] *= f; 
  return *this;
}

Vec4D Vec4D::operator/(double f)
{
  Vec4D Tmp(coord[0]/f, coord[1]/f, coord[2]/f,coord[3]/f);

  return Tmp;
}

Vec4D Vec4D::operator/=(double f)
{
  coord[0] /= f; 
  coord[1] /= f; 
  coord[2] /= f;
  coord[3] /= f; 
  return *this;
}

double Vec4D::operator*(const Vec4D& v) const // Producto escalar
{
 return(coord[0]*v.coord[0] + coord[1]*v.coord[1] + coord[2]*v.coord[2] + coord[3]*v.coord[3]);
}

Vec4D Vec4D::operator^(const Vec4D& v) const // Producto Cruz (el ultimo valor es 1)
{
  Vec4D Tmp(coord[1]*v.coord[2] - v.coord[1]*coord[2], 
		coord[2]*v.coord[0] - v.coord[2]*coord[0], 
		coord[0]*v.coord[1] - v.coord[0]*coord[1], 1.f);

  return Tmp;
}

Vec4D Vec4D::operator-(void) const
{
	return Vec4D(-coord[0],-coord[1],-coord[2],-coord[3]);
}

Vec4D Vec4D::operator=(const Vec4D& v)
{
  coord[0] = v.coord[0];
  coord[1] = v.coord[1];
  coord[2] = v.coord[2];
  coord[3] = v.coord[3];

  return(*this);
}

Vec4D operator*(double f, const Vec4D& v4Vec)
{
  Vec4D v4Tmp(f*v4Vec.coord[0], f*v4Vec.coord[1], f*v4Vec.coord[2], f*v4Vec.coord[3]);

  return v4Tmp;
}

Vec4D operator*(const Vec4D& v4Vec, double f)
{
  Vec4D v4Tmp(f*v4Vec.coord[0], f*v4Vec.coord[1], f*v4Vec.coord[2], f*v4Vec.coord[3]);

  return v4Tmp;
}

ostream& operator <<(ostream& os, const Vec4D& v)
{
	return os << "Vec4D(" << v.coord[0] << ", " << v.coord[1] << ", " << v.coord[2] << ", " << v.coord[3] << ")\n";
}
