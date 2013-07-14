#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#define DDS_BLOCKSIZE (1<<20)
#define DDS_INTERLEAVE (1<<24)
#define DDS_RL (7)
#define BOOLINT char

#define DDS_ISINTEL (*((unsigned char *)(&DDS_INTEL)+1)==0)
#define ERRORMSG() errormsg(__FILE__,__LINE__)

#define FORU(q,n) for(size_t q=0;q<n;q++)
#define FOR(q,n) for(int q=0;q<n;q++)

typedef unsigned char GLubyte;

inline void errormsg(const char *file,int line)
{
	fprintf(stderr,"fatal error in <%s> en la linea %d!\n",file,line);
	exit(EXIT_FAILURE);
}

//leo el volumen desde un raw
GLubyte* ReadRawVolume(const char *filename, int *dimx,int *dimy, int *dimz, int* components);

/*
*	codigo que en lvida volvere a revisar
*/

inline void DDS_swapuint(unsigned int *x);

// helper functions for DDS:

inline unsigned int DDS_shiftl(const unsigned int value,const unsigned int bits)
{return((bits>=32)?0:value<<bits);}

inline unsigned int DDS_shiftr(const unsigned int value,const unsigned int bits)
{return((bits>=32)?0:value>>bits);}

inline void DDS_initbuffer();

inline int DDS_decode(int bits)
{return(bits>=1?bits+1:bits);}

inline void DDS_loadbits(unsigned char *data,unsigned int size);


void DDS_deinterleave(unsigned char *data,unsigned int bytes,unsigned int skip,unsigned int block,BOOLINT restore);

inline unsigned int DDS_readbits(unsigned int bits);

void DDS_interleave(unsigned char *data,unsigned int bytes,unsigned int skip,unsigned int block);

void DDS_decode(unsigned char *chunk,unsigned int size,
				unsigned char **data,unsigned int *bytes,
				unsigned int block);

unsigned char *readRAWfiled(FILE *file,unsigned int *bytes);

unsigned char *readRAWfile(const char *filename,unsigned int *bytes);

unsigned char *readDDSfile(const char *filename,unsigned int *bytes);

GLubyte *readPVMvolume(const char *filename,
							 unsigned int *width,unsigned int *height,unsigned int *depth,unsigned int *components,
							 float *scalex,float *scaley,float *scalez);

/* * */