// tarea11.cpp
//
#include "poligono.h"
#include <cstdio>

#define FOR(q,n) for(int q = 0; q < n; q++)

int main(int argc, char* argv[]){

	int c=0, n=0, p=0;
	punto ini, des;
	float x,y;
	scanf("%d",&c);//numero de casos
	std::vector<Poligono>ps;
	FOR(i,c){
		scanf("%d",&p);//numero de poligonos
		ps.clear();
		FOR(j,p){		
			scanf("%d",&n);//numero de puntos
			Poligono po(n);
			FOR(k,n){
				scanf("%f %f",&x,&y);
				po.agregarPunto(x,y);
			}
			ps.push_back(po);
		}
		scanf("%f %f",&x,&y);
		ini.x=x;
		ini.y=y;
		scanf("%f %f",&x,&y);
		des.x=x;
		des.y=y;
		//aqui debo hace el grafo para luego hacer el distra
	}

	return 0;
}

