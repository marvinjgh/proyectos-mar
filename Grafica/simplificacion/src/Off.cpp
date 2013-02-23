#include "Off.h"

Off::Off(void)
{
}

//destructor
Off::~Off(void)
{
}

size_t Off::findArista(Arista a){
	if (aristas.empty())
		return -1;
	FOR(i,aristas.size()){
		Arista b = aristas.at(i);
		if (a == b){
			return i;
		}else
			continue;
	}
	return -1;
}

void Off::cargarModelo(char* file){
	size_t nVert=0, nTrian=0, asd=0;

	float aux[]={0.0f,0.0f,0.0f,1.0f};
	size_t ver[]={0,0,0}, asdf=0;

	string str;
	ifstream f (file, ifstream::in);

	if (!f.is_open()){ puts("Error al abrir el archivo off , saliendo del programa...");}

#pragma region
	if (nTri>0){
		glDeleteBuffers(1,&BufferObject);
		triangulos.clear();
		vertices.clear();
		aristas.clear();
		colisiones.clear();
		nVer = 0; nTri = 0;
		minp=glm::vec3();
		maxp=glm::vec3();
	}
#pragma endregion limpiar los vectores

	f>>str;
	if (str=="OFF"){

		f>>nVert>>nTrian>>asd;
#pragma region
		//for (int i = 0; i < nVert; i++){
		FOR(i,nVert){
			f>>aux[0]>>aux[1]>>aux[2];
			min(minp,aux);
			max(maxp,aux);
			vertices.push_back(Vertice(aux));
		}
#pragma endregion cargar los vertices
		FOR(i,nTrian){
#pragma region
			f>>asd>>ver[0]>>ver[1]>>ver[2];
			vertices[ver[0]].agregarTriangulo(i);
			vertices[ver[1]].agregarTriangulo(i);
			vertices[ver[2]].agregarTriangulo(i);
			triangulos.push_back(Triangulo(ver[0],ver[1],ver[2]));

#pragma endregion cargo un triangulo
#pragma region
			asdf=findArista(Arista(ver[0],ver[1]));
			if (asdf==-1){
				asdf = aristas.size();
				aristas.push_back(Arista(ver[0],ver[1]));
				aristas.at(asdf).setTrian_1(i);
			}else{
				aristas.at(asdf).setTrian_2(i);
			}
			triangulos.at(i).setArista(0,asdf);

			asdf=findArista(Arista(ver[1],ver[2]));
			if (asdf==-1){
				asdf = aristas.size();
				aristas.push_back(Arista(ver[1],ver[2]));
				aristas.at(asdf).setTrian_1(i);
			}else{
				aristas.at(asdf).setTrian_2(i);
			}
			triangulos.at(i).setArista(1,asdf);

			asdf=findArista(Arista(ver[2],ver[0]));
			if (asdf==-1){
				asdf = aristas.size();
				aristas.push_back(Arista(ver[2],ver[0]));
				aristas.at(asdf).setTrian_1(i);
			}else{
				aristas.at(asdf).setTrian_2(i);
			}
			triangulos.at(i).setArista(2,asdf);

#pragma endregion agrego las aristas del triangulo
#pragma region

			glm::vec3 a(vertices.at(ver[0]).getPosicion());
			glm::vec3 b(vertices.at(ver[1]).getPosicion());
			glm::vec3 c(vertices.at(ver[2]).getPosicion());
			triangulos.at(i).setNormal(glm::normalize(glm::cross(c - a, b - a)));
#pragma endregion normal del triangulo
		}
	}else{
		puts("El archivo no es un off");
		f.close();
		return;
	}
	f.close();

	nVer = nVert;
	nTri = nTrian;
	datasize = (nTrian*(7*3))*sizeof(float);
#pragma region
	FOR(i,nVert){
		Vertice & v = vertices.at(i);
		glm::vec3 n(0.0f);
		FOR(j,v.getSizetrian()){
			n += triangulos.at(v.getTriangulo(j)).getNormal();
		}
		n = glm::normalize(n);
		v.setNormal(n);
	}
#pragma endregion normal de los vertices

	glGenBuffersARB(1, &BufferObject);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, BufferObject);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, datasize, 0, GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	centrar();
}

