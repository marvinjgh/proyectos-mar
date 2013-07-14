#include "mc.h"

Modelo::Modelo(){
	dimx=dimy=dimz=0;
	b=NULL;
	vol=NULL;
}

size_t Modelo::VertexInterp(Punto3D* p1,Punto3D* p2,Punto3D* g1,Punto3D* g2,double valp1,double valp2, float umbral)
{
	float mu;
	Vertice *p = new Vertice();

	long long id = ((long long)(p1->coord[0]+p2->coord[0]))<<32;
	//id <<=32;
	id += ((short)(p1->coord[1]+p2->coord[1]))<<16;
	id += ((short)(p1->coord[2]+p2->coord[2]));
	size_t bu = -1;//aristas.busqueda(id);
	if (bu==-1){
		if (ABS(umbral-valp1) < 0.00001){
			p->p=*p1;
			p->n=*g1;		
		}else{
			if (ABS(umbral-valp2) < 0.00001){
				p->p=*p2;
				p->n=*g2;
			}else
				if (ABS(valp1-valp2) < 0.00001){
					p->p=*p1;
					p->n=*g1;
				}else{
					p->p=*p1;
					p->n=*g1;

					mu = (float)((umbral - valp1) / (valp2 - valp1));

					p->p += mu * (*p2 - *p1);
					p->n += mu * (*g2 - *g1);
				}
		}
		//aristas.insertar(id,puntos.size());
		puntos.push_back(p);
		return puntos.size()-1;
	}else{
		return bu;
	}

}

int Modelo::PolygoniseCube(Celda g,Triangulo *tri,int index,float umbral)
{
	int i,ntri = 0;
	size_t vertlist[12];

	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[index] & 1) {
		vertlist[0] = VertexInterp(&g.p[0],&g.p[1],&g.g[0],&g.g[1],g.val[0],g.val[1],umbral);
	}
	if (edgeTable[index] & 2) {
		vertlist[1] = VertexInterp(&g.p[1],&g.p[2],&g.g[1],&g.g[2],g.val[1],g.val[2],umbral);
	}
	if (edgeTable[index] & 4) {
		vertlist[2] = VertexInterp(&g.p[2],&g.p[3],&g.g[2],&g.g[3],g.val[2],g.val[3],umbral);
	}
	if (edgeTable[index] & 8) {
		vertlist[3] = VertexInterp(&g.p[3],&g.p[0],&g.g[3],&g.g[0],g.val[3],g.val[0],umbral);
	}
	if (edgeTable[index] & 16) {
		vertlist[4] = VertexInterp(&g.p[4],&g.p[5],&g.g[4],&g.g[5],g.val[4],g.val[5],umbral);
	}
	if (edgeTable[index] & 32) {
		vertlist[5] = VertexInterp(&g.p[5],&g.p[6],&g.g[5],&g.g[6],g.val[5],g.val[6],umbral);
	}
	if (edgeTable[index] & 64) {
		vertlist[6] = VertexInterp(&g.p[6],&g.p[7],&g.g[6],&g.g[7],g.val[6],g.val[7],umbral);
	}
	if (edgeTable[index] & 128) {
		vertlist[7] = VertexInterp(&g.p[7],&g.p[4],&g.g[7],&g.g[4],g.val[7],g.val[4],umbral);
	}
	if (edgeTable[index] & 256) {
		vertlist[8] = VertexInterp(&g.p[0],&g.p[4],&g.g[0],&g.g[4],g.val[0],g.val[4],umbral);
	}
	if (edgeTable[index] & 512) {
		vertlist[9] = VertexInterp(&g.p[1],&g.p[5],&g.g[1],&g.g[5],g.val[1],g.val[5],umbral);
	}
	if (edgeTable[index] & 1024) {
		vertlist[10] = VertexInterp(&g.p[2],&g.p[6],&g.g[2],&g.g[6],g.val[2],g.val[6],umbral);
	}
	if (edgeTable[index] & 2048) {
		vertlist[11] = VertexInterp(&g.p[3],&g.p[7],&g.g[3],&g.g[7],g.val[3],g.val[7],umbral);
	}

	/* Create the triangles */
	for (i=0;triTable[index][i]!=-1;i+=3) {
		tri[ntri].p[0] = vertlist[triTable[index][i  ]];
		tri[ntri].p[1] = vertlist[triTable[index][i+1]];
		tri[ntri].p[2] = vertlist[triTable[index][i+2]];
		ntri++;
	}

	return(ntri);
}

