#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <ctime>
#include <cstring>
using namespace std;

int** Matriz;
bool disponible;
int cantL;
int numL=0;

void imprimir(int tam){

	for(int i=0;i<tam;i++){
		for(int j=0;j<tam;j++){
			cout<<Matriz[i][j]<<' ';
		}
		cout<<endl;
	}
	cout<<endl;
}

void fill(int x,int y){
	int r=0;
	if (Matriz[x][y]==Matriz[x][y+1])
		r=2;
	if (Matriz[x+1][y]==Matriz[x][y])
		r=0x0001|r;
	numL++;
	switch(r){
		// _|
	case 0:
		Matriz[x+1][y+1]=numL;
		Matriz[x+1][y]=numL;
		Matriz[x][y+1]=numL;
		break;		
		//  _
		//   |
	case 2:
		Matriz[x][y]=numL;
		Matriz[x][y+1]=numL;
		Matriz[x+1][y+1]=numL;
		break;
		// |_
	case 1:
		Matriz[x][y]=numL;
		Matriz[x+1][y]=numL;
		Matriz[x+1][y+1]=numL;
		break;
		//   _
		//  |
	case 3:
		Matriz[x][y]=numL;
		Matriz[x+1][y]=numL;
		Matriz[x][y+1]=numL;
		break;
	default: 
		break;
	}
}

//el x tiene el valor de xi,yi(del cuadrante actual) + tam/2
int randL2_0(int x,int y,int r2){

	srand(time(NULL));

	int r=0;
	numL++;
	if (disponible){//el "centro" esta vacio
		r= rand() % 4;
		Matriz[x+((0x0002&r)?1:0)][y+((0x0001&r)?1:0)]=0;
	}else{
		r=r2;
	}

	switch(r){
		// _|
	case 0:
		Matriz[x+1][y+1]=numL;
		Matriz[x+1][y]=numL;
		Matriz[x][y+1]=numL;
		break;		
		//  _
		//   |
	case 2:
		Matriz[x][y]=numL;
		Matriz[x][y+1]=numL;
		Matriz[x+1][y+1]=numL;
		break;
		// |_
	case 1:
		Matriz[x][y]=numL;
		Matriz[x+1][y]=numL;
		Matriz[x+1][y+1]=numL;
		break;
		//   _
		//  |
	case 3:
		Matriz[x][y]=numL;
		Matriz[x+1][y]=numL;
		Matriz[x][y+1]=numL;
		break;
	default: 
		break;
	}
	if (disponible){
		r=4;
		disponible=false;
	}
	return r;
}

void configM(int* Matriz[],int xi,int yi, int tam,int r){

	int r2;
	if(tam==2){
		//caso base
		if (!disponible)
			fill(xi,yi);
		else
			randL2_0(xi+(tam>>1)-1,yi+(tam>>1)-1,r);
	}else{
		//DIVIDE AND CONQUER
		r2=randL2_0(xi+(tam>>1)-1,yi+(tam>>1)-1,r);
		//imprimir(16);

		if(r2==4){
			configM(Matriz,xi,yi,tam>>1,3);
			configM(Matriz,xi,yi+(tam>>1),tam>>1,2);
			configM(Matriz,xi+(tam>>1),yi,tam>>1,1);
			configM(Matriz,xi+(tam>>1),yi+(tam>>1),tam>>1,0);
		}else{
			if (r2!=0){
				configM(Matriz,xi,yi,tam>>1,3);
			}else{
				configM(Matriz,xi,yi,tam>>1,0);
			}
			if (r2!=1){
				configM(Matriz,xi,yi+(tam>>1),tam>>1,2);
			}else{
				configM(Matriz,xi,yi+(tam>>1),tam>>1,1);
			}
			if (r2!=2){
				configM(Matriz,xi+(tam>>1),yi,tam>>1,1);
			}else{
				configM(Matriz,xi+(tam>>1),yi,tam>>1,2);
			}
			if (r2!=3){
				configM(Matriz,xi+(tam>>1),yi+(tam>>1),tam>>1,0);
			}else{
				configM(Matriz,xi+(tam>>1),yi+(tam>>1),tam>>1,3);
			}
		}
	}

}

int main(){

	int n=-1,k=-1,tam_act;
	vector<int> numK;

	while(k!=0){
		cin>>k;
		numK.push_back(k);
		n++;
	}


	for(int i=0;i<n;i++){

		Matriz= new int*[1<<numK[i]];
		numL=0;
		tam_act=1<<numK[i];
		disponible=true;
		for (int j=0;j<(1<<numK[i]);j++){
			Matriz[j]=new int[tam_act];
			Matriz[0][j]=-1;
		}
		for (int j=1;j<tam_act;j++)
			memcpy(Matriz[j],Matriz[0],sizeof(int)*(tam_act));

		cantL=((1<<(numK[i]<<1))-1)/3;
		configM(Matriz,0,0,tam_act,-1);		

		//hacemos el print o cout
		imprimir(tam_act);

		//Borramos la matriz para el siguiente caso
		for (int j=0;j<(tam_act);j++)
			delete[] Matriz[j];
		delete[] Matriz;

	}

	cin.get();
	return 0;
}