void Off::centrar(){
	glm::vec3 x = (minp+maxp);
	x/=2.0;
	glm::vec3 y = (maxp-minp);
	float d = std::max(std::max(y.x,y.y),y.z);
	glm::vec3 s(2.0/d,2.0/d,2.0/d);
	glm::vec3 r(0.0,1.0,0.0);
	centro = glm::mat4();
	centro = glm::scale(centro,s);
	centro = glm::translate(centro,-x);
}

void Off::updateBuffer(Sombreado x){
	int asd;
	GLfloat* buffer = new GLfloat[nTri*(7*3)];
	asd=0;
	GLsizeiptr datasize = (nTri*(7*3))*sizeof(float);

	FOR(i,triangulos.size()){

		Triangulo & t = triangulos.at(i);

		if (!t.estaActivo()) continue;

		FOR(j,3){

			Vertice & v = vertices.at(t.getVertice(j));

			buffer[asd++]=v.getPosicion().x;
			buffer[asd++]=v.getPosicion().y;
			buffer[asd++]=v.getPosicion().z;
			buffer[asd++]=v.getPosicion().w;
			if (x==GOURAUD){
				buffer[asd++]=v.getNormal().x;
				buffer[asd++]=v.getNormal().y;
				buffer[asd++]=v.getNormal().z;
			}else{
				buffer[asd++]=t.getNormal().x;
				buffer[asd++]=t.getNormal().y;
				buffer[asd++]=t.getNormal().z;
			}

		} 
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, BufferObject);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0, datasize,buffer);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void Off::pintar(size_t n){

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, BufferObject);
	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
	glVertexAttribPointerARB(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*7, 0);
	glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)16);

	glDrawArrays(GL_TRIANGLES,0,nTri*3);

	glDisableVertexAttribArrayARB(0);
	glDisableVertexAttribArrayARB(1);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);

}

bool Off::isLoad(void){
	return nTri>0;
}

