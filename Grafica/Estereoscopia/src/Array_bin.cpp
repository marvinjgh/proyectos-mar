#include "Array_bin.h"


Array_bin::Array_bin(void)
{
	max=1<<8;
	size=0;
	array=(data*) malloc(max*sizeof(data));
}


Array_bin::~Array_bin(void)
{
	free(array);
}

void Array_bin::insertar(long long id, size_t v){
	
	if(size+1 > max && max!=0xffffffff){
		max=(max<<1)?max<<1:0xfffffffe;
		array=(data*)realloc(array,max*sizeof(data));
	}else if (max==0xffffffff){
		printf("No se puede colocar mas data");
		return;
	}

	long i=size;
	array[size]=data(id,v);
	data aux;
	while (i!=0){
		if (array[i].id<array[i-1].id)
		{
			aux=array[i];
			array[i]=array[i-1];
			array[i-1]=aux;
		}
		--i;
	}
	size++;
}

size_t Array_bin::busqueda(long long id){
	return busqueda_bin(id,0,size);
}

size_t Array_bin::busqueda_bin(long long dato, size_t star, size_t end){
	size_t res;
     if(star <= end  && end!=-1){
         int m = (star + end)>>1;
		 if(dato < array[m].id) res = busqueda_bin(dato, star, m-1);
         else if(dato > array[m].id) res = busqueda_bin(dato, m+1, end);
		 else res = array[m].v;
     }else res = -1;
     return res;
}

void Array_bin::limpiar(void){
	free(array);
	array=(data*) malloc(max*sizeof(data));
}