#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define ABS(x) (x < 0 ? -(x) : (x))

using namespace std;

struct coord{
	int x;
	int y;
	coord(int x, int y){
		this->x=x;
		this->y=y;
	}
	coord(){
		this->x=0;
		this->y=0;
	}
};

struct nodo{
	double costo; // energia usada
	int h; // heuristica
	int f; // costo + h
	coord pos;
	nodo *padre;
	nodo(int x, int y, nodo *padre, double costo){
		pos.x=x;
		pos.y=y;
		this->padre=padre;
		this->costo=costo;		
	}
	nodo(int x, int y){
		pos.x=x;
		pos.y=y;
		this->padre=NULL;
		this->costo=0;		
	}
	nodo(){
		pos.x=0;
		pos.y=0;
		this->padre=NULL;
		this->costo=0;		
	}
};

char Matriz[8][8];
vector<nodo*> close;
vector<nodo*> sucesores;
vector<nodo*> salida;
vector<coord> mineral_pos;
int n,m,minerales;
double min_a;
nodo inicio;

double heuristica (){
	return 0.0;
}

bool moverRoca(int x, int y,int d){
	
	if (d){
		if(y+1<=n){
			if(Matriz[x][y+1]=='.')
				return true;
			else
				return false;
		}
	}else{
		if(y-1>=0){
			if(Matriz[x][y-1]=='.')
				return true;
			else
				return false;	
		}
	}
	return false;
}

void obtenerSucesor(nodo *padre){

	bool haymineral=false;
	nodo *nuevo;
	int x=padre->pos.x,y=padre->pos.y;

	if(x-1>=0 && Matriz[x-1][y]=='*'){
		nuevo=new nodo(x-1,y,padre,1);
		sucesores.push_back(nuevo);
		haymineral=true;
	}

	if(x+1<n && Matriz[x+1][y]=='*'){
		nuevo=new nodo(x+1,y,padre,1);
		sucesores.push_back(nuevo);
		haymineral=true;
	}

	if(y-1>=0 && Matriz[x][y-1]=='*'){
		nuevo=new nodo(x,y-1,padre,1);
		sucesores.push_back(nuevo);
		haymineral=true;
	}

	if(y+1<m && Matriz[x][y+1]=='*'){
		nuevo=new nodo(x,y+1,padre,1);
		sucesores.push_back(nuevo);
		haymineral=true;
	}

	if(!haymineral){

		if(x-1>=0){

			if(Matriz[x-1][y]=='.')
				nuevo=new nodo(x-1,y,padre,1);

			if(Matriz[x-1][y]=='-')
				nuevo=new nodo(x-1,y,padre,1.5);

			if(Matriz[x-1][y]=='@' && moverRoca(x-1,y,0))
				nuevo=new nodo(x-1,y,padre,2);

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,1);

			sucesores.push_back(nuevo);
			nuevo = NULL;
		}

		if(x+1<n){

			if(Matriz[x+1][y]=='.')
				nuevo=new nodo(x+1,y,padre,1);

			if(Matriz[x+1][y]=='-')
				nuevo=new nodo(x+1,y,padre,1.5);

			if(Matriz[x+1][y]=='@' && moverRoca(x+1,y,1))
				nuevo=new nodo(x+1,y,padre,2);

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,1);

			if (nuevo)
				sucesores.push_back(nuevo);							
		}

		if(y-1>=0){

			if(Matriz[x][y-1]=='.')
				nuevo=new nodo(x,y-1,padre,1);

			if(Matriz[x][y-1]=='-')
				nuevo=new nodo(x,y-1,padre,1.5);

			if(Matriz[x][y-1]=='@' && moverRoca(x,y-1,0))
				nuevo=new nodo(x,y-1,padre,2);	

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,1);

			if (nuevo)
				sucesores.push_back(nuevo);						
		}

		if(y+1<m){

			if(Matriz[x][y+1]=='.')
				nuevo=new nodo(x,y+1,padre,1);

			if(Matriz[x][y+1]=='-')
				nuevo=new nodo(x,y+1,padre,1.5);

			if(Matriz[x][y+1]=='@' && moverRoca(x,y+1,1))
				nuevo=new nodo(x,y+1,padre,2);

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,1);

			if (nuevo)
				sucesores.push_back(nuevo);							
		}
	}
}

nodo* elegirSucesor(){

	srand ( time(NULL) );
	int k= rand()% sucesores.size();

	return sucesores[k];	
}

bool esFin(){

	int final=close.size()-1;
	int n=salida.size();
	int i;

	if(minerales==0){

		for(i=0;i<n;i++){
			if(salida[i]->pos.x==close[final]->pos.x && salida[i]->pos.y==close[final]->pos.y)
				return true;
		}

	}else return false;

}