void Off::simplificar(float offset){
	double unbral = (PI * offset) / 180.0;
	double mejor_a=100000000;
	vector<double> angulos;vector<size_t> asdf;
	bool s[]={false,false};//true ese no va
	size_t c=-1,mejor;


#pragma region verificacion de posible colapso
	FOR(i,aristas.size()){
		Arista & a = aristas.at(i);
		double a1=0.0, a2=0.0;
		glm::vec3 n(0.0f);
		s[0]=false;s[1]=false;
		if (!a.estaActivo()) continue;
		if (a.getTrian_1()>triangulos.size() || a.getTrian_2()>triangulos.size()) continue;
		//if(!faces[b->t1]->activo && !faces[b->t2]->activo) continue;
		angulos.clear();

		//for de los triangulos de a
#pragma region calculo el angulo de los triangulos de a

		size_t t1=a.getTrian_1(), t2=a.getTrian_2();
		size_t va=a.getVert_a(), vb=a.getVert_b();
		Vertice v0 = vertices.at(va);

		FOR(j, v0.getSizetrian())
		{
			if (v0.getTriangulo(j) == t1 || v0.getTriangulo(j) == t2 ) continue;
			if (!triangulos.at(v0.getTriangulo(j)).estaActivo()) continue;
			Triangulo & tr = triangulos.at(v0.getTriangulo(j));
			angulos.push_back(0);

			//el vertice a se sustituido por b(posicion)
			glm::vec3 na,nb,nc;
			if (tr.getVertice(0) == va){
				na = glm::vec3(vertices.at(vb).getPosicion());
				nb = glm::vec3(vertices.at(tr.getVertice(1)).getPosicion());
				nc = glm::vec3(vertices.at(tr.getVertice(2)).getPosicion());
			}else{
				na = glm::vec3(vertices.at(tr.getVertice(0)).getPosicion());
				if (tr.getVertice(1) == va){
					nb = glm::vec3(vertices.at(vb).getPosicion());
					nc = glm::vec3(vertices.at(tr.getVertice(2)).getPosicion());
				}else{
					nb = glm::vec3(vertices.at(tr.getVertice(1)).getPosicion());
					nc = glm::vec3(vertices.at(vb).getPosicion());
				}
			}
			n = glm::normalize(glm::cross(nc - na, nb - na));
			angulos.push_back(glm::angle(n, tr.getNormal()));
		}
#pragma endregion 

		//for de los triangulos de b
#pragma region calculo el angulo de los triangulos de b
		v0 = vertices.at(vb);
		FOR(j, v0.getSizetrian())
		{
			if (v0.getTriangulo(j) == t1 || v0.getTriangulo(j) == t2 ) continue;
			if (!triangulos.at(v0.getTriangulo(j)).estaActivo()) continue;
			Triangulo & tr = triangulos.at(v0.getTriangulo(j));

			//el vertice b se sustituido por a
			glm::vec3 na,nb,nc;
			if (tr.getVertice(0) == vb){
				na = glm::vec3(vertices.at(va).getPosicion());
				nb = glm::vec3(vertices.at(tr.getVertice(1)).getPosicion());
				nc = glm::vec3(vertices.at(tr.getVertice(2)).getPosicion());
			}else{
				na = glm::vec3(vertices.at(tr.getVertice(0)).getPosicion());
				if (tr.getVertice(1) == vb){
					nb = glm::vec3(vertices.at(va).getPosicion());
					nc = glm::vec3(vertices.at(tr.getVertice(2)).getPosicion());
				}else{
					nb = glm::vec3(vertices.at(tr.getVertice(1)).getPosicion());
					nc = glm::vec3(vertices.at(va).getPosicion());
				}
			}
			n = glm::normalize(glm::cross(nc - na, nb - na));
			angulos.push_back(glm::angle(n, tr.getNormal()));

			angulos.push_back(0);
		}
#pragma endregion 

		//calcualar el error
		FOR(j,angulos.size()){
			if (j%2==0){//par
				a1+=angulos.at(j);//acumulo el angulo de los a
				if (angulos.at(j) > unbral) s[0]=true;
			}else{//impar
				a2+=angulos.at(j);//acumulo el angulo de los a
				if (angulos.at(j) > unbral) s[1]=true;
			}
		}

		if (a1<mejor_a && !s[0]){
			mejor_a = a1;
			c=1;
			mejor=i;
		}
		if (a2<mejor_a && !s[1]){
			mejor_a = a2;
			c=2;
			mejor=i;
		}
		if (mejor_a == 0.0) break;
	}
#pragma endregion 

	//backup

	if (c==-1){return;}

	Arista & a = aristas.at(mejor);
	size_t t1=a.getTrian_1(), t2=a.getTrian_2();
	size_t va=a.getVert_a(), vb=a.getVert_b();

	backup b;

	//std::cout<<"a.a "<<a.getVert_a()<<" a.b "<<a.getVert_b()<<" a.t1 "<<a.getTrian_1()<<" a.t2 "<<a.getTrian_2()<<" c "<<c <<" mejor "<<mejor<<endl;

	if (angulos.size()>2){
#pragma region desabilito los triangulos y la arista 
		if (triangulos.at(t1).estaActivo()){ 
			nTri--; 
			triangulos.at(t1).cambiarEstado();
			accion t;
			t.e=TRI_ACT;
			t.param.push_back(t1);
			b.estado_anterior.push_back(t);
		}

		if (triangulos.at(t2).estaActivo()){ 
			nTri--;
			triangulos.at(t2).cambiarEstado();
			accion t;
			t.e=TRI_ACT;
			t.param.push_back(t2);
			b.estado_anterior.push_back(t);
		}

		accion t;
		t.e=ARIS_ACT;
		t.param.push_back(mejor);
		b.estado_anterior.push_back(t);

		a.cambiarEstado();

#pragma endregion 
	}else{
		std::cout<<"esto por que pasa\n";
		return;
	}

	if (c==2){

		accion v;
		v.e=VERT_MOV;
		v.param.push_back(va);
		v.pos=vertices.at(va).getPosicion();
		vertices.at(va).setPosicion(vertices[vb].getPosicion());
		b.estado_anterior.push_back(v);
	}

	//agrego los triangulos del vertice a
	vector<size_t> vt;
	Vertice v0 = vertices.at(va);
	FOR(i,v0.getSizetrian()){
		if (v0.getTriangulo(i) == t1 || v0.getTriangulo(i) == t2 ) continue;
		if (!triangulos.at(v0.getTriangulo(i)).estaActivo()) continue;
		asdf.push_back(v0.getTriangulo(i));
		//guadarlos triangulos para actualizar las normales
	}

	//agrego los triangulos del vertice b y modifico los vertices ser necesario
	v0 = vertices.at(vb);
	size_t  buscado;
	FOR(i,v0.getSizetrian()){
		if (v0.getTriangulo(i) == t1 || v0.getTriangulo(i) == t2) continue;
		if (!triangulos.at(v0.getTriangulo(i)).estaActivo()) continue;
		asdf.push_back(v0.getTriangulo(i));
		//modifico
		buscado=buscarVertice(v0.getTriangulo(i),vb);

		if (buscado == -1 ) cout << "esto es raro";
		//std::cout << v0.getTriangulo(i)<< " ";
		accion t;
		t.e=TRI_VER;
		t.param.push_back(v0.getTriangulo(i));
		t.param.push_back(buscado);
		t.param.push_back(triangulos.at(v0.getTriangulo(i)).getVertice(buscado));
		b.estado_anterior.push_back(t);
		triangulos.at(v0.getTriangulo(i)).setVertice(buscado,va);
		actualizar(v0.getTriangulo(i),va,vb,b);
	}

	accion v;
	v.e=VER_TRI;
	v.param = vertices.at(va).getTriangulos();
	v.param.push_back(va);
	vertices.at(va).setTrian(asdf);
	b.estado_anterior.push_back(v);
	colisiones.push_back(b);
	recalcular(asdf);
}

