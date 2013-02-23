#pragma once
#include <vector>
#include <cstdio>
#include <cmath>

#define FOR(q,n) for(int q = 0; q < n; q++)

#define INF_F 1e7
#define CERO 1e-8
#define INF_I 0x3e3e3e3e

float min_all;

bool *pun_b;

#define igual(a,b) (abs(a-b) < CERO) 
#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a>b)?a:b)

struct punto
{
	float x,y;
	int p;
	punto() : x(-1),y(-1),p(-1) {}
	punto(float _x, float _y,int _p) : x(_x),y(_y),p(_p) {}
};

std::vector<punto> puntos;

float distancia(punto p1, punto p2){
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

struct Arista
{
	float peso;
	int origen, destino;
	punto dir;
	Arista() : peso(INF_F), origen(-1), destino(-1) {}
	Arista(int _origen, int _destino) : origen(_origen), destino(_destino) 
	{
		peso = distancia(puntos[_origen],puntos[_destino]);
	}
};



double Cross(punto a, punto b,punto c){
  return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

bool on_segment(punto a, punto b, punto test){
  return (test.x>= min(a.x,b.x) && test.x<= max(a.x,b.x) &&
    test.y>=min(a.y,b.y) && test.y<=max(a.y,b.y));
}

bool intersectan(punto p1,punto p2,punto p3,punto p4){
  double d1,d2,d3,d4;
  d1 = Cross(p3,p4,p1);
  d2 = Cross(p3,p4,p2);
  d3 = Cross(p1,p2,p3);
  d4 = Cross(p1,p2,p4);
  if(((d1>0 && d2<0) || (d1<0 && d2>0)) &&
    ((d3>0 && d4<0) || (d3<0 && d4>0)))
    return true;

  if(d1==0 && on_segment(p3,p4,p1))
    return true;
  if(d2==0 && on_segment(p3,p4,p2))
    return true;    
  if(d3==0 && on_segment(p1,p2,p3))
    return true;
  if(d4==0 && on_segment(p1,p2,p4))
    return true;

  return false;
}