#include "lectura.h"

char DDS_ID[]="DDS v3d\n";
char DDS_ID2[]="DDS v3e\n";

unsigned int DDS_buffer;
unsigned int DDS_bufsize;

unsigned char *DDS_cache;
unsigned int DDS_cachepos,DDS_cachesize;
unsigned short int DDS_INTEL=1;

GLubyte* ReadRawVolume(const char *filename, int *dimx,int *dimy, int *dimz, int* components){
	char keys[] = "1234567890",*ptr;
	size_t i;
	std::string a = filename;
	ptr=(char*)strrchr(filename,'\\');
	a = ptr?ptr+1:filename;

	if (a.empty())
		a = filename;

	i = a.find_first_of(keys);

	a = a.substr(i, a.find(".raw")-i);
	i = a.find_first_of("x");

	ptr=strtok ((char*)a.c_str(),"x");
	if (ptr != NULL){
		*dimx = atoi(ptr);
	}else{
		*dimx = 0;
	}
	if ((ptr = strtok (NULL, "x") )!= NULL){
		*dimy = atoi(ptr);
	}else{
		*dimy = 0;
	}
	if ((ptr = strtok (NULL, "x") )!= NULL){
		*dimz = atoi(ptr);
	}else{
		*dimz = 0;
	}
	if ((ptr = strtok (NULL, "x") )!= NULL){
		*components = atoi(ptr);
	}else{
		*components = 0;
	}

	if (*dimx==0 || *dimy==0 || *dimz==0 || *components == 0) {
		*dimx=0; *dimy=0; *dimz=0; *components=0;
		return NULL;
	}

	FILE *fp;

	size_t size = (*dimx) * (*dimy) * (*dimz) * (*components), asd;   
	GLubyte *data = new GLubyte[size];            // 8bit   

	if (!(fp = fopen(filename, "rb")))   
	{   
		fprintf(stderr,"Error: fallo al abrir .raw file \n");   
		*dimx=0; *dimy=0; *dimz=0; *components=0;
		delete[] data;
		return NULL;
	}   

	if ( (asd=fread(data, sizeof(char), size, fp))!= size)   
	{  
		fprintf(stderr,"Error: read .raw file failed\n");
		*dimx=0; *dimy=0; *dimz=0; *components=0;
		fclose(fp);

		return NULL;
	}   
	else   
		fprintf(stderr,"OK: read .raw file successed\n");   

	return data;

}

/*
*	codigo que en lvida volvere a revisar
*/

inline void DDS_swapuint(unsigned int *x)
{
	unsigned int tmp=*x;

	*x=((tmp&0xff)<<24)|
		((tmp&0xff00)<<8)|
		((tmp&0xff0000)>>8)|
		((tmp&0xff000000)>>24);
}

// helper functions for DDS:

inline void DDS_initbuffer()
{
	DDS_buffer=0;
	DDS_bufsize=0;
	DDS_cache=NULL;
	DDS_cachepos=0;
	DDS_cachesize=0;
}

inline void DDS_loadbits(unsigned char *data,unsigned int size)
{
	DDS_cache=data;
	DDS_cachesize=size;

	if ((DDS_cache=(unsigned char *)realloc(DDS_cache,DDS_cachesize+4))==NULL) ERRORMSG();
	*((unsigned int *)&DDS_cache[DDS_cachesize])=0;

	DDS_cachesize=4*((DDS_cachesize+3)/4);
	if ((DDS_cache=(unsigned char *)realloc(DDS_cache,DDS_cachesize))==NULL) ERRORMSG();
}

void DDS_deinterleave(unsigned char *data,unsigned int bytes,unsigned int skip,unsigned int block=0,BOOLINT restore=false)
{
	unsigned int i,j,k;

	unsigned char *data2,*ptr;

	if (skip<=1) return;

	if (block==0)
	{
		if ((data2=(unsigned char *)malloc(bytes))==NULL) ERRORMSG();

		if (!restore)
			for (ptr=data2,i=0; i<skip; i++)
				for (j=i; j<bytes; j+=skip) *ptr++=data[j];
		else
			for (ptr=data,i=0; i<skip; i++)
				for (j=i; j<bytes; j+=skip) data2[j]=*ptr++;

		memcpy(data,data2,bytes);
	}
	else
	{
		if ((data2=(unsigned char *)malloc((bytes<skip*block)?bytes:skip*block))==NULL) ERRORMSG();

		if (!restore)
		{
			for (k=0; k<bytes/skip/block; k++)
			{
				for (ptr=data2,i=0; i<skip; i++)
					for (j=i; j<skip*block; j+=skip) *ptr++=data[k*skip*block+j];

				memcpy(data+k*skip*block,data2,skip*block);
			}

			for (ptr=data2,i=0; i<skip; i++)
				for (j=i; j<bytes-k*skip*block; j+=skip) *ptr++=data[k*skip*block+j];

			memcpy(data+k*skip*block,data2,bytes-k*skip*block);
		}
		else
		{
			for (k=0; k<bytes/skip/block; k++)
			{
				for (ptr=data+k*skip*block,i=0; i<skip; i++)
					for (j=i; j<skip*block; j+=skip) data2[j]=*ptr++;

				memcpy(data+k*skip*block,data2,skip*block);
			}

			for (ptr=data+k*skip*block,i=0; i<skip; i++)
				for (j=i; j<bytes-k*skip*block; j+=skip) data2[j]=*ptr++;

			memcpy(data+k*skip*block,data2,bytes-k*skip*block);
		}
	}

	free(data2);
}