inline void Modelo::calcular(Celda* grids,int i,int j,int k){
	Punto3D p(0.f,0.f,0.f);
	grids->p[0].coord[0] = (float)i;
	grids->p[0].coord[1] = (float)j;
	grids->p[0].coord[2] = (float)k;
	grids->val[0] = vol[i][j][k];
	grids->p[1].coord[0] = (float)i+1;
	grids->p[1].coord[1] = (float)j;
	grids->p[1].coord[2] = (float)k; 
	grids->val[1] = vol[i+1][j][k];
	grids->p[2].coord[0] = (float)i+1;
	grids->p[2].coord[1] = (float)j+1;
	grids->p[2].coord[2] = (float)k;
	grids->val[2] = vol[i+1][j+1][k];
	grids->p[3].coord[0] = (float)i;
	grids->p[3].coord[1] = (float)j+1;
	grids->p[3].coord[2] = (float)k;
	grids->val[3] = vol[i][j+1][k];
	grids->p[4].coord[0] = (float)i;
	grids->p[4].coord[1] = (float)j;
	grids->p[4].coord[2] = (float)k+1;
	grids->val[4] = vol[i][j][k+1];
	grids->p[5].coord[0] = (float)i+1;
	grids->p[5].coord[1] = (float)j;
	grids->p[5].coord[2] = (float)k+1;
	grids->val[5] = vol[i+1][j][k+1];
	grids->p[6].coord[0] = (float)i+1;
	grids->p[6].coord[1] = (float)j+1;
	grids->p[6].coord[2] = (float)k+1;
	grids->val[6] = vol[i+1][j+1][k+1];
	grids->p[7].coord[0] = (float)i;
	grids->p[7].coord[1] = (float)j+1;
	grids->p[7].coord[2] = (float)k+1;
	grids->val[7] = vol[i][j+1][k+1];

	grids->g[0]=(i!=0 && j!=0 && k!=0)?GRAD_VERT_0():p;
	grids->g[1]=(i!=dimx-2 && j!=0 && k!=0)?GRAD_VERT_1():p;
	grids->g[2]=(i!=dimx-2 && j!=dimy-2 && k!=0)?GRAD_VERT_2():p;
	grids->g[3]=(i!=0 && j!=dimy-2 && k!=0)?GRAD_VERT_3():p;
	grids->g[4]=(i!=0 && j!=0 && k!=dimz-2)?GRAD_VERT_4():p;
	grids->g[5]=(i!=dimx-2 && j!=0 && k!=dimz-2)?GRAD_VERT_5():p;
	grids->g[6]=(i!=dimx-2 && j!=dimy-2 && k!=dimz-2)?GRAD_VERT_6():p;
	grids->g[7]=(i!=0 && j!=dimy-2 && k!=dimz-2)?GRAD_VERT_7():p;

}

void Modelo::mc(float umbral, size_t limite){
	Celda grids;
	int n=0,ntri=0,index;
	Triangulo triangles[10];
	finales.clear();
	puntos.clear();
	aristas.limpiar();
	time_t tiempo = time(NULL);
	double seg=0.0;

	FOR(k,dimz-1){
		if(k%25 == 0){
			printf("slite %d\n",k);
		}
		FOR(j,dimy-1){
			FOR(i,dimx-1){
				index=0;

				if (vol[i][j][k] < umbral) index |= 1;
				if (vol[i+1][j][k] < umbral) index |= 2;
				if (vol[i+1][j+1][k] < umbral) index |= 4;
				if (vol[i][j+1][k] < umbral) index |= 8;
				if (vol[i][j][k+1] < umbral) index |= 16;
				if (vol[i+1][j][k+1] < umbral) index |= 32;
				if (vol[i+1][j+1][k+1] < umbral) index |= 64;
				if (vol[i][j+1][k+1] < umbral) index |= 128;

				if (index == 0 || index == 255) continue;

				calcular(&grids,i,j,k);


				n = PolygoniseCube(grids,triangles,index,umbral);
				for (int l=0;l<n;l++)
					finales.push_back(triangles[l]);
				ntri += n;

				if (ntri >= limite){
					if (dimx>0){
						seg = difftime(time(NULL),tiempo);
						info(umbral,seg,1);
					}
					return;
				}

			}
		}

	}

	if (dimx>0){
		seg = difftime(time(NULL),tiempo);
		info(umbral,seg,0);
	}
}

