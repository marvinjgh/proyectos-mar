//#include <iostream>
//#include "hashs.h"
//#include <fstream>
//
//
//int main(){
//
//	ofstream sal ("salida.txt",ios::out);
//	ifstream entrada;
//	string aux;
//	size_t sum1,sum2,sum3,sum4,sum5,sum6,sum7,sum8;
//	sal<<"caso|Adicion(31)|Adicion(217)|Xor(217)|Hashpjw(211)|Multiplicacion(256)|Random(100)|4 ultimos(211)|4 centrales(211)|"<<endl;
//	for (int i = 0; i < 6; i++)
//	{
//		Adicion<string,int> h1(31);
//		Adicion<string,int> h2(217);
//		Xor<string,int> h3(217);
//		Hashpjw<string,int> h4(211);
//		Mult<string,int> h5(256);
//		Random<string,int> h6(100);
//		Centro<string,int> h7(211);
//		Ultimo<string,int> h8(211);	
//		sum1=sum2=sum3=sum4=sum5=sum6=sum7=sum8=0;
//
//		if (i==0){
//			sal<<"100 palabras|";
//			entrada.open("Caso1.txt",ios::in);
//			while(!entrada.eof()){
//				entrada>>aux;
//				h1.Insertar(aux,1);
//				h2.Insertar(aux,1);
//				h3.Insertar(aux,1);
//				h4.Insertar(aux,1);
//				h5.Insertar(aux,1);
//				h6.Insertar(aux,1);
//				h7.Insertar(aux,1);
//				h8.Insertar(aux,1);
//			}
//
//		}
//		if (i==1){
//			sal<<"200 palabras|";
//			entrada.open("Caso2.txt",ios::in);
//			while(!entrada.eof()){
//				entrada>>aux;
//				h1.Insertar(aux,1);
//				h2.Insertar(aux,1);
//				h3.Insertar(aux,1);
//				h4.Insertar(aux,1);
//				h5.Insertar(aux,1);
//				h6.Insertar(aux,1);
//				h7.Insertar(aux,1);
//				h8.Insertar(aux,1);
//			}
//
//		}
//		if (i==2){
//			sal<<"500 palabras|";
//			entrada.open("Caso3.txt",ios::in);
//			while(!entrada.eof()){
//				entrada>>aux;
//				h1.Insertar(aux,1);
//				h2.Insertar(aux,1);
//				h3.Insertar(aux,1);
//				h4.Insertar(aux,1);
//				h5.Insertar(aux,1);
//				h6.Insertar(aux,1);
//				h7.Insertar(aux,1);
//				h8.Insertar(aux,1);
//			}
//
//		}
//		if (i==3){
//			sal<<"nombres v100-v399|";
//			entrada.open("Caso4.txt",ios::in);
//			while(!entrada.eof()){
//				entrada>>aux;
//				h1.Insertar(aux,1);
//				h2.Insertar(aux,1);
//				h3.Insertar(aux,1);
//				h4.Insertar(aux,1);
//				h5.Insertar(aux,1);
//				h6.Insertar(aux,1);
//				h7.Insertar(aux,1);
//				h8.Insertar(aux,1);
//			}
//
//		}
//		if (i==4){
//			sal<<"500 palabras entre 3-15|";
//			entrada.open("Caso5.txt",ios::in);
//			while(!entrada.eof()){
//				entrada>>aux;
//				h1.Insertar(aux,1);
//				h2.Insertar(aux,1);
//				h3.Insertar(aux,1);
//				h4.Insertar(aux,1);
//				h5.Insertar(aux,1);
//				h6.Insertar(aux,1);
//				h7.Insertar(aux,1);
//				h8.Insertar(aux,1);
//			}
//
//		}
//		if (i==5){
//			sal<<"codigo fuente|";
//			entrada.open("Caso6.txt",ios::in);
//			while(!entrada.eof()){
//				entrada>>aux;
//				h1.Insertar(aux,1);
//				h2.Insertar(aux,1);
//				h3.Insertar(aux,1);
//				h4.Insertar(aux,1);
//				h5.Insertar(aux,1);
//				h6.Insertar(aux,1);
//				h7.Insertar(aux,1);
//				h8.Insertar(aux,1);
//			}
//
//		}
//		for (int i = 0; i < 31; i++){
//			int x =0;
//			Adicion<string,int>::Data* auxd;
//			auxd=h1.Tabla[i];
//			while(auxd!=NULL)	{auxd=auxd->Prox;x++;}
//			sum1 += (x*(x+1))>>1;
//		}
//
//		for(int i=0;i<217;i++){
//			int x =0;
//			Adicion<string,int>::Data* auxd;
//			auxd=h2.Tabla[i];
//			while(auxd!=NULL)	{auxd=auxd->Prox;x++;}
//			sum2+=	(x*(x+1))>>1;
//
//			x=0;
//			Xor<string,int>::Data* auxd1;
//			auxd1=h3.Tabla[i];
//			while(auxd1!=NULL)	{auxd1=auxd1->Prox;x++;}
//			sum3+=	(x*(x+1))>>1;
//		}
//
//		for(int i=0;i<211;i++){
//			int x =0;
//			Hashpjw<string,int>::Data* auxd;
//			auxd=h4.Tabla[i];
//			while(auxd!=NULL)	{auxd=auxd->Prox;x++;}
//			sum4+=	(x*(x+1))>>1;
//
//			x=0;
//			Centro<string,int>::Data* auxd1;
//			auxd1=h7.Tabla[i];
//			while(auxd1!=NULL)	{auxd1=auxd1->Prox;x++;}
//			sum7+=	(x*(x+1))>>1;
//
//			x=0;
//			Ultimo<string,int>::Data* auxd2;
//			auxd2=h8.Tabla[i];
//			while(auxd2!=NULL)	{auxd2=auxd2->Prox;x++;}
//			sum8+=	(x*(x+1))>>1;
//		}
//
//		for(int i=0;i<256;i++){
//			int x =0;
//			Mult<string,int>::Data* auxd;
//			auxd=h5.Tabla[i];
//			while(auxd!=NULL)	{auxd=auxd->Prox;x++;}
//			sum5+=	(x*(x+1))>>1;
//		}
//		for(int i=0;i<100;i++){
//			int x =0;
//			Random<string,int>::Data* auxd;
//			auxd=h6.Tabla[i];
//			while(auxd!=NULL)	{auxd=auxd->Prox;x++;}
//			sum6+=	(x*(x+1))>>1;
//		}
//
//		//|200 palabras|500 palabras
//		sal<<sum1<<"|"<<sum2<<"|"<<sum3<<"|"<<sum4<<"|"<<sum5<<"|"<<sum6<<"|"<<sum7<<"|"<<sum8<<"|"<<endl;
//		entrada.close();
//	}
//
//	sal.close();
//	cin.get();
//	return 0;
//}
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
//#include <cuda_runtime.h>

