#include "..\include\mod\ModeloOff.h"

ModeloOff::ModeloOff(void)
{
	minp = Punto3D(FLT_MAX,FLT_MAX,FLT_MAX);
	maxp = Punto3D(FLT_MIN,FLT_MIN,FLT_MIN);
	tra =0;
	vea=0;
}

ModeloOff::~ModeloOff(void)
{
	glDeleteBuffers(1,&BufferObject);
	delete buffer;
	triangulos.clear();
	vertices.clear();
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

	triangulo *t;
	vertice *v;
	arista *a;

	string str;
	ifstream f (file, ifstream::in);

#pragma region
	if (total>0){
		glDeleteBuffers(1,&BufferObject);
		delete buffer;
		triangulos.clear();
		vertices.clear();
		aristas.clear();
	}
#pragma endregion limpiar datos

	if (!f.is_open()){ puts("Error al abrir el archivo off , saliendo del programa..."); cin.get();exit(0); }
	f>>str;
	if (str=="OFF"){

		f>>nVert>>nTrian>>asd;


#pragma region
		for (int i = 0; i < nVert; i++){
			v=new vertice;
			f>>v->posicion.coord[0]>>v->posicion.coord[1]>>v->posicion.coord[2];
			minp.min(v->posicion.coord);
			maxp.max(v->posicion.coord);
			vertices.push_back(v);
		}
#pragma endregion cargar los vertices

#pragma region
		for (int i = 0; i < nTrian; i++){
			t=new triangulo;
			f>>asd>>t->vertices[0]>>t->vertices[1]>>t->vertices[2];
			vertices[t->vertices[0]]->trians.push_back(i);
			vertices[t->vertices[1]]->trians.push_back(i);
			vertices[t->vertices[2]]->trians.push_back(i);
			t->activo=true;
			calcularNormal(t);
			triangulos.push_back(t);
#pragma region
			a = new arista;
			a->a=(int)t->vertices[0];
			a->b=(int)t->vertices[1];
			a->t1=(int)triangulos.size()-1;
			a->t2=-1;
			a->activo=true;
			t->aristas[0]=agregarArista(a,triangulos.size()-1);//arista 1
			a = new arista;
			a->a=(int)t->vertices[1];
			a->b=(int)t->vertices[2];
			a->t1=(int)triangulos.size()-1;
			a->t2=-1;
			a->activo=true;
			t->aristas[1]=agregarArista(a,triangulos.size()-1);//arista 2
			a = new arista;
			a->a=(int)t->vertices[2];
			a->b=(int)t->vertices[0];
			a->t1=(int)triangulos.size()-1;
			a->t2=-1;
			a->activo=true;
			t->aristas[2]=agregarArista(a,triangulos.size()-1);//arista 3

#pragma endregion agrego las aristas del triangulo

		}
#pragma endregion cargo un triangulo

	}else{
		puts("El archivo no es un off");
		f.close();
		return;
	}
	f.close();


	for (int i = 0; i < nVert; i++)
	{
		v=vertices[i];
		v->normal/=((float)v->trians.size());
	}

	total=nTrian;
	buffer = new GLfloat[nTrian*(18)];
	datasize=(nTrian*(18))*sizeof(float);

	glGenBuffers(1, &BufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_STREAM_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	updateBuffer(GOURAUD);
	centrar();
	tra=nTrian;vea=nVert;
}

void ModeloOff::calcularNormal(triangulo* t){
	Vec3D u(vertices[t->vertices[0]]->posicion,vertices[t->vertices[1]]->posicion);
	Vec3D v(vertices[t->vertices[0]]->posicion,vertices[t->vertices[2]]->posicion);
	t->normal=u^v;
	t->normal.Normalizar();

	vertices[t->vertices[0]]->normal+=t->normal;
	vertices[t->vertices[1]]->normal+=t->normal;
	vertices[t->vertices[2]]->normal+=t->normal;
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

	asd=0;
	for (int i = 0; i < triangulos.size(); i++)
	{

		t = triangulos[i];

		if (!t->activo) continue;

		for (int k = 0; k < 3; k++)
		{
			p3=vertices[t->vertices[k]]->posicion;

			v3=(x)? vertices[t->vertices[k]]->normal:t->normal;

			memcpy(buffer+asd,p3.coord,sizeof(p3.coord));
			memcpy(buffer+asd+3,v3.coord,sizeof(v3.coord));

			asd+=6;
		} 
	}
	//	datasize=(total*(18))*sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, BufferObject);
	glBufferSubDataARB(GL_ARRAY_BUFFER,0, datasize,buffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModeloOff::colapse(double offSetAngle){
	double a1=0.0, a2=0.0, a3=0.0, am=10000.0;
	arista *b;
	int c=-1;

	double unbral = (PI * offSetAngle) / 180.0;

	vector<double> angulos;
	vector<int>::iterator i;
	Vec3D n;
	triangulo *tr;

	Punto3D medio;
	bool s[3]={false,false,false};//true ese no va

	int mejor;

#pragma region
	for (int t = 0; t < aristas.size(); t++){
		b=aristas[t];
		a1=0.0; a2=0.0; a3=0.0;
		s[0]=false;s[1]=false;s[2]=false;
		if (!b->activo) continue;
		//if(!faces[b->t1]->activo && !faces[b->t2]->activo) continue;
		angulos.clear();

		//for de los triangulos de a
#pragma region
		for (i = vertices[b->a]->trians.begin(); i < vertices[b->a]->trians.end(); i++)
		{
			if (*i == b->t1 || *i == b->t2 ) continue;
			//if (!faces[*i]->activo) continue;
			tr=triangulos[*i];
			//calcular nueva normal para cada triangulo

			angulos.push_back(0);//como b no tiene interaccion con este triangulo lo coloco tal cual

			n= calcularNormal(//el vertice a se sustituido por b
				(tr->vertices[0]==b->a)?vertices[b->b]->posicion:vertices[tr->vertices[0]]->posicion,
				(tr->vertices[1]==b->a)?vertices[b->b]->posicion:vertices[tr->vertices[1]]->posicion,
				(tr->vertices[2]==b->a)?vertices[b->b]->posicion:vertices[tr->vertices[2]]->posicion);
			angulos.push_back(calcularAngulo(n,tr->normal));
			//calculo el punto medio

			//fin calculo
		}
#pragma endregion calculo el angulo de los triangulos de a

		//for de los triangulos de b
#pragma region
		for (i = vertices[b->b]->trians.begin(); i < vertices[b->b]->trians.end(); i++)
		{
			if (*i == b->t1 || *i == b->t2 ) continue;
			//if (!triangulos[*i]->activo) continue;
			tr=triangulos[*i];
			//calcular nueva normal para cada triangulo

			n = calcularNormal(//el vertice b se sustituido por a
				(tr->vertices[0]==b->b)?vertices[b->a]->posicion:vertices[tr->vertices[0]]->posicion,
				(tr->vertices[1]==b->b)?vertices[b->a]->posicion:vertices[tr->vertices[1]]->posicion,
				(tr->vertices[2]==b->b)?vertices[b->a]->posicion:vertices[tr->vertices[2]]->posicion);
			angulos.push_back(calcularAngulo(n,tr->normal));
			angulos.push_back(0);//como a no tiene interaccion con este triangulo lo coloco tal cual

			//fin calculo
		}
#pragma endregion calculo el angulo de los triangulos de b

		//calcualar el error
		for (vector<double>::iterator j = angulos.begin(); j < angulos.end(); j++)
		{
			a1+=*j;//acumulo el angulo de los a
			if (*j > unbral) s[0]=true;
			a2+=*(++j);//acumulo el angulo de los b
			if (*j > unbral) s[1]=true;
		}

		if (a1<am && !s[0]){
			am = a1;
			c=1;
			mejor=t;
		}
		if (a2<am && !s[1]){
			am = t;
			c=2;
			mejor=t;
		}
		if (am == 0.0) break;
	}
#pragma endregion verificacion de posible colapso

	//backup

	if (c==-1){return;}

	backup ba;
	b=aristas[mejor];
	//ba.b=*mejor;


	cout<<b->a<<" "<<b->b<<" "<<b->t1<<" "<<b->t2<<" "<<c <<" "<<vea<<" "<<tra<<" "<<mejor<<endl;
	if (angulos.size()>2){
		if (triangulos[b->t1]->activo){ total--; tra--;}
		triangulos[b->t1]->activo=false;
		if (triangulos[b->t2]->activo){ total--; tra--;}
		triangulos[b->t2]->activo=false;
		if (vertices[b->b]->activo)	vea--;
		vertices[b->b]->activo=false;
		b->activo=false;
	}else{
		return;
	}

	vector<int> asdf;
	if (c==1){
		//nada
	}
	if (c==2){
		vertices[b->a]->posicion = vertices[b->b]->posicion;
	}

	for(i = vertices[b->a]->trians.begin(); i < vertices[b->a]->trians.end(); i++){
		if (*i == b->t1 || *i == b->t2 ) continue;
		if (!triangulos[*i]->activo) continue;
		asdf.push_back(*i);
	}

	for(i = vertices[b->b]->trians.begin(); i < vertices[b->b]->trians.end(); i++){
		if (*i == b->t1 || *i == b->t2 ) continue;
		//if (!vertices[*i]->activo) continue;

		asdf.push_back(*i);
		if (triangulos[*i]->vertices[0] == b->b){
			triangulos[*i]->vertices[0] = b->a;
			if (aristas[triangulos[*i]->aristas[0]]->a == b->b){
				aristas[triangulos[*i]->aristas[0]]->a = b->a;
			}else{
				aristas[triangulos[*i]->aristas[0]]->b = b->a;
			}
		}
		if (triangulos[*i]->vertices[1] == b->b){
			triangulos[*i]->vertices[1] = b->a;
			if (aristas[triangulos[*i]->aristas[1]]->a == b->b){
				aristas[triangulos[*i]->aristas[1]]->a = b->a;
			}else{
				aristas[triangulos[*i]->aristas[1]]->b = b->a;
			}
		}
		if (triangulos[*i]->vertices[2] == b->b){
			triangulos[*i]->vertices[2] = b->a;
			if (aristas[triangulos[*i]->aristas[2]]->a == b->b){
				aristas[triangulos[*i]->aristas[2]]->a = b->a;
			}else{
				aristas[triangulos[*i]->aristas[2]]->b = b->a;
			}
		}
	}
	
	int aux,aux2;
/*	if (aristas[faces[b->t1]->aristas[0]]==b)
	{
	aux = faces[b->t1]->aristas[1];
	aux2= faces[b->t1]->aristas[2];
	if (aristas[aux]->a==b->b || aristas[aux]->b==b->b){//arista duplicada
	if (aristas[aux2]->t1==b->t1){
	aristas[aux2]->t1= aristas[aux]->t2;
	}else
	aristas[aux2]->t2= aristas[aux]->t1;
	}else{
	if (aristas[aux2]->t1==b->t1)
	aristas[aux]->t1= aristas[aux2]->t2;
	else
	aristas[aux]->t2= aristas[aux2]->t1;
	}
	}
	if (aristas[faces[b->t1]->aristas[1]]==b)
	{
	aux = faces[b->t1]->aristas[0];
	aux2= faces[b->t1]->aristas[2];
	if (aristas[aux]->a==b->b || aristas[aux]->b==b->b){//arista duplicada
	if (aristas[aux2]->t1==b->t1)
	aristas[aux2]->t1= aristas[aux]->t2;
	else
	aristas[aux2]->t2= aristas[aux]->t1;
	}else{
	if (aristas[aux2]->t1==b->t1)
	aristas[aux]->t1= aristas[aux2]->t2;
	else
	aristas[aux]->t2= aristas[aux2]->t1;
	}
	}
	if (aristas[faces[b->t1]->aristas[2]]==b)
	{
	aux = faces[b->t1]->aristas[0];
	aux2= faces[b->t1]->aristas[1];
	if (aristas[aux]->a==b->b || aristas[aux]->b==b->b){//arista duplicada
	if (aristas[aux2]->t1==b->t1)
	aristas[aux2]->t1= aristas[aux]->t2;
	else
	aristas[aux2]->t2= aristas[aux]->t1;
	}else{
	if (aristas[aux2]->t1==b->t1)
	aristas[aux]->t1= aristas[aux2]->t2;
	else
	aristas[aux]->t2= aristas[aux2]->t1;
	}
	}

	if (aristas[faces[b->t2]->aristas[0]]==b)
	{
	aux = faces[b->t2]->aristas[1];
	aux2= faces[b->t2]->aristas[2];
	if (aristas[aux]->a==b->b || aristas[aux]->b==b->b){//arista duplicada
	if (aristas[aux2]->t1==b->t2)
	aristas[aux2]->t1= aristas[aux]->t2;
	else
	aristas[aux2]->t2= aristas[aux]->t1;
	}else{
	if (aristas[aux2]->t1==b->t2)
	aristas[aux]->t1= aristas[aux2]->t2;
	else
	aristas[aux]->t2= aristas[aux2]->t1;
	}
	}
	if (aristas[faces[b->t2]->aristas[1]]==b)
	{
	aux = faces[b->t2]->aristas[0];
	aux2= faces[b->t2]->aristas[2];
	if (aristas[aux]->a==b->b || aristas[aux]->b==b->b){//arista duplicada
	if (aristas[aux2]->t1==b->t2)
	aristas[aux2]->t1= aristas[aux]->t2;
	else
	aristas[aux2]->t2= aristas[aux]->t1;
	}else{
	if (aristas[aux2]->t1==b->t2)
	aristas[aux]->t1= aristas[aux2]->t2;
	else
	aristas[aux]->t2= aristas[aux2]->t1;
	}
	}
	if (aristas[faces[b->t2]->aristas[2]]==b)
	{
	aux = faces[b->t2]->aristas[0];
	aux2= faces[b->t2]->aristas[1];
	if (aristas[aux]->a==b->b || aristas[aux]->b==b->b){//arista duplicada
	if (aristas[aux2]->t1==b->t2)
	aristas[aux2]->t1= aristas[aux]->t2;
	else
	aristas[aux2]->t2= aristas[aux]->t1;
	}else{
	if (aristas[aux2]->t1==b->t2)
	aristas[aux]->t1= aristas[aux2]->t2;
	else
	aristas[aux]->t2= aristas[aux2]->t1;
	}
	}
	

	vertices[b->a]->trians = asdf;


	*/
}

int ModeloOff::agregarArista(arista *a, int t){

	arista *b;int i=0;
	if (aristas.empty()){
		aristas.push_back(a);
		return 0;
	}else{
		for (i = 0; i < aristas.size(); i++)
		{
			b=aristas[i];

			if ((b->a == a->a && b->b == a->b) || (b->a == a->b && b->b == a->a)){
				b->t2=t;
				return i;
			}else
				continue;
		}
		aristas.push_back(a);
		return i;
	}
	return 0;
}

int ModeloOff::buscarArista(int a, int b){
	int i=0;
	arista *p;
	for (vector<arista*>::iterator at = aristas.begin(); at < aristas.end(); at++,i++)
	{
		p=*at;

		if ((p->a == a && p->b == b) || (p->a == b && p->b == a)){
			return i;
		}else
			continue;
	}
	return -1;
}

double calcularAngulo(Vec3D a, Vec3D b){
	double ma=a.Magnitud(), mb=b.Magnitud();
	double p= a*b;
	return (ma*mb<=0.0)?0.0:acos(p/(ma*mb));
}