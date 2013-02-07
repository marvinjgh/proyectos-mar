#include <iostream>
#include <queue>
using namespace std;

short n,r,d,anterior;
short matrix[50][50];
short visitadas1[50][50];
short visitadas2[50][50];
enum direccion {N,O,S,E};
enum accion {A,RD,RI,STAR};
direccion r1,r2;
short rx1,ry1,rx2,ry2;//posicion de cada robot
short dx1,dy1,dx2,dy2;//posicion de cada destino
char c;
int minimo;


struct nodo{
	accion x;
	int carga;
};

queue<nodo> cola;

int avanzar(int pasos){
	if (pasos>0){
		visitadas1[rx1][ry1]=true;
		visitadas2[rx2][ry2]=true;

		//robot 1
		if (r1==N)
			rx1-=pasos;

		if (r1==S)
			rx1+=pasos;

		if (r1==O)
			ry1-=pasos;

		if (r1==E)
			ry1+=pasos;

		//robot 2
		if (r2==N)
			rx2-=pasos;

		if (r2==S)
			rx2+=pasos;

		if (r2==O)
			ry2-=pasos;

		if (r2==E)
			ry2+=pasos;

		//validaciones

		if (rx2<0 || rx2>=n || ry2<0 || ry2>=n || matrix[rx2][ry2] == 1  ){
			if (r2==N)
				rx2+=pasos;

			if (r2==S)
				rx2-=pasos;

			if (r2==O)
				ry2+=pasos;

			if (r2==E)
				ry2-=pasos;
		}

		if (rx1<0 || rx1>=n || ry1<0 || ry1>=n || matrix[rx1][ry1] == 1 ){
			if (r1==N)
				rx1+=pasos;

			if (r1==S)
				rx1-=pasos;

			if (r1==O)
				ry1+=pasos;

			if (r1==E)
				ry1-=pasos;
		}

		if (rx1==rx2 && ry1==ry2){
			if (r2==N)
				rx2+=pasos;

			if (r2==S)
				rx2-=pasos;

			if (r2==O)
				ry2+=pasos;

			if (r2==E)
				ry2-=pasos;

			if (r1==N)
				rx1+=pasos;

			if (r1==S)
				rx1-=pasos;

			if (r1==O)
				ry1+=pasos;

			if (r1==E)
				ry1-=pasos;
		}

		if ((visitadas1[rx1][ry1]&&visitadas2[rx2][ry2])||(visitadas2[rx1][ry1]&&visitadas1[rx2][ry2])){
			if (r2==N)
				rx2+=pasos;

			if (r2==S)
				rx2-=pasos;

			if (r2==O)
				ry2+=pasos;

			if (r2==E)
				ry2-=pasos;

			if (r1==N)
				rx1+=pasos;

			if (r1==S)
				rx1-=pasos;

			if (r1==O)
				ry1+=pasos;

			if (r1==E)
				ry1-=pasos;
			return -1;
		}


	}else{
		if (r1==N)
			rx1-=pasos;

		if (r1==S)
			rx1+=pasos;

		if (r1==O)
			ry1-=pasos;

		if (r1==E)
			ry1+=pasos;

		//robot 2
		if (r2==N)
			rx2-=pasos;

		if (r2==S)
			rx2+=pasos;

		if (r2==O)
			ry2-=pasos;

		if (r2==E)
			ry2+=pasos;

	}
	return 1;
}

void rotar(int direc){
	switch (r1)
	{
	case N:r1=(direc)?O:E;
		break;
	case O:r1=(direc)?S:N;
		break;
	case S:r1=(direc)?E:O;
		break;
	case E:r1=(direc)?N:S;
		break;
	default:;
		break;
	}

	switch (r2)
	{
	case N:r2=(direc)?O:E;
		break;
	case O:r2=(direc)?S:N;
		break;
	case S:r2=(direc)?E:O;
		break;
	case E:r2=(direc)?N:S;
		break;
	default:
		break;
	}

}

int visitar(int sadf){

	int m=1000000,min=1000000,m2;

	while (!cola.empty()){
		nodo actual=cola.front();
		cola.pop();
		for (int i=0;i<gr.vertex,++i){
			if (gr.matriz[actual][i]>0&&!visitade[i]){
				q.push(i);
				visitad[i]=i;
			}}
	}

	return min;

}

int main(int argc, char* argv[])
{

	while (true){
		//inicializar la matriz
		for (short i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++){
				matrix[i][j]=-1;
				visitadas1[i][j]=false;
				visitadas2[i][j]=false;
			}
			r=0;d=0;

			cin >> n;
			if (n==0) break;
			for (short i = 0; i < n; i++)
				for (short j = 0; j < n; j++){
					cin>>c;
					if (c=='.'){
						matrix[i][j]=0;
					}
					if (c=='*'){
						matrix[i][j]=1;
					}
					if (c=='D'){
						if (d==0){dx1=i;dy1=j;++d;}
						else{dx2=i;dy2=j;++d;}
						matrix[i][j]=0;
					}
					if (c=='N'||c=='S'||c=='E'||c=='O'){
						if (r==0){	
							rx1=i;ry1=j;
							++r;
							r1=(c=='N')?N:(c=='S')?S:(c=='E')?E:O;
						}
						else{
							rx2=i;ry2=j;
							++d;
							r2=(c=='N')?N:(c=='S')?S:(c=='E')?E:O;
						}
						matrix[i][j]=0;
					}
				}
				anterior=0;
				int min = visitar(0);

				for (short i = 0; i < n; i++){
					for (short j = 0; j < n; j++)
						cout<<"|"<<matrix[i][j];
					cout<<endl;
				}

				cout<<min;
	}
	return 0;
}