void obtenerSucesor_v1_1(nodo *padre,float mn[8][8]){

	bool haymineral=false;
	nodo *nuevo=NULL;
	int x=padre->pos.x,y=padre->pos.y;

	if(x-1>=0 && Matriz[x-1][y]=='*'){
		nuevo=new nodo(x-1,y,padre,padre->costo+1);
		sucesores.push_back(nuevo);
		haymineral=true;
		min_a=nuevo->costo;
	}

	if(x+1<n && Matriz[x+1][y]=='*'){
		nuevo=new nodo(x+1,y,padre,padre->costo+1);
		sucesores.push_back(nuevo);
		haymineral=true;
		min_a=nuevo->costo;
	}

	if(y-1>=0 && Matriz[x][y-1]=='*'){
		nuevo=new nodo(x,y-1,padre,padre->costo+1);
		sucesores.push_back(nuevo);
		haymineral=true;
		min_a=nuevo->costo;
	}

	if(y+1<m && Matriz[x][y+1]=='*'){
		nuevo=new nodo(x,y+1,padre,padre->costo+1);
		sucesores.push_back(nuevo);
		haymineral=true;
		min_a=nuevo->costo;
	}

	if(!haymineral){

		if(x-1>=0){

			if(Matriz[x-1][y]=='.')
				nuevo=new nodo(x-1,y,padre,padre->costo+1);

			if(Matriz[x-1][y]=='-')
				nuevo=new nodo(x-1,y,padre,padre->costo+1.5);

			//if(Matriz[x-1][y]=='@' && moverRoca(x-1,y,0))
			//nuevo=new nodo(x-1,y,padre,padre->costo+2);

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,padre->costo+1);

			if ((nuevo && mn[x-1][y]<0)||(nuevo && mn[x-1][y]>padre->costo))
				sucesores.push_back(nuevo);
			nuevo = NULL;
		}

		if(x+1<n){

			if(Matriz[x+1][y]=='.')
				nuevo=new nodo(x+1,y,padre,padre->costo+1);

			if(Matriz[x+1][y]=='-')
				nuevo=new nodo(x+1,y,padre,padre->costo+1.5);

			//if(Matriz[x+1][y]=='@' && moverRoca(x+1,y,1))
			//nuevo=new nodo(x+1,y,padre,padre->costo+2);

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,padre->costo+1);

			if ((nuevo && mn[x+1][y]<0)||(nuevo && mn[x+1][y]>padre->costo))
				sucesores.push_back(nuevo);
			nuevo = NULL;
		}

		if(y-1>=0){

			if(Matriz[x][y-1]=='.')
				nuevo=new nodo(x,y-1,padre,padre->costo+1);

			if(Matriz[x][y-1]=='-')
				nuevo=new nodo(x,y-1,padre,padre->costo+1.5);

			if(Matriz[x][y-1]=='@' && moverRoca(x,y-1,0))
				nuevo=new nodo(x,y-1,padre,padre->costo+2);	

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,padre->costo+1);

			if ((nuevo && mn[x][y-1]<0)||(nuevo && mn[x][y-1]>padre->costo))
				sucesores.push_back(nuevo);
			nuevo = NULL;
		}

		if(y+1<m){

			if(Matriz[x][y+1]=='.')
				nuevo=new nodo(x,y+1,padre,padre->costo+1);

			if(Matriz[x][y+1]=='-')
				nuevo=new nodo(x,y+1,padre,padre->costo+1.5);

			if(Matriz[x][y+1]=='@' && moverRoca(x,y+1,1))
				nuevo=new nodo(x,y+1,padre,padre->costo+2);

			if(minerales==0)	nuevo=new nodo(x-1,y,padre,padre->costo+1);

			if ((nuevo && mn[x][y+1]<0)||(nuevo && mn[x][y+1]>padre->costo))
				sucesores.push_back(nuevo);
			nuevo = NULL;
		}
	}
}

//obtener el costo de moverse del nodo actual al sucesor
double obtenerCosto(){
	return 0.0;
}

void BusquedaEstrella(float m[8][8],int asterisco){
	nodo* tmp = sucesores.at(0);

	int x = sucesores.size();
	while (x!=0){
		for (int i = 0; i < x; i++)
		{
			if (m[tmp->pos.x][tmp->pos.y]<tmp->costo && tmp->costo<min_a){
				m[tmp->pos.x][tmp->pos.y]=tmp->costo;
				obtenerSucesor_v1_1(tmp,m);
			}
			sucesores.erase(sucesores.begin());
			if (sucesores.size())
				tmp = sucesores.at(0);
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j  < 8; j ++)
			{
				printf("%.2f ",m[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		x=sucesores.size();
	}
}

void leer(int argc, char const *argv[]){

	int i,j;
	char aux;

	minerales=0;
	n=atoi(argv[1]);
	m=atoi(argv[2]);	

	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++){
			scanf("%c",&aux);
			if (aux!='\n'){
				Matriz[i][j]=aux;
			}else{
				j--;
			}
			if(aux=='*'){	
				minerales++;
				mineral_pos.push_back(coord(i,j));
			}
			if(aux=='S')	salida.push_back(new nodo(i,j,NULL,0));
			if(aux=='R'){
				inicio.pos.x=i;
				inicio.pos.y=j;
				inicio.padre=NULL;
				inicio.costo=0;
				sucesores.push_back(&inicio);
			}
		}

}

//retorna la posicion del asterisco mas cercano al r
int obtener_asterisco(nodo actual){
	int x;
	coord a= actual.pos;
	float d=0,d2=100;
	for (int i = 0; i < mineral_pos.size(); i++)
	{
		d=ABS(mineral_pos[i].x-a.x)+ABS(mineral_pos[i].y-a.y);
		if (d2>d){
			d2=d;
			x=i;
		}
	}
	return x;
}
int main(int argc, char const *argv[]){

	leer(argc,argv);
	min_a = 1000;
	float total=0;

	

	float m[8][8];
	for (int i = 0; i < minerales; i++)
	{
		int b = obtener_asterisco(inicio);
		BusquedaEstrella(m,b);

		if (min_a == 1000)
		{
			//no hay solucion
		}else{
			total=min_a;
			min_a=1000;
		}
		//actualizar el R
	}
	
	//si hay solucion 

	buscar_salida();

	return 0;
}