inline unsigned int DDS_readbits(unsigned int bits)
{
	unsigned int value;

	if (bits<DDS_bufsize)
	{
		DDS_bufsize-=bits;
		value=DDS_shiftr(DDS_buffer,DDS_bufsize);
	}
	else
	{
		value=DDS_shiftl(DDS_buffer,bits-DDS_bufsize);

		if (DDS_cachepos>=DDS_cachesize) DDS_buffer=0;
		else
		{
			DDS_buffer=*((unsigned int *)&DDS_cache[DDS_cachepos]);
			if (DDS_ISINTEL) DDS_swapuint(&DDS_buffer);
			DDS_cachepos+=4;
		}

		DDS_bufsize+=32-bits;
		value|=DDS_shiftr(DDS_buffer,DDS_bufsize);
	}

	DDS_buffer&=DDS_shiftl(1,DDS_bufsize)-1;

	return(value);
}

void DDS_decode(unsigned char *chunk,unsigned int size,
				unsigned char **data,unsigned int *bytes,
				unsigned int block=0)
{
	unsigned int skip,strip;

	unsigned char *ptr1,*ptr2;

	unsigned int cnt,cnt1,cnt2;
	int bits,act;

	DDS_initbuffer();

	DDS_loadbits(chunk,size);

	skip=DDS_readbits(2)+1;
	strip=DDS_readbits(16)+1;

	ptr1=ptr2=NULL;
	cnt=act=0;

	while ((cnt1=DDS_readbits(DDS_RL))!=0)
	{
		bits=DDS_decode(DDS_readbits(3));

		for (cnt2=0; cnt2<cnt1; cnt2++)
		{
			if (strip==1 || cnt<=strip) act+=DDS_readbits(bits)-(1<<bits)/2;
			else act+=*(ptr2-strip)-*(ptr2-strip-1)+DDS_readbits(bits)-(1<<bits)/2;

			while (act<0) act+=256;
			while (act>255) act-=256;

			if ((cnt&(DDS_BLOCKSIZE-1))==0)
				if (ptr1==NULL)
				{
					if ((ptr1=(unsigned char *)malloc(DDS_BLOCKSIZE))==NULL) ERRORMSG();
					ptr2=ptr1;
				}
				else
				{
					if ((ptr1=(unsigned char *)realloc(ptr1,cnt+DDS_BLOCKSIZE))==NULL) ERRORMSG();
					ptr2=&ptr1[cnt];
				}

				*ptr2++=act;
				cnt++;
		}
	}

	if (ptr1!=NULL)
		if ((ptr1=(unsigned char *)realloc(ptr1,cnt))==NULL) ERRORMSG();

	DDS_interleave(ptr1,cnt,skip,block);

	*data=ptr1;
	*bytes=cnt;
}

unsigned char *readRAWfiled(FILE *file,unsigned int *bytes)
{
	unsigned char *data;
	unsigned int cnt,blkcnt;

	data=NULL;
	cnt=0;

	do
	{
		if (data==NULL)
		{if ((data=(unsigned char *)malloc(DDS_BLOCKSIZE))==NULL) ERRORMSG();}
		else
			if ((data=(unsigned char *)realloc(data,cnt+DDS_BLOCKSIZE))==NULL) ERRORMSG();

		blkcnt=fread(&data[cnt],1,DDS_BLOCKSIZE,file);
		cnt+=blkcnt;
	}
	while (blkcnt==DDS_BLOCKSIZE);

	if (cnt==0)
	{
		free(data);
		return(NULL);
	}

	if ((data=(unsigned char *)realloc(data,cnt))==NULL) ERRORMSG();

	*bytes=cnt;

	return(data);
}

