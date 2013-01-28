#include "Modelo.h"

void min(glm::vec3* p,float* aux){
	if ((*p)[0]>aux[0])
		(*p)[0]=aux[0];
	if ((*p)[1]>aux[1])
		(*p)[1]=aux[1];
	if ((*p)[2]>aux[2])
		(*p)[2]=aux[2];
}

void max(glm::vec3* p,float* aux){
	if ((*p)[0]<aux[0])
		(*p)[0]=aux[0];
	if ((*p)[1]<aux[1])
		(*p)[1]=aux[1];
	if ((*p)[2]<aux[2])
		(*p)[2]=aux[2];
}

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
	glm::vec3 x = (this->minp+this->maxp);
	x/=2.0;
	glm::vec3 y = (this->maxp-this->minp);
	float d = max(max(y.x,y.y),y.z);
	glm::vec3 s(2.0/d,2.0/d,2.0/d);
	glm::vec3 r(0.0,1.0,0.0);
	centro = glm::rotate(centro,180.f,r);
	centro = glm::scale(centro,s);
	centro = glm::translate(centro,-x);
}

void Modelo::cargarModelo(const char* file){
	size_t ver[3], norv[3], texv[3];
	size_t asd=0,p=0,t=0,n=0;
	float aux[4],s=0.0;aux[3]=1.0;
	string str, mtl;
	char strs[256];
	ifstream f (file, ifstream::in);
	minp = glm::vec3(FLT_MAX,FLT_MAX,FLT_MAX);
	maxp = glm::vec3(FLT_MIN,FLT_MIN,FLT_MIN);
	vector<glm::vec4*> pos;
	vector<glm::vec3*> nor;
	vector<glm::vec2*> tex;

	if (!f.is_open()){ puts("Error al abrir el archivo obj , saliendo del programa..."); exit(0);}
	while (!f.eof())
	{
		f>>str;
		if (str!="#")
		{
			if (str=="v")
			{
				f>>aux[0]>>aux[1]>>aux[2];
				pos.push_back(new glm::vec4(aux[0],aux[1],aux[2],aux[3]));
				++p;
				min(&minp,aux);
				max(&maxp,aux);
			}
			if (str=="vt"){
				f>>aux[0]>>aux[1];
				tex.push_back(new glm::vec2(aux[0],aux[1]));
				++t;
			}
			if(str=="vn"){
				f>>aux[0]>>aux[1]>>aux[2];
				nor.push_back(new glm::vec3(aux[0],aux[1],aux[2]));
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
				/*f>>str;
				mtl="files/"+str;
				mtlFileRead((char*)mtl.c_str());
				s=-1.0;*/
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
	glm::vec4 v4;
	glm::vec3 v3;
	glm::vec2 p2;
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

			buffer[x++]=v4.x;buffer[x++]=v4.y;buffer[x++]=v4.z;buffer[x++]=v4.w;

			buffer[x++]=v3.x;buffer[x++]=v3.y;buffer[x++]=v3.z;


			buffer[x++]=p2.x;buffer[x++]=p2.y;
			buffer[x++]=tr.s;

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
				/*glGenTextures(1, &tex);
				glBindTexture(GL_TEXTURE_2D, tex);
				f>>str;
				string a = "files/"+str;
				bool x=loadTexture((char*)a.c_str(),GL_LINEAR,GL_NEAREST,GL_REPEAT);
				texturas.push_back(tex);
				glBindTexture(GL_TEXTURE_2D, 0);*/
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