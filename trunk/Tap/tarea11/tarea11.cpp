// tarea11.cpp
//
#include "poligono.h"
#include <cmath>

std::vector<Poligono>ps;

float calcular_dis(punto ini, punto des, float acum){

	punto origen = ini,aux;
	bool b=false;
	float d;int c,asd=0;
	if (acum > min_all)
		return INF_F;
	FOR(i,ps.size()){
		b = ps.at(i).interseccion(origen,des);
		if (b){ asd=i;break;}
	}

	if (b){
		FOR(i,puntos.size()){
			if (pun_b[i]){
				b=false;c=0;
				FOR(j,ps.size()){
					aux = ps.at(j).interseccion_p(origen,puntos.at(i));
					//b=ps.at(j).interseccion(origen,puntos.at(i));
					if (aux.x == puntos[i].x && aux.y == puntos[i].y){
						c++;
					}else{
						if ((aux.p%2)==1 && aux.p>2){
							b=true;
						}
						if ((acum)==0 && aux.x==(-INF_F+1) && aux.y==(-INF_F+1)){
							b=true;
						}
					}
				}
				if (c==1 && !b){
					d = acum + distancia(ini,puntos.at(i));
					//printf("%.2f %.2f\n",puntos.at(i).x,puntos.at(i).y);
					pun_b[i]=false;
					calcular_dis(puntos.at(i),des,d);
					pun_b[i]=true;
				}
			}
		}
	}else{
		d=acum+distancia(ini,des);
		if (min_all > d){
			min_all = d;
			//printf("min %.2f\n",min_all);
		}
		return d;
	}

	return 0;
}


int main(int argc, char* argv[]){

	int c=0, n=0, p=0;
	punto ini, des;
	float x,y,dis=-INF_F;

	scanf("%d",&c);//numero de casos

	FOR(i,c){
		scanf("%d",&p);//numero de poligonos
		ps.clear();
		FOR(j,p){		
			scanf("%d",&n);//numero de puntos
			Poligono po(n);
			FOR(k,n){
				scanf("%f %f",&x,&y);
				punto p(x,y,j);
				puntos.push_back(p);

				po.agregarPunto(puntos.size()-1);
			}
			po.crearAristas();
			ps.push_back(po);
		}
		scanf("%f %f",&x,&y);
		ini.x=x;
		ini.y=y;
		scanf("%f %f",&x,&y);
		des.x=x;
		des.y=y;
		min_all=INF_F;
		pun_b = new bool[puntos.size()];
		FOR(j,puntos.size()){
			pun_b[j]=true;	
		}

		dis = calcular_dis(ini,des,0);
		printf("%.2f\n",min_all);
		//aqui debo hace el grafo para luego hacer el distra
		c = getchar();
		c = getchar();
	}

	return 0;
}

