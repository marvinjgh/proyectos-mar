#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

template<class T1, class T2>


class Hash{

public:

	struct Data{
		T1 Nombre;
		T2 Valor;
		Data *Prox;	
	};

	vector<Data*> Tabla;
	int M;
	Hash(int n){
		M=n;
		Tabla = vector<Data*>(M,NULL);
	}

	~Hash(){

	}

	int Capacidad(){
		return M;
	}

	virtual int Dispersion(T1 Nombre){
		return 0;
	}

	void Insertar(T1 Nombre, T2 Valor){

		int pos=Dispersion(Nombre);

		if(Tabla[pos]==NULL){

			Data *nuevo = new Data();
			nuevo->Nombre=Nombre;
			nuevo->Valor=Valor;
			nuevo->Prox=NULL;

			Tabla[pos]= nuevo;

		}else{

			Data *aux=Tabla[pos];

			while(aux->Prox!=NULL)	aux=aux->Prox;

			if (aux->Nombre.compare(Nombre)==0)
				aux->Valor=Valor;
			else{
				Data *nuevo = new Data();
				nuevo->Nombre=Nombre;
				nuevo->Valor=Valor;
				nuevo->Prox=NULL;
				aux->Prox= nuevo;

			}
		}

	}

	bool Verificar(T1 Nombre){

		int pos=Dispersion(Nombre);

		if(Tabla[pos]==NULL)

			return false;

		else{

			Data *aux = Tabla[pos];

			while( aux != NULL && aux->Nombre.compare(Nombre)!=0 )	aux=aux->Prox;

			if(aux==NULL)

				return false;

			else

				return true;

		}

	}

	void Eliminar(T1 Nombre){

		int pos=Dispersion(Nombre);

		if (Tabla[pos] != NULL){

			Data *anterior =NULL;
			Data *actual= Tabla[pos];

			while(actual->Prox != NULL && actual->Nombre.compare(Nombre)!=0 ){
				anterior=actual;
				actual=actual->Prox;
			}

			if(actual->Nombre.compare(Nombre)==0){

				if(anterior==NULL){

					Data *proximo=actual->Prox;
					delete actual;
					Tabla[pos]=proximo;

				}else{

					Data *aux=actual->Prox;
					delete actual;
					anterior->Prox=aux;

				}

			}

		}

	}

	T2 operator[] (T1 Nombre){

		int pos=Dispersion(Nombre);

		if(Tabla[pos]==NULL)

			return -1;

		else{

			Data *aux = Tabla[pos];

			while( aux != NULL && aux->Nombre.compare(Nombre)!=0 )	aux=aux->Prox;

			if(aux==NULL)

				return -1;

			else

				return aux->Valor;

		}

	}

};

