#pragma once
#include <vector>
#include <cstdio>
#include <cmath>

#define FOR(q,n) for(int q = 0; q < n; q++)

#define INF_F 10000.0f
#define INF_I 10000

struct Arista
{
	float peso;
	int origen, destino;
	Arista() : peso(INF_F), origen(-1), destino(-1) {}
	Arista(int _origen, int _destino, float _peso) : peso(_peso), origen(_origen), destino(_destino) {}
};

struct punto
{
	float x,y;
	punto() : x(-1),y(-1) {}
	punto(float _x, float _y) : x(_x),y(_y) {}
};

punto vec_dir(punto p1, punto p2){
	return punto(p2.x-p1.x,p2.y-p1.y);
}

float distancia(punto p1, punto p2){
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

std::vector<punto> puntos;