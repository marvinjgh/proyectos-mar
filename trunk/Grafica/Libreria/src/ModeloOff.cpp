#include "..\include\mod\ModeloOff.h"

ModeloOff::ModeloOff(void)
{
	minp = Punto3D(FLT_MAX,FLT_MAX,FLT_MAX);
	maxp = Punto3D(FLT_MIN,FLT_MIN,FLT_MIN);
}

ModeloOff::~ModeloOff(void)
{
	glDeleteBuffers(1,&BufferObject);
	delete buffer;
	faces.clear();
}

void ModeloOff::centrar(){
	Punto3D x = (this->minp+this->maxp)/2;
	Punto3D y = (this->maxp-this->minp);
	float d = max(max(y.coord[0],y.coord[1]),y.coord[2]);
	centro = MatScale(2/d,2/d,2/d)*MatTranslate(-x);
}

void ModeloOff::cargarModelo(const char* file){
	size_t nVert=0, nTrian=0, asd=0;
	Vec3D v3;
	Punto3D p3;
	triangulo *t;
	vertice *v;

	string str;
	ifstream f (file, ifstream::in);

	if (!f.is_open()){ puts("Error al abrir el archivo off , saliendo del programa..."); cin.get();exit(0); }
	f>>str;
	if (str=="OFF"){
		#pragma region
		f>>nVert>>nTrian>>asd;
		for (size_t i = 0; i < nVert; i++){
			v=new vertice;
			f>>v->posicion.coord[0]>>v->posicion.coord[1]>>v->posicion.coord[2];
			minp=minp.minp(v->posicion.coord);
			maxp=maxp.maxp(v->posicion.coord);
			vert.push_back(v);
		}
		#pragma endregion cargar los vertices

		#pragma region
		for (size_t i = 0; i < nTrian; i++){
			t=new triangulo;
			f>>asd>>t->vertices[0]>>t->vertices[1]>>t->vertices[2];
			vert[t->vertices[0]]->trians.push_back(faces.size());
			vert[t->vertices[1]]->trians.push_back(faces.size());
			vert[t->vertices[2]]->trians.push_back(faces.size());
			t->activo=true;
			calcularNormal(t);
			faces.push_back(t);
		}
		#pragma endregion cargar los triangulos
	}else{
		puts("El archivo no es un off");
		f.close();
		return;
	}
	f.close();

	vector<vertice*>::iterator i;
	for (i = vert.begin(); i != vert.end(); i++)
	{
		(*i)->normal/((float)(*i)->trians.size());
	}

	total=faces.size();
	buffer = new GLfloat[total*(18)];
	datasize=(total*(18))*sizeof(float);
	

	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_STREAM_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	updateBuffer(GOURAUD);
	centrar();

}

void ModeloOff::calcularNormal(triangulo* t){
	Vec3D u(vert[t->vertices[0]]->posicion,vert[t->vertices[1]]->posicion);
	Vec3D v(vert[t->vertices[0]]->posicion,vert[t->vertices[2]]->posicion);
	t->normal=u^v;
	t->normal.Normalizar();

	vert[t->vertices[0]]->normal+=t->normal;
	vert[t->vertices[1]]->normal+=t->normal;
	vert[t->vertices[2]]->normal+=t->normal;
}

void ModeloOff::updateBuffer(Sombreado x){
	int asd;
	triangulo *t;
	Vec3D v3;
	Punto3D p3;
	

	for (int j = 0; j < total; j++)
	{
		asd=j*18;
		t = faces[j];

		for (int k = 0; k < 3; k++)
		{
			p3=vert[t->vertices[k]]->posicion;
			
			v3=(x)? vert[t->vertices[k]]->normal:t->normal;
			
			memcpy(buffer+asd,p3.coord,sizeof(p3.coord));
			memcpy(buffer+asd+3,v3.coord,sizeof(v3.coord));

			asd+=6;
		} 
	}

	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferSubDataARB(GL_ARRAY_BUFFER,0, datasize,buffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModeloOff::colapse(){

}