void Modelo::readRaw(char* file){

	int dx,dy,dz,c;
	GLubyte *data = ReadRawVolume(file,&dx,&dy,&dz,&c);

	if (data == NULL)
		return;
	if (vol != NULL){

		FOR(i,dimx){
			FOR(j,dimy)
				delete vol[i][j];
			delete vol[i];
		}
		delete vol;
	}
	dimx=dx;dimy=dy;dimz=dz;comp=c;
	dx<<=1; dy<<=1;dz<<=1;
	dimmx = max(max(dx,dy),dz);

	vol = new float**[dimx];
	FOR(i,dimx){
		vol[i] = new float*[dimy];
		FOR(j,dimy)
			vol[i][j] = new float[dimz];
	}

	FOR(z,dimz){
		FOR(y,dimy){
			FOR(x,dimx){
				if (c==1)
				{
					vol[x][y][z]=data[dimx*((z*dimy+y)+1)-(x+1)];
				}else{
					c=dx*((z*dy+y)+1)-((x-1)<<1);
					vol[x][z][y]=(data[c]<<8)+data[c+1];
				}

			}
		}
	}

	delete data;
}

void Modelo::readPvm(char* file){

	size_t dx,dy,dz,c;
	float sx,sy,sz;
	GLubyte *data = readPVMvolume(file,&dx,&dy,&dz,&c,&sx,&sy,&sz);

	if (data == NULL)
		return;
	if (vol != NULL){

		FOR(i,dimx){
			FOR(j,dimy)
				delete vol[i][j];
			delete vol[i];
		}
		delete vol;
	}
	dimx=dx;dimy=dy;dimz=dz;comp=c;
	dx<<=1; dy<<=1;dz<<=1;
	dimmx = max(max(dx,dy),dz);

	vol = new float**[dimx];
	FOR(i,dimx){
		vol[i] = new float*[dimy];
		FOR(j,dimy)
			vol[i][j] = new float[dimz];
	}

	FOR(z,dimz){
		FOR(y,dimy){
			FOR(x,dimx){
				if (c==1)
				{
					vol[x][y][z]=data[dimx*((z*dimy+y)+1)-(x+1)];
				}else{
					c=dx*((z*dy+y)+1)-((x-1)<<1);
					vol[x][z][y]=(data[c]<<8)+data[c+1];
				}

			}
		}
	}

	delete data;
}

void Modelo::centrar(){
	glm::vec3 x(dimx,dimy,dimz);
	x/=2.0;
	glm::vec3 s(2.0/dimmx,2.0/dimmx,2.0/dimmx);
	glm::vec3 r(0.0,1.0,0.0);
	centro = glm::mat4();
	centro = glm::scale(centro,s);
	centro = glm::translate(centro,-x);
}

void Modelo::buffer(){
	int asd=0;
	Punto3D p3;
	
	if (b==NULL){
		b = new float[(puntos.size()+(puntos.size()%2))*6];
		glGenBuffers(2, BuffersObject);
		glBindBuffer(GL_ARRAY_BUFFER, BuffersObject[0]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, (puntos.size()+(puntos.size()%2))*6*sizeof(float), 0, GL_DYNAMIC_COPY_ARB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, BuffersObject[1]);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, finales.size()*3*sizeof(size_t), 0 , GL_DYNAMIC_COPY_ARB);
	}else{
		delete b;
		b = new float[(puntos.size()+(puntos.size()%2))*6];
		glBindBuffer(GL_ARRAY_BUFFER, BuffersObject[0]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, (puntos.size()+(puntos.size()%2))*6*sizeof(float), 0, GL_DYNAMIC_COPY_ARB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, BuffersObject[1]);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, finales.size()*3*sizeof(size_t), 0 , GL_DYNAMIC_COPY_ARB);
	}

	unsigned int *t = new unsigned int[finales.size()*3];
	FORU(i,puntos.size()){
		memcpy(b+asd,puntos.at(i)->p.coord,sizeof(puntos.at(i)->p.coord));
		memcpy(b+asd+3,puntos.at(i)->n.coord,sizeof(puntos.at(i)->n.coord));
		asd+=6;
	}
	if(puntos.size()%2){
		memcpy(b+asd,p3.coord,sizeof(p3.coord));
		memcpy(b+asd+3,p3.coord,sizeof(p3.coord));
	}
	asd=0;
	FORU(i,finales.size()){
		memcpy(t+asd,finales.at(i).p,sizeof(finales.at(i).p));
		asd+=3;
	}
	glBindBuffer(GL_ARRAY_BUFFER, BuffersObject[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER,0, puntos.size()*6*sizeof(float),b);
	glBindBuffer(GL_ARRAY_BUFFER, BuffersObject[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER,0,finales.size()*3*sizeof(size_t),t);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Modelo::info(float umbral,double time,bool b){
	printf("////////////////////////////////////////////\n");
	if (b)
		printf("se alcanzo el limite de poligonos\n");
	printf("Triangulos generados %d\n",finales.size());
	printf("Vertices generados %d\n",puntos.size());
	printf("Bits por muestra %d\n",comp*8);
	printf("Umbral %f\n",umbral);
	printf("Tiempo %f\n",time);
	printf("////////////////////////////////////////////\n");
}