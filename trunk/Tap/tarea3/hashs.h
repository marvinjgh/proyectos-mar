#include "Hash.h"
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define log2 0.301029995
#define raizde5 2.236067978

ptrdiff_t unrandom (ptrdiff_t i) { return rand()%i;};
ptrdiff_t (*p_myrandom)(ptrdiff_t) = unrandom;

template<class T1, class T2>
class Xor: public Hash<T1,T2>{
	public:
	Xor(int i): Hash<T1,T2>:: Hash(i){};
	int Dispersion(T1 Nombre){
		string str = (string) Nombre;
		size_t h=0,i=0;
		while(i<str.size()) h^=str[i++];
		return h%(Hash<T1,T2>::M);
	};
};

template<class T1, class T2>
class Hashpjw: public Hash<T1,T2> {
	public:
	Hashpjw(size_t i): Hash<T1,T2>:: Hash(i){};
	int Dispersion(T1 Nombre){
			string s= (string)Nombre;
			size_t h=0, g,i;
			for (i=0; i<s.size(); i++)
			{
				h=(h<<4) + s[i];
				if (g=h & 0xf0000000)
				{
					h=h^(g>>24);
					h=h^g;
				}
			}
		return h%(Hash<T1,T2>::M);
	}
};

template<class T1, class T2>
class Mult:  public Hash<T1,T2>{
	public:
	Mult(int i): Hash<T1,T2>:: Hash(i){};
	
	int Dispersion(T1 Nombre){
		size_t constante;
		size_t x;	
		size_t logM;
		string str = (string)Nombre;
		constante = (size_t)((1<<16) *(raizde5 +1.0 ) / 2.0);
		x = 0;
		for(size_t a = 0; a < str.size(); a++)
			x = x + ((unsigned int) str[a]);
		x = x * constante;					
		logM = (int)(log10(Hash<T1,T2>::M) / log2);
		x = x << (16 - (logM >> 1));
		x = x >> 32 - logM;
		return x;

	}

};

template<class T1, class T2>
class Random: public Hash<T1,T2>{ 
	public:
	vector <char> random;

	Random(int i): Hash<T1,T2>:: Hash(i){
		srand(time(NULL)); 
		for(int i=0;i<256;i++){
			random.push_back((char)i);
		}
		random_shuffle( random.begin(), random.end(),p_myrandom);
	};

	int Dispersion(T1 Nombre){ 
		size_t h=0,i=0;
		string str = (string)Nombre;
		while(i<str.size()){ 
			if (str[i]<0)
			{
				h+=random[(128+str[i])+128];
				i++;
			}else
				h+=random[str[i++]];
		}
		return h%(Hash<T1,T2>::M);
	}
};

template<class T1, class T2>
class Centro: public Hash<T1,T2>{
	public:
	Centro(int i): Hash<T1,T2>:: Hash(i){};
	int Dispersion(T1 Nombre){
		string str = (string)Nombre;
		size_t h=0,i=0;
		if(str.length()<5){
			while(i<str.size()) h^=str[i++];
		}else{
			for(i=str.length()/2-2;i<str.length()/2+2;i++){ 
				h^=str[i];
			}
		}
		return h%(Hash<T1,T2>::M);
	};
	
};


//similar a hash_centro
template<class T1, class T2>
class Ultimo: public Hash<T1,T2>{ 
	public:
	Ultimo(int i): Hash<T1,T2>:: Hash(i){};
	int Dispersion(T1 Nombre){
		string str = (string)Nombre;
		size_t h=0,i=0;
		if(str.length()<5){ //si el sring tiene 4 letras o menos sencillamente se aplica el xor
			while(i<str.size()) h^=str[i++];
		}else{
			for(i=str.length()-4;i<str.length();i++){
				h^=str[i];
			}
		}
		return h%(Hash<T1,T2>::M);
	};
	
};

template<class T1, class T2>
class Adicion:  public Hash<T1,T2>{
	public:
	Adicion(int i): Hash<T1,T2>:: Hash(i){};
	int Dispersion(T1 Nombre){ 
		string str = (string)Nombre;
		size_t h=0,i=0;
		while(i<str.size()) h+=str[i++];
		return h%(Hash<T1,T2>::M);
	};
	
};