#include "Modelo.h"


Modelo::Modelo(void)
{
}


Modelo::~Modelo(void)
{
	glDeleteBuffers(1,&BufferObject);
	for (size_t i = 0; i < texturas.size(); i++)
	{
		glDeleteTextures(1,&texturas[i]);
	}
	texturas.clear();
}

void Modelo::centrar(){
	Punto3D x = (this->minp+this->maxp)/2;
	Punto3D y = (this->maxp-this->minp);
	float d = max(max(y.coord[0],y.coord[1]),y.coord[2]);

	centro = centro.MatScale(2/d,2/d,2/d)*centro.MatTranslate(-x);
}

void Modelo::cargarModelo(const char* file){
	size_t ver[3], norv[3], texv[3];
	size_t asd=0,p=0,t=0,n=0;
	float aux[4],s=0.0;aux[3]=1.0;
	string str, mtl;
	char strs[256];
	ifstream f (file, ifstream::in);
	minp = Punto3D(FLT_MAX,FLT_MAX,FLT_MAX);
	maxp = Punto3D(FLT_MIN,FLT_MIN,FLT_MIN);
	vector<Vec4D*> pos;
	vector<Vec3D*> nor;
	vector<Punto2D*> tex;

	if (!f.is_open()){ puts("Error al abrir el archivo obj , saliendo del programa..."); exit(0);}
	while (!f.eof())
	{
		f>>str;
		if (str!="#")
		{
			if (str=="v")
			{
				f>>aux[0]>>aux[1]>>aux[2];
				pos.push_back(new Vec4D(aux));
				++p;
				minp=minp.minp(aux);
				maxp=maxp.maxp(aux);
			}
			if (str=="vt"){
				f>>aux[0]>>aux[1];
				tex.push_back(new Punto2D(aux[0],aux[1]));
				++t;
			}
			if(str=="vn"){
				f>>aux[0]>>aux[1]>>aux[2];
				nor.push_back(new Vec3D(aux));
				++n;
			}
			if (str == "o"){
				f>>str;
			}
			if(str=="f"){
				f.getline(strs,256);
				string asdf = strs;
				int i =0;
				str = "";
				while (!asdf.empty())
				{
					//f>>str;
					asd = asdf.find_first_of(' ');
					if (asd > 0 ){
						str = asdf.substr(0,asd);
						if (!str.empty()){
							asd=str.find_first_of("/");
							ver[i]=(size_t)atoi((str.substr(0,asd)).c_str());
							if (asd!=string::npos){
								str=str.substr(asd+1);
							}else{
								str.clear();
							}
						}
						if (!str.empty()){
							asd=str.find_first_of("/");
							if (aux==0){
								// v//n
								norv[i]=(size_t)atoi((str.substr(1)).c_str());
								str.clear();
							}else{
								// v/t
								texv[i]=(size_t)atoi((str.substr(0,asd)).c_str());
								if (asd!=string::npos)
									str=str.substr(asd+1);
								else
									str.clear();
							}
						}
						if (!str.empty()){
							norv[i]=(size_t)atoi((str.substr(0)).c_str());
						}
						i++;
					}
					asd = asdf.find_first_of(' ');
					if (asd==string::npos)
						break;
					else
						asdf = asdf.substr(asdf.find_first_of(" ")+1);

				}
				añadirFace(ver,norv,texv,s);
			}
			if (str=="mtllib"){
				f>>str;
				mtl="files/"+str;
				mtlFileRead((char*)mtl.c_str());
				s=-1.0;
			}
			if (str=="usemtl"){
				f>>mtl;
				s+=1.0;
			}
		}else{
			f.getline(strs,256);
		}
	}

	f.close();
	Vec4D v4;
	Vec3D v3;
	Punto2D p2;
	triangulo tr;
	size_t x;
	total=faces.size();
	datasize=(total*(30))*sizeof(float);
	GLfloat* buffer = new GLfloat[total*(30)];

	for (size_t j = 0; j < total; j++)
	{
		x=j*30;
		tr = *faces[j];
		//vertice 1
		for (int k = 0; k < 3; k++)
		{
			v4=*pos[tr.ver[k]];
			v3=*nor[tr.nor[k]];
			p2=*tex[tr.tex[k]];
			memcpy(buffer+x,v4.coord,sizeof(v4.coord));
			memcpy(buffer+x+4,v3.coord,sizeof(v3.coord));
			buffer[x+7]=(float)p2.coord[0];buffer[x+8]=(float)p2.coord[1];
			buffer[x+9]=tr.s;
			x+=10;
		} 
	}
	
	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferData(GL_ARRAY_BUFFER, datasize,buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete buffer;
	faces.clear();

	pos.clear();
	nor.clear();
	tex.clear();

	centrar();

}

void Modelo::mtlFileRead(char *fn){

	ifstream f (fn, ifstream::in);
	float coe;
	string name, str;
	char strs[256];
	float c[4];
	if (!f.is_open()){ puts("Error al abrir el archivo mtl , saliendo del programa..."); exit(0);}
	GLuint tex;
	while (!f.eof())
	{
		f>>str;
		if (str!="#"){
			if (str=="newmtl"){

			}
			if (str=="Ka"){
				f>>c[0]>>c[1]>>c[2];
				//m->setAmbiet(c);
			}
			if (str=="Kd"){
				f>>c[0]>>c[1]>>c[2];
				//m->setDiffuse(c);
			}
			if (str=="Ks"){
				f>>c[0]>>c[1]>>c[2];
				//m->setSpecular(c);
			}
			if (str=="Ns"){
				f>>coe;
				//m->setCoef(coe);
			}
			if (str=="d" || str=="Tr"){
				f>>coe;
				//m->setTransparency(coe);
			}
			if (str=="map_Kd"){

				//if (m!=NULL){
				//m->map_Kd=(GLuint*)calloc(1,sizeof(GLuint));
				glGenTextures(1, &tex);
				glBindTexture(GL_TEXTURE_2D, tex);
				f>>str;
				string a = "files/"+str;
				bool x=loadTexture((char*)a.c_str(),GL_LINEAR,GL_NEAREST,GL_REPEAT);
				texturas.push_back(tex);
				glBindTexture(GL_TEXTURE_2D, 0);
				//}
			}
		}else
			f.getline(strs,256);

	}
	f.close();
}

void Modelo::añadirFace(size_t* v,size_t* n,size_t* t,float s){
	triangulo* a = new triangulo();
	a->ver[0]=v[0]-1;
	a->ver[1]=v[1]-1;
	a->ver[2]=v[2]-1;
	a->nor[0]=n[0]-1;
	a->nor[1]=n[1]-1;
	a->nor[2]=n[2]-1;
	a->tex[0]=t[0]-1;
	a->tex[1]=t[1]-1;
	a->tex[2]=t[2]-1;
	a->s = s;
	faces.push_back(a);
}

bool Modelo::loadTexture (char *path ,GLenum minFilter, GLenum magFilter, GLenum wrapMode){

	
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	
	FIBITMAP *dib(0);
	
	BYTE* bits(0);
	
	GLuint width(0), height(0);
	
	
	fif = FreeImage_GetFileType(path, 0);
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(path);

	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, path);

	if	(dib != NULL){

		dib = FreeImage_ConvertTo32Bits(dib);
		if (dib != NULL){
			bits = FreeImage_GetBits(dib);
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
					GL_BGRA_EXT, GL_UNSIGNED_BYTE, bits );
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapMode);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapMode);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magFilter);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minFilter);		}
	}

	FreeImage_Unload(dib);
	return true;
}