unsigned char *readRAWfile(const char *filename,unsigned int *bytes)
{
	FILE *file;

	unsigned char *data;

	if ((file=fopen(filename,"rb"))==NULL) return(NULL);

	data=readRAWfiled(file,bytes);

	fclose(file);

	return(data);
}

unsigned char *readDDSfile(const char *filename,unsigned int *bytes)
{
	int version=1;

	FILE *file;

	int cnt;

	unsigned char *chunk,*data;
	unsigned int size;

	if ((file=fopen(filename,"rb"))==NULL) return(NULL);

	for (cnt=0; DDS_ID[cnt]!='\0'; cnt++)
		if (fgetc(file)!=DDS_ID[cnt])
		{
			fclose(file);
			version=0;
			break;
		}

		if (version==0)
		{
			if ((file=fopen(filename,"rb"))==NULL) return(NULL);

			for (cnt=0; DDS_ID2[cnt]!='\0'; cnt++)
				if (fgetc(file)!=DDS_ID2[cnt])
				{
					fclose(file);
					return(NULL);
				}

				version=2;
		}

		if ((chunk=readRAWfiled(file,&size))==NULL) ERRORMSG();

		fclose(file);

		DDS_decode(chunk,size,&data,bytes,version==1?0:DDS_INTERLEAVE);

		free(chunk);

		return(data);
}


GLubyte *readPVMvolume(const char *filename,
					   unsigned int *width,unsigned int *height,unsigned int *depth,unsigned int *components,
					   float *scalex,float *scaley,float *scalez)
{
	unsigned char *data,*ptr;
	unsigned int bytes,numc;

	int version=1;

	unsigned char *volume;

	float sx=1.0f,sy=1.0f,sz=1.0f;

	unsigned int len1=0,len2=0,len3=0,len4=0;

	if ((data=readDDSfile(filename,&bytes))==NULL)
		if ((data=readRAWfile(filename,&bytes))==NULL) return(NULL);

	if (bytes<5) return(NULL);

	if ((data=(unsigned char *)realloc(data,bytes+1))==NULL) ERRORMSG();
	data[bytes]='\0';

	if (strncmp((char *)data,"PVM\n",4)!=0)
	{
		if (strncmp((char *)data,"PVM2\n",5)==0) version=2;
		else if (strncmp((char *)data,"PVM3\n",5)==0) version=3;
		else return(NULL);

		ptr=&data[5];
		if (sscanf((char *)ptr,"%d %d %d\n%g %g %g\n",width,height,depth,&sx,&sy,&sz)!=6) ERRORMSG();
		if (*width<1 || *height<1 || *depth<1 || sx<=0.0f || sy<=0.0f || sz<=0.0f) ERRORMSG();
		ptr=(unsigned char *)strchr((char *)ptr,'\n')+1;
	}
	else
	{
		ptr=&data[4];
		while (*ptr=='#')
			while (*ptr++!='\n');

		if (sscanf((char *)ptr,"%d %d %d\n",width,height,depth)!=3) ERRORMSG();
		if (*width<1 || *height<1 || *depth<1) ERRORMSG();
	}

	if (scalex!=NULL && scaley!=NULL && scalez!=NULL)
	{
		*scalex=sx;
		*scaley=sy;
		*scalez=sz;
	}

	ptr=(unsigned char *)strchr((char *)ptr,'\n')+1;
	if (sscanf((char *)ptr,"%d\n",&numc)!=1) ERRORMSG();
	if (numc<1) ERRORMSG();

	if (components!=NULL) *components=numc;
	else if (numc!=1) ERRORMSG();

	ptr=(unsigned char *)strchr((char *)ptr,'\n')+1;
	if (version==3) len1=strlen((char *)(ptr+(*width)*(*height)*(*depth)*numc))+1;
	if (version==3) len2=strlen((char *)(ptr+(*width)*(*height)*(*depth)*numc+len1))+1;
	if (version==3) len3=strlen((char *)(ptr+(*width)*(*height)*(*depth)*numc+len1+len2))+1;
	if (version==3) len4=strlen((char *)(ptr+(*width)*(*height)*(*depth)*numc+len1+len2+len3))+1;
	if ((volume=(unsigned char *)malloc((*width)*(*height)*(*depth)*numc))==NULL) ERRORMSG();
	if (data+bytes!=ptr+(*width)*(*height)*(*depth)*numc+len1+len2+len3+len4) ERRORMSG();

	//<<aqui ya se tiene la data del raw>>
	memcpy(volume,ptr,(*width)*(*height)*(*depth)*numc);
	free(data);

	return(volume);
}

void DDS_interleave(unsigned char *data,unsigned int bytes,unsigned int skip,unsigned int block=0)
{DDS_deinterleave(data,bytes,skip,block,true);}