//dado un triangulo y un vertice determina la posicion de 
//dicho vertice en el arreglo de vertices en el triangulo
size_t Off::buscarVertice(size_t t,size_t v){
	if (triangulos.at(t).getVertice(0)== v)
		return 0;
	if (triangulos.at(t).getVertice(1)== v)
		return 1;
	if (triangulos.at(t).getVertice(2)== v)
		return 2;
	return -1;
}

void Off::actualizar(size_t t,size_t a, size_t b,backup & bk){
	FOR(i,3){
		Arista & ar = aristas.at(triangulos.at(t).getArista(i));
		size_t busqueda;
		if (!triangulos.at(ar.getTrian_1()).estaActivo() && !triangulos.at(ar.getTrian_2()).estaActivo()) continue;

		if (ar.getVert_a() == b){
			busqueda = findArista(Arista(a,ar.getVert_b()));
			if (busqueda != -1){
				//ar=busqueda
				accion t1;
				t1.e=TRI_ARI;
				t1.param.push_back(t);
				t1.param.push_back(i);
				t1.param.push_back(triangulos.at(t).getArista(i));
				bk.estado_anterior.push_back(t1);
				triangulos.at(t).setArista(i, busqueda);
				ar.cambiarEstado();
				accion x;
				x.e=ARIS_ACT;
				x.param.push_back(triangulos.at(t).getArista(i));
				bk.estado_anterior.push_back(x);
				if (triangulos.at(aristas.at(busqueda).getTrian_1()).estaActivo()){
					accion t1;
					t1.e=ARIS_TRI;
					t1.param.push_back(busqueda);
					t1.param.push_back(1);
					t1.param.push_back(aristas.at(busqueda).getTrian_2());
					bk.estado_anterior.push_back(t1);
					aristas.at(busqueda).setTrian_2(t);
				}else{
					accion t1;
					t1.e=ARIS_TRI;
					t1.param.push_back(busqueda);
					t1.param.push_back(0);
					t1.param.push_back(aristas.at(busqueda).getTrian_1());
					bk.estado_anterior.push_back(t1);
					aristas.at(busqueda).setTrian_1(t);
				}
			}else{
				//ar->a= a
				accion t1;
				t1.e=ARIS_VER;
				t1.param.push_back(triangulos.at(t).getArista(i));
				t1.param.push_back(0);
				t1.param.push_back(aristas.at(triangulos.at(t).getArista(i)).getVert_a());
				bk.estado_anterior.push_back(t1);
				ar.setVert_a(a);
			}
		}
		if (ar.getVert_b() == b){
			busqueda = findArista(Arista(ar.getVert_a(),a));
			if (busqueda != -1){
				//ar=busqueda
				accion t1;
				t1.e=TRI_ARI;
				t1.param.push_back(t);
				t1.param.push_back(i);
				t1.param.push_back(triangulos.at(t).getArista(i));
				bk.estado_anterior.push_back(t1);
				triangulos.at(t).setArista(i, busqueda);
				ar.cambiarEstado();
				accion x;
				x.e=ARIS_ACT;
				x.param.push_back(triangulos.at(t).getArista(i));
				bk.estado_anterior.push_back(x);
				if (triangulos.at(aristas.at(busqueda).getTrian_1()).estaActivo()){
					accion t1;
					t1.e=ARIS_TRI;
					t1.param.push_back(busqueda);
					t1.param.push_back(1);
					t1.param.push_back(aristas.at(busqueda).getTrian_2());
					bk.estado_anterior.push_back(t1);
					aristas.at(busqueda).setTrian_2(t);
				}else{
					accion t1;
					t1.e=ARIS_TRI;
					t1.param.push_back(busqueda);
					t1.param.push_back(0);
					t1.param.push_back(aristas.at(busqueda).getTrian_1());
					bk.estado_anterior.push_back(t1);
					aristas.at(busqueda).setTrian_1(t);
				}
			}else{
				//ar->b = a
				accion t1;
				t1.e=ARIS_VER;
				t1.param.push_back(triangulos.at(t).getArista(i));
				t1.param.push_back(1);
				t1.param.push_back(aristas.at(triangulos.at(t).getArista(i)).getVert_b());
				ar.setVert_b(a);
			}
		}
		//backup

	}
}