void MultiplicacionMatricesCPU(float *a, float *b, float *c, int size, int d);
double MultiplicacionMatricesGPU(float *a, float *b, float *c, int size, int m);

int main(int argc, char const *argv[])
{
	
	int s[]={2,4,32,64,512,1024,4096}, i=0, d=0, m,j=0;
	FILE *out = fopen("salida.out","w");
	for (j=1;j<2;j++){
		switch(s[j]){
			//bloque 2x2
			case 2: d=1; m=2; break;
			case 4: d=2; m=2; break;
			//bloque 8x8
			case 32: d=5; m=8; break;
			case 64: d=6; m=8; break;
			//bloque 16x16
			case 512: d=9; m=16; break;
			case 1024: d=10; m=16; break;
			case 4096: d=12; m=16; break;
		}

		size_t size = s[j]*s[j]*sizeof(float);

		float *a = (float*) malloc(size), *b = (float*) malloc(size), *c = (float*) malloc(size);

		for (i = 0; i < s[j]*s[j]; i++)
		{
			a[i]=b[i]=1.f;
			c[i]=0.f;
		}

		time_t tiempo = time(NULL);
		double seg=0.0;

		MultiplicacionMatricesCPU(a,b,c,s[j],d);

		seg = time(NULL) - tiempo ;

		fprintf(out,"El tiempo para una matriz de  %d x %d con cpu es de %f\n",s[j],s[j],seg);
		for (int i = 0; i < s[j]*s[j]; ++i)
		{
			printf("%f\n", c[i]);
		}
		//seg = MultiplicacionMatricesGPU(a, b, c, s[j], m);
		fprintf(out,"El tiempo para una matriz de  %d x %d con gpu es de %f y bloques grid %d\n",s[j],s[j],seg,s[j]/m);
		
		free(a);
		free(b);
		free(c);
	}
	fclose(out);

	return 0;
}


// a, b, c son matrices cuadradas con dimenciones sizexsize
void MultiplicacionMatricesCPU(float *a, float *b, float *c, int size, int d)
{
	int i,j,cu=size*size,di=0,mo;
	
	for (i = 0; i < cu; i++)
	{
		if (i >= size)
		{
			di = (i>>d)<<d;
			mo = i%size;
		}else
			mo=i;
		for (j = 0; j < size; j++)
		{
			c[i] += a[j+di] * b[mo + (j<<d)];
		}
	}
}