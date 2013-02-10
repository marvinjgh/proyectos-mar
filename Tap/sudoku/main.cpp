#include <cstdio>
#include <ctime>
#include <cstring>

typedef unsigned short ent;

#define FOR(i,n) for(ent i = 0; i < n; i++)
#define SQFOR(i,n,s,q) for(ent i = s; i < n; i=i+q)
#define SFOR(i,n,s) for(ent i = s; i < n; i++)
#define cubo0 {0,1,2,9,10,11,18,19,20}
#define cubo1 {3,4,5,12,13,14,21,22,23}
#define cubo2 {6,7,8,15,16,17,24,25,26}
#define cubo3 {27,28,29,36,37,38,45,46,47}
#define cubo4 {30,31,32,39,40,41,48,49,50}
#define cubo5 {33,34,35,42,43,44,51,52,53}
#define cubo6 {54,55,56,63,64,65,72,73,74}
#define cubo7 {57,58,59,66,67,68,75,76,77}
#define cubo8 {60,61,62,69,70,71,78,79,80}

#define ICHI 0xffef
#define NI 0xffdf
#define SAN 0xffbf
#define YON 0xff7f
#define GO 0xfeff
#define ROKU 0xfdff
#define NANA 0xfbff
#define HACHI 0xf7ff
#define KYU 0xefff

/*
para hacer una optimizacion de la memoria utilizo enteros short,
tomo los primeros 4bit para representar el valor final,los siquientes
9 bit, son para los posibles valores que puede tomas la casilla
bbbbbbbbbbbbbbbb
||||||||||||||||
000987654321nnnn

*/

//sudoku
ent su[81];
ent pasos;
//cambio el estado de una bandera activa a inactiva
void cambiar_flag(ent j,ent i){
	switch (su[i]){
	case 1:
		su[j]&=ICHI;
		break;
	case 2:
		su[j]&=NI;
		break;
	case 3:
		su[j]&=SAN;
		break;
	case 4:
		su[j]&=YON;
		break;
	case 5:
		su[j]&=GO;
		break;
	case 6:
		su[j]&=ROKU;
		break;
	case 7:
		su[j]&=NANA;
		break;
	case 8:
		su[j]&=HACHI;
		break;
	case 9:
		su[j]&=KYU;
		break;
	}
}

//llena y actualiza los valores posibles para cada posicion del sudoku
bool actualizar(ent i){
	ent aux=0;
	//i es la posicion actual
	//FOR(i,81){
	//verifico si es una posicion vacia para actualizar sus banderas
	if ((su[i]&0x000f) != 0){
#pragma region fila
		aux=i-i%9;//consigo la comienzo de la fila
		SFOR(j,aux+9,aux){
			if (j!=i && (0x000f&su[j])==0){
				cambiar_flag(j,i);
			}
		}
#pragma endregion actualizacion con la fila
#pragma region columna
		//actualizacion en la columna
		aux=i%9;//consigo la comienzo de la columna
		SQFOR(j,81,aux,9){
			if (j!=i && (0x000f&su[j])==0){
				cambiar_flag(j,i);
			}
		}
#pragma endregion actualizacion con la columna
#pragma region cubo
		aux=(i/27)*3;
		aux+=(i%9)<3? 0 : (i%9)<6? 1 : 2;
		switch (aux){
		case 0:
			{
				ent cubo[] = cubo0;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 1:
			{
				ent cubo[] = cubo1;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 2:
			{
				ent cubo[] = cubo2;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 3:
			{
				ent cubo[] = cubo3;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 4:
			{
				ent cubo[] = cubo4;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 5:
			{
				ent cubo[] = cubo5;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 6:
			{
				ent cubo[] = cubo6;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 7:
			{
				ent cubo[] = cubo7;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		case 8:
			{
				ent cubo[] = cubo8;
				for (ent &j : cubo){
					if (j!=i && (0x000f&su[j])==0){
						cambiar_flag(j,i);
					}
				}
				break;
			}
		}
#pragma endregion actualizo con el cubo interno
	}else{
		if (su[i]==0)
			return false;
	}
	return true;
}

ent valor(char i){
	if (su[i]==0x0010)
		return 1;
	else if (su[i]==0x0020)
		return 2;
	else if (su[i]==0x0040)
		return 3;
	else if (su[i]==0x0080)
		return 4;
	else if (su[i]==0x0100)
		return 5;
	else if (su[i]==0x0200)
		return 6;
	else  if (su[i]==0x0400)
		return 7;
	else  if (su[i]==0x0800)
		return 8;
	else
		return 9;
}

void sudoku(){
	pasos++;
	ent backup[81];
	char min=10,pos=0,aux=0;
	bool status;
	FOR(i,81){
		if ((su[i]&0x000f)==0){
			FOR(j,9){
				if ((su[i]&(1<<4+j))){
					aux++;
				}
			}
			if (aux<min){
				min = aux;
				pos=i;
			}
			if (min==1){
				su[pos]=valor(pos);
				status = actualizar(pos);
				i=-1;
				min=10;
			}
			aux=0;
		}
	}
	if (min==10)
		return;
	memcpy(backup,su,sizeof(backup));
	FOR(i,9){
		if ((backup[pos]&(1<<4+i))){
			su[pos]=i+1;
			status = actualizar(pos);
			if (!status) memcpy(su,backup,sizeof(backup));;
			sudoku();
		}
	}
	


}



int main(){

	FILE* entrada;
	entrada = fopen("sudoku.in","r");
	char c;
	int n,p;

	fscanf(entrada,"%d",&n);

	FOR(k,n){
		pasos=0;
		c=fgetc(entrada);
		FOR(i,81){
			su[i]=0x1ff0;
		}
		FOR(i,81){
			c=fgetc(entrada);
			su[i]= c=='0'? su[i] : 0x000f&c;
			actualizar(i);
		}
		sudoku();
		printf("%d\n",pasos);
	}
	fclose(entrada);
	getchar();
	return 0;
}