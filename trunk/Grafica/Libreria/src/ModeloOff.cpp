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
	vert.clear();
	aristas.clear();
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
	arista *a;

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


		for (size_t i = 0; i < nTrian; i++){
#pragma region

			t=new triangulo;
			f>>asd>>t->vertices[0]>>t->vertices[1]>>t->vertices[2];
			vert[t->vertices[0]]->trians.push_back(faces.size());
			vert[t->vertices[1]]->trians.push_back(faces.size());
			vert[t->vertices[2]]->trians.push_back(faces.size());
			t->activo=true;
			calcularNormal(t);
			faces.push_back(t);

#pragma endregion cargo un triangulo

#pragma region

			a = new arista;
			a->a=(int)t->vertices[0];
			a->b=(int)t->vertices[1];
			a->t1=(int)faces.size()-1;
			agregarArista(a,faces.size()-1);//arista 1
			a = new arista;
			a->a=(int)t->vertices[1];
			a->b=(int)t->vertices[2];
			a->t1=(int)faces.size()-1;
			agregarArista(a,faces.size()-1);//arista 2
			a = new arista;
			a->a=(int)t->vertices[2];
			a->b=(int)t->vertices[0];
			a->t1=(int)faces.size()-1;
			agregarArista(a,faces.size()-1);//arista 3

#pragma endregion agrego las aristas del triangulo

		}

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

Vec3D ModeloOff::calcularNormal(Punto3D a, Punto3D b, Punto3D c){
	Vec3D u(a,b);
	Vec3D v(a,c);
	u=u^v;
	u.Normalizar();
	return u;
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

		if (!t->activo) continue;

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
	double a1=0.0, a2=0.0, a3=0.0, am=10000.0;
	arista *b;
	int c=-1;

	double offSetAngle = 5;

	double unbral = (PI * offSetAngle) / 180.0;

	vector<double> angulos;
	vector<int>::iterator i;
	Vec3D n;
	triangulo *tr;

	Punto3D medio;
	bool s[3]={false,false,false};//true ese no va

	vector<arista*>::iterator mejor;


	for (ar = aristas.begin(); ar < aristas.end(); ar++){
		b=*ar;
		a1=0.0; a2=0.0; a3=0.0;

		if (!vert[b->a]->activo || !vert[b->b]->activo) continue;
		angulos.clear();

		//for de los triangulos de a
		for (i = vert[b->a]->trians.begin(); i < vert[b->a]->trians.end(); i++)
		{
			if (*i == b->t1 || *i == b->t2 ) continue;

			tr=faces[*i];
			//calcular nueva normal para cada triangulo

			angulos.push_back(0);//como b no tiene interaccion con este triangulo lo coloco tal cual

			n= calcularNormal(//el vertice a se sustituido por b
				(tr->vertices[0]==b->a)?vert[b->b]->posicion:vert[tr->vertices[0]]->posicion,
				(tr->vertices[1]==b->a)?vert[b->b]->posicion:vert[tr->vertices[1]]->posicion,
				(tr->vertices[2]==b->a)?vert[b->b]->posicion:vert[tr->vertices[2]]->posicion);
			angulos.push_back(calcularAngulo(n,tr->normal));



			//calculo el punto medio
			medio = (vert[b->a]->posicion - vert[b->b]->posicion)/2;
			medio+=vert[b->a]->posicion;
			n=	calcularNormal(//el vertice a se sustituido por el medio
				(tr->vertices[0]==b->a)?medio:vert[tr->vertices[0]]->posicion,
				(tr->vertices[1]==b->a)?medio:vert[tr->vertices[1]]->posicion,
				(tr->vertices[2]==b->a)?medio:vert[tr->vertices[2]]->posicion);
			angulos.push_back(calcularAngulo(n,tr->normal));
			//fin calculo
		}

		//for de los triangulos de b
		for (i = vert[b->b]->trians.begin(); i < vert[b->b]->trians.end(); i++)
		{
			if (*i == b->t1 || *i == b->t2 ) continue;

			tr=faces[*i];
			//calcular nueva normal para cada triangulo

			n = calcularNormal(//el vertice b se sustituido por a
				(tr->vertices[0]==b->b)?vert[b->a]->posicion:vert[tr->vertices[0]]->posicion,
				(tr->vertices[1]==b->b)?vert[b->a]->posicion:vert[tr->vertices[1]]->posicion,
				(tr->vertices[2]==b->b)?vert[b->a]->posicion:vert[tr->vertices[2]]->posicion);
			angulos.push_back(calcularAngulo(n,tr->normal));

			angulos.push_back(0);//como a no tiene interaccion con este triangulo lo coloco tal cual

			//calculo el punto medio
			medio = (vert[b->a]->posicion - vert[b->b]->posicion)/2;
			medio+=vert[b->a]->posicion;
			n = calcularNormal(//el vertice a se sustituido por el medio
				(tr->vertices[0]==b->a)?medio:vert[tr->vertices[0]]->posicion,
				(tr->vertices[1]==b->a)?medio:vert[tr->vertices[1]]->posicion,
				(tr->vertices[2]==b->a)?medio:vert[tr->vertices[2]]->posicion);

			angulos.push_back(calcularAngulo(n,tr->normal));

			//fin calculo
		}

		//calcualar el error
		for (vector<double>::iterator j = angulos.begin(); j < angulos.end(); j++)
		{

			a1+=*j;//acumulo el angulo de los a
			if (*j > unbral) s[0]=true;
			a2+=*(++j);//acumulo el angulo de los b
			if (*j > unbral) s[1]=true;
			a3+=*(++j);//acumulo el angulo del medio
			if (*j > unbral) s[2]=true;
		}

		if (a1<am && !s[0]){
			am = a1;
			c=1;
			mejor=ar;
		}
		if (a2<am && !s[1]){
			am = a2;
			c=2;
			mejor=ar;
		}
		if (a3<am && !s[2]){
			am = a3;
			c=3;
			mejor=ar;
		}
		if (am == 0.0) break;
	}

	//backup

	if (c==-1){return;}

	if (c==1){}

	if (c==2){}

	if (c==3){}

	cout<<vert[(*mejor)->a]->posicion<<vert[(*mejor)->b]->posicion;
}

void ModeloOff::agregarArista(arista *a, int t){

	arista *b;
	if (aristas.empty()){
		aristas.push_back(a);
	}else{
		for (ar = aristas.begin(); ar < aristas.end(); ar++)
		{
			b=*ar;

			if ((b->a == a->a && b->b == a->b) || (b->a == a->b && b->b == a->a)){
				b->t2=t;
				return;
			}else
				continue;
		}
		aristas.push_back(a);
	}
}

double calcularAngulo(Vec3D a, Vec3D b){
	double ma=a.Magnitud(), mb=b.Magnitud();
	double p= a*b;
	return acos(p/(ma*mb));
}