void Off::undo(){
	if (colisiones.empty()) return;
	backup bk = colisiones.at(colisiones.size()-1);

	FOR(i,bk.estado_anterior.size()){
		resolver(bk.estado_anterior.at(i));
	}

	colisiones.pop_back();
}

void Off::resolver(accion a){
	size_t aux;
	switch (a.e)
	{
	case VERT_MOV : 
		vertices.at(a.param.at(0)).setPosicion(a.pos);
		break;
	case VER_TRI:
		aux = a.param.at(a.param.size()-1);
		a.param.pop_back();
		vertices.at(aux).setTrian(a.param);
		break;
	case ARIS_TRI: 
		aux = a.param.at(1);
		if (aux)
			aristas.at(a.param.at(0)).setTrian_2(a.param.at(2));
		else
			aristas.at(a.param.at(0)).setTrian_1(a.param.at(2));
		break;
	case ARIS_VER : 
		aux = a.param.at(1);
		if (aux)
			aristas.at(a.param.at(0)).setVert_b(a.param.at(2));
		else
			aristas.at(a.param.at(0)).setVert_a(a.param.at(2));
		break;
	case ARIS_ACT : 
		aristas.at(a.param.at(0)).cambiarEstado();
		break;
	case TRI_ACT : 
		triangulos.at(a.param.at(0)).cambiarEstado();
		nTri++;
		break;
	case TRI_ARI : break;
		aux = a.param.at(1);
		triangulos.at(a.param.at(0)).setArista(aux,a.param.at(2));
		break;
	case TRI_VER : 
		aux = a.param.at(1);
		triangulos.at(a.param.at(0)).setVertice(aux,a.param.at(2));
		break;
	default:
		break;
	}
}

void Off::recalcular(vector<size_t> l){
	vector<size_t> v;
	
	FOR(i,l.size()){
		Triangulo & t=triangulos.at(l.at(i));
		v.push_back(t.getVertice(0));
		v.push_back(t.getVertice(1));
		v.push_back(t.getVertice(2));

		glm::vec3 a(vertices.at(t.getVertice(0)).getPosicion());
		glm::vec3 b(vertices.at(t.getVertice(1)).getPosicion());
		glm::vec3 c(vertices.at(t.getVertice(2)).getPosicion());
		t.setNormal(glm::normalize(glm::cross(c - a, b - a)));
	}
	
	FOR(i,v.size()){
		Vertice& ve = vertices.at(v.at(i));
		glm::vec3 n(0.0f);
		FOR(j,ve.getSizetrian()){
			n += triangulos.at(ve.getTriangulo(j)).getNormal();
		}
		n = glm::normalize(n);
		ve.setNormal(n);
	}
}