#pragma once

#include "def.h"

class Poligono
{
public:
	Poligono(int);
	~Poligono();
	void agregarPunto(int);
	void pintar();
	bool interseccion(punto o,punto d);
	punto interseccion_p(punto o,punto d);
	void crearAristas();
private:
	std::vector <int> pun;
	std::vector <Arista> ari;
};

Poligono::Poligono(int n)
{
	pun.reserve(n);
}

Poligono::~Poligono()
{
}

void Poligono::agregarPunto(int p){
	pun.push_back(p);
}

void Poligono::pintar(){
	printf("Poligono tiene %d puntos ",puntos.size());
	FOR(i,puntos.size()){
		printf("punto (%f,%f) ",puntos.at(pun.at(i)).x,puntos.at(pun.at(i)).y);
	}
}

bool Poligono::interseccion(punto o,punto d){
	int cont;punto out(INF_F,INF_F,INF_I),a,b;
	float dis1,dis2;bool b1;

	FOR(i,pun.size()){
		a=puntos.at(ari.at(i).origen);
		b=puntos.at(ari.at(i).destino);
		if ((o.x == a.x && o.y==a.y) || (o.x == b.x && o.y==b.y)){
			b1=false;
		}else{
			b1=intersectan(o,d,a,b);
		}
		if (b1){
			return true;
		}
	}
	return false;
}

punto Poligono::interseccion_p(punto o,punto d){
	int cont=0;punto out(INF_F,INF_F,0),a,b;
	float dis1,dis2;bool bo,b2=false;

	FOR(i,pun.size()){
		a=puntos.at(pun.at(i));
		if (o.x==a.x && o.y==a.y){
			b2=true;
			break;
		}
	}

	FOR(i,pun.size()){
		a=puntos.at(ari.at(i).origen);
		b=puntos.at(ari.at(i).destino);
		bo=intersectan(o,d,a,b);
		if (bo){
			if ((d.x == a.x && d.y==a.y) || (d.x == b.x && d.y==b.y)){
				out.x=d.x;
				out.y=d.y;
				if (b2){
					if ((o.x == a.x && o.y==a.y) || (o.x == b.x && o.y==b.y)){
						out.x=d.x;
						out.y=d.y;
						//printf("%f %f\n",out.x,out.y);
						return out;
					}else{
						out.x=INF_F;
						out.y=INF_F;
						//return out;
					}
				}
				//return out;
			}else{
				out.x=-INF_F;
				out.y=-INF_F;
				//cont++;
				out.p++;
				/*if (o.p != d.p){
				out.x=INF_F+1;
				out.y=INF_F+1;
				}*/
				//printf("2\n");
				if (!b2){
					out.x=-INF_F+1;
					out.y=-INF_F+1;
					return out;
				}else{
					if ((o.x == a.x && o.y==a.y) || (o.x == b.x && o.y==b.y)){
						out.x=-INF_F-1;
						out.y=-INF_F-1;
						//out.p=cont;
					}
				}
			}
		}
	}
	return out;
}

void Poligono::crearAristas(){
	int n = pun.size();
	FOR(i,n){
		ari.push_back(Arista(pun.at(i),pun.at((i+1)%n)));
	}

}