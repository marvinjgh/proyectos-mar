// tarea11.cpp
//
#include "poligono.h"

std::vector<Poligono>ps;

void lectura(){
	int c=0, n=0, p=0;
	punto ini, des;
	float x,y;
	scanf("%d",&c);//numero de casos

	FOR(i,c){
		scanf("%d",&p);//numero de poligonos
		ps.clear();
		FOR(j,p){		
			scanf("%d",&n);//numero de puntos
			Poligono po(n);
			FOR(k,n){
				scanf("%f %f",&x,&y);
				punto p;
				p.x=x;
				p.y=y;
				puntos.push_back(p);

				po.agregarPunto(puntos.size()-1);
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
}

int main(int argc, char* argv[]){

	return 0;
}

