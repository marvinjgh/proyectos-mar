#include "Clases.h"
#include <cmath>
#include <cstdlib>

/* Clase Color3f
 * Constructor por defecto 
 */
Color3f::Color3f()
	:r(0.0),g(0.0),b(0.0)
{ return; }

/* Clase Color3f
 * Constructor apartir de red, green, blue
 */
Color3f::Color3f(GLfloat red, GLfloat green, GLfloat blue)
	:r(red),g(green),b(blue)
{ return; }

/* Clase Punto
 * Constructor por defecto */
Punto::Punto()
{ return; }

/* Clase Punto
 * Constructor apartir de un x, y */
Punto::Punto(GLint xi, GLint yi)
	:x(xi),y(yi)
{ return; }

/* Clase Rectangulo
 * Determinar si ya esta el punto final */
bool Rectangulo::haypuntofinal()
{
	return i==1;
}

void Figura::rotar(float a)
{
	double angle = PI*(360-a)/180.0;
	cose=(float) (cos(angle));
	seno=(float) (sin(angle));
	angulo=a;
}

//cada nuevo factor afecta al anterior
void Figura::escalar(float x, float y)
{
	int xc=(min_max(true,true)+min_max(false,true))>>1,yc=(min_max(true,false)+min_max(false,false))>>1;
	for (int j=0;j<=i;j++){
		puntos[j].x=(int)(xc + puntos[j].x*x -x*xc);
		puntos[j].y=(int)(yc + puntos[j].y*y -y*yc);
	}
	e1*=x;
	e2*=y;
}

//cada nuevo valor altera al existente
void Figura::traladar(GLint x, GLint y)
{
	for (int j=0;j<=i;j++){
		puntos[j].x+=x;
		puntos[j].y+=y;
	}
	xt+=x;
	yt+=y;
}

void Figura::setPointEnd(GLint xf, GLint yf, PT actual)
{
	if (haypuntofinal()){
		puntos[i].x=xf;
		puntos[i].y=yf;
	}
	if(actual == NEW)
	{
		puntos[++i]=Punto(xf,yf);
	}
}

GLint Figura::min_max(bool min_mx,bool x_y){
	GLint x=(min_mx)?10000:0,j=0;
	for (j=0;j<=i;j++)
		if (min_mx){
			if (x_y)
			x=min(x,puntos[j].x);
			else
			x=min(x,puntos[j].y);
		}else{
			if (x_y)
			x=max(x,puntos[j].x);
			else
			x=max(x,puntos[j].y);
		}
	return x;
}

/* Clase Rectangulo
 * Constructor con x, y, pt, red, green, blue */
Rectangulo::Rectangulo(GLint x,GLint y, GLushort pt,Color3f c_line ,Color3f c_fill , L f)
{
	puntos=(Punto*)calloc(2,sizeof(Punto));
	puntos[0]=Punto(x,y);
	patron=pt;
	linea = Color3f(c_line.r,c_line.g,c_line.b);
	fondo = Color3f(c_fill.r,c_fill.g,c_fill.b);
	i=0;
	llenado=f;
	e1=e2=cose=1.0;
	angulo=xt=yt=seno=0.0;
	type=REC;
}

/* Clase Rectangulo
 * Funcion que determina como pintar */
void Rectangulo::draw()
{
	GLint xini=min(puntos[0].x,puntos[1].x),yini=min(puntos[0].y,puntos[1].y);
	GLint xfin=max(puntos[0].x,puntos[1].x),yfin=max(puntos[0].y,puntos[1].y);
	int xc=(xini+xfin)>>1,yc=(yini+yfin)>>1;
	glColor3f (linea.r,linea.g,linea.b);
	glPushMatrix();
	line(xini,yini,xfin,yini,xc,yc,cose,seno,patron);
	line(xfin,yini,xfin,yfin,xc,yc,cose,seno,patron);
	line(xfin,yfin,xini,yfin,xc,yc,cose,seno,patron);
	line(xini,yfin,xini,yini,xc,yc,cose,seno,patron);
	if (llenado){
		glColor3f (fondo.r,fondo.g,fondo.b);
		for (int j=yini+1;j<yfin;j++){
			line(xini+1,j,xfin-1,j,xc,yc,cose,seno,patron);
		}
	}
	
	glPopMatrix();
}

/* Clase Linea
 * Determinar si ya esta el punto final */
bool Linea::haypuntofinal()
{
	return i==1;
}

/* Clase Linea
 * Constructor con x, y, pt, red, green, blue */
Linea::Linea(GLint x,GLint y, GLushort pt, Color3f c_line)
{
	puntos=(Punto*)calloc(2,sizeof(Punto));
	puntos[0]=Punto(x,y);
	patron=pt;
	linea = Color3f(c_line.r,c_line.g,c_line.b);
	i=0;
	e1=e2=cose=1.0;
	angulo=xt=yt=seno=0.0;
	type=LINE;
}

/* Clase Linea
 * Funcion que determina como pintar */
void Linea::draw()
{
	glPushMatrix();
	int xc=(min_max(true,true)+min_max(false,true))>>1,yc=(min_max(true,false)+min_max(false,false))>>1;
	glColor3f (linea.r,linea.g,linea.b);
	line(puntos[0].x,puntos[0].y,puntos[1].x,puntos[1].y,xc,yc,cose,seno,patron);
	glPopMatrix();
}

/* Clase Circulo
 * Determinar si ya esta el punto final */
bool Circulo::haypuntofinal()
{
	return i==1;
}

/* Clase Circulo
 * Constructor con x, y, pt, red, green, blue */
Circulo::Circulo(GLint x,GLint y, GLushort pt,Color3f c_line ,Color3f c_fill , L f)
{
	puntos=(Punto*)calloc(2,sizeof(Punto));
	puntos[0]=Punto(x,y);
	patron=pt;
	linea = Color3f(c_line.r,c_line.g,c_line.b);
	fondo=Color3f(c_fill.r,c_fill.g,c_fill.b);
	i=0;
	llenado=f;
	e1=e2=cose=1.0;
	angulo=xt=yt=seno=0.0;
	type=CIRC;
}

/* Clase Circulo
 * Modificador del ultimo punto */
void Circulo::setPointEnd(GLint xf,GLint yf, PT actual)
{
	if (haypuntofinal()){
		puntos[i].x=xf;
		puntos[i].y=yf;
	}
	if(actual == NEW)
	{
		puntos[++i]=Punto(xf,yf);
	}
	r=(unsigned int)sqrt(pow((double)(xf-puntos[0].x),2.0)+pow((double)(yf-puntos[0].y),2.0));
}

/* Clase Circulo
 * Funcion que determina como pintar */
void Circulo::draw()
{
	int x=0, d=(1-r)<<1, y=r, y_o=0;
	GLushort actual=1;
	glPushMatrix();
	draw8point(x,y,&actual,y_o);
	while(y>x){
		if (d>=0) 
		{
			d+=((x-y)<<1)+5;
			--y;
			y_o=1;
		}
		else 
		{
			d+=((x<<1)+3); 
		}
		x++;
		draw8point(x,y,&actual,y_o);
		y_o=0;
	}
	glPopMatrix();
}

/* Clase Circulo
 * Funcion que pintar los puntos en los octantes */
void Circulo::draw8point(GLint x, GLint y,GLushort* actual,GLint y_o ){
	//xc,yc son el centro
	//x,y seran las coordenadas de la espera unitaria
	GLint xc=puntos[0].x, yc = puntos[0].y;
	glBegin(GL_POINTS);
	glColor3f (linea.r,linea.g,linea.b);

	float xs= xc + (cose*(y)-seno*(-x));
	float ys= yc + (seno*(y)+cose*(-x));
	
	if (isDrawpixel(patron,actual)){
		glVertex2f(xs,ys);//O1
		xs= xc + (cose*(x)-seno*(-y));
		ys= yc + (seno*(x)+cose*(-y));
		glVertex2f(xs,ys);//O2
		xs= xc + (cose*(-x)-seno*(-y));
		ys= yc + (seno*(-x)+cose*(-y));
		glVertex2f(xs,ys);//O3
		xs= xc + (cose*(-y)-seno*(-x));
		ys= yc + (seno*(-y)+cose*(-x));
		glVertex2f(xs,ys);//O4
		xs= xc + (cose*(-y)-seno*(x));
		ys= yc + (seno*(-y)+cose*(x));
		glVertex2f(xs,ys);//O5
		xs= xc + (cose*(x)-seno*(y));
		ys= yc + (seno*(x)+cose*(y));
		glVertex2f(xs,ys);//O7
		xs= xc + (cose*(-x)-seno*(y));
		ys= yc + (seno*(-x)+cose*(y));
		glVertex2f(xs,ys);//O6
		xs= xc + (cose*(y)-seno*(x));
		ys= yc + (seno*(y)+cose*(x));
		glVertex2f(xs,ys);//O8
	}
	glEnd();
	
	if(llenado ){
		glColor3f (fondo.r,fondo.g,fondo.b);
		if (y_o==1){
			line(xc-x+1,yc+y,xc+x-1,yc+y,xc,yc,cose,seno,0xFFFF);
			line(xc-x+1,yc-y,xc+x-1,yc-y,xc,yc,cose,seno,0xFFFF);
		}
		line(xc-y+1,yc+x,xc+y-1,yc+x,xc,yc,cose,seno,0xFFFF);
		line(xc-y+1,yc-y,xc+y-1,yc-x,xc,yc,cose,seno,0xFFFF);
	}
}

/* Clase Elipse
 * Determinar si ya esta el punto final */
bool Elipse::haypuntofinal()
{
	return i==1;
}

/* Clase Elipse
 * Constructor con x, y, pt, red, green, blue */
Elipse::Elipse(GLint x ,GLint y ,GLushort pt ,Color3f c_line ,Color3f c_fill ,L f )
{
	puntos=(Punto*)calloc(2,sizeof(Punto));
	puntos[0]=Punto(x,y);
	patron=pt;
	linea = Color3f(c_line.r,c_line.g,c_line.b);
	fondo=Color3f(c_fill.r,c_fill.g,c_fill.b);
	i=0;
	llenado=f;
	e1=e2=cose=1.0;
	angulo=xt=yt=seno=0.0;
	type=ELIP;
}

/* Clase Elipse
 * Modificador del ultimo punto */
void Elipse::setPointEnd(GLint xf,GLint yf, PT actual)
{
	if (haypuntofinal()){
		puntos[i].x=xf;
		puntos[i].y=yf;
	}
	if(actual == NEW)
	{
		puntos[++i]=Punto(xf,yf);
	}
}

/* Clase Elipse
 * Funcion que determina como pintar */
void Elipse::draw()
{
	a=abs(puntos[0].x-puntos[1].x)>>1;
	b=abs(puntos[0].y-puntos[1].y)>>1;
	int a2=a*a,b2=b*b;
	int d=((b2-a2*b)<<2)+a2;
	int x=0,y=b;
	GLushort actual=1;
	GLint y_o=0;
	glPushMatrix();
	draw4point(x,y,&actual,y_o);
	while(((b2<<1)*(x+1))<(a2*((y+1)<<1))){
		if (d>=0)
		{
			d+=((b2*((x<<1)+3))<<2)-((a2*((y-1)<<1))<<2);
			y--;
			y_o=1;
		}
		else
		{
			d+=(b2*((x<<1)+3))<<2;
		}
		x++;
		draw4point(x,y,&actual,y_o);
		y_o=0;
	}

	d=b2*((x<<1)+1)*((x<<1)+1)+(a2<<2)*(((y-1)*(y-1))-b2);
	while(y>0){
		if (d<0){
			d+=(b2*((x+1)<<1)+a2*(-(y<<1)+3))<<2;
			x++;
		}
		else
		{
			d+=(a2*(-(y<<1)+3))<<2;
		}
		y--;
		draw4point(x,y,&actual,1);
	}
	
	glPopMatrix();
}

/* Clase Elipse
 * Funcion que pintar los puntos en los cuadrantes */
void Elipse::draw4point(GLint x, GLint y,GLushort* actual,GLint y_o){
	//x,y seran las coordenadas a mover
	GLint xc=min_max(true,true)+a,yc=min_max(true,false)+b;
	glColor3f (linea.r,linea.g,linea.b);
	glBegin(GL_POINTS);
	float xs= xc + (cose*(x)-seno*(-y));
	float ys= yc + (seno*(x)+cose*(-y));
	if (isDrawpixel(patron,actual)){
		glVertex2f(xs,ys);//C1
		xs= xc +(cose*(-x)-seno*(-y));
		ys= yc +(seno*(-x)+cose*(-y));
		glVertex2f(xs,ys);//C2
		xs=xc + (cose*(-x)-seno*(y));
		ys=yc + (seno*(-x)+cose*(y));
		glVertex2f(xs,ys);//C3
		xs=xc + (cose*(x)-seno*(y));
		ys=yc + (seno*(x)+cose*(y));
		glVertex2f(xs,ys);//C4
	}
	glEnd();
	if (llenado && y_o==1)
	{
		glColor3f (fondo.r,fondo.g,fondo.b);
		line(xc-x+1,yc-y,xc+x-1,yc-y,xc,yc,cose,seno,0xFFFF);
		line(xc-x+1,yc+y,xc+x-1,yc+y,xc,yc,cose,seno,0xFFFF);
	}
}

/* Clase Triangulo
 * Determinar si ya esta el punto final */
bool Triangulo::haypuntofinal()
{
	return i==2;
}

/* Clase Triangulo
 * Constructor con x, y, pt, red, green, blue */
Triangulo::Triangulo(GLint x, GLint y, GLushort pt, Color3f c_line ,Color3f c_fill , L f)
{
	puntos=(Punto*)calloc(3,sizeof(Punto));
	puntos[0]=Punto(x,y);
	patron=pt;
	linea = Color3f(c_line.r,c_line.g,c_line.b);
	fondo = Color3f(c_fill.r,c_fill.g,c_fill.b);
	i=0;
	llenado=f;
	e1=e2=cose=1.0;
	angulo=xt=yt=seno=0.0;
	type=TRI;
}

/* Clase Triangulo
 * Modificador del ultimo punto */
void Triangulo::setPointEnd(GLint xf,GLint yf, PT actual)
{
	if (actual == OLD){
		puntos[i].x=xf;
		puntos[i].y=yf;
	}
	if(actual == NEW)
	{
		puntos[++i]=Punto(xf,yf);
	}
	
}

/* Clase Triangulo
 * Funcion que determina como pintar */
void Triangulo::draw()
{
	glPushMatrix();
	glColor3f (linea.r,linea.g,linea.b);
	
	int xc=(min_max(true,true)+min_max(false,true))>>1;
	int yc=(min_max(true,false)+min_max(false,false))>>1;
	if (haypuntofinal())
	{
		line(puntos[0].x,puntos[0].y,puntos[1].x,puntos[1].y,xc,yc,cose,seno,patron);
		line(puntos[1].x,puntos[1].y,puntos[2].x,puntos[2].y,xc,yc,cose,seno,patron);
		line(puntos[2].x,puntos[2].y,puntos[0].x,puntos[0].y,xc,yc,cose,seno,patron);
	}
	else
	{
		line(puntos[0].x,puntos[0].y,puntos[1].x,puntos[1].y,xc,yc,cose,seno,patron);
	}
	glPopMatrix();
}


/* Clase Curva_bezier
 * Modificador del ultimo punto ingresado */
void Curva_bezier::setPointEnd(GLint xf,GLint yf, PT actual)
{
	if (actual == OLD){
		puntos[i].x=xf;
		puntos[i].y=yf;
		count=0;
	}
	if(actual == NEW)
	{
		puntos[++i]=Punto(xf,yf);
	}
}

/* Clase Curva_bezier
 * Determinar si ya esta el punto final */
bool Curva_bezier::haypuntofinal()
{
	return i==(size-1);
}

/* Clase Triangulo_gl
 * Constructor con x, y, pt, red, green, blue */
Curva_bezier::Curva_bezier(GLint x, GLint y, GLushort pt,Color3f c_line,Color3f c_fill, L f,GLuint tam)
{
	size=tam;
	puntos=(Punto*)calloc(tam,sizeof(Punto));
	puntos[0]=Punto(x,y);
	patron=pt;
	linea = Color3f(c_line.r,c_line.g,c_line.b);
	fondo = Color3f(c_fill.r,c_fill.g,c_fill.b);
	i=0;
	llenado=f;
	t1=0.01;
	double t2=1.0;
	n=(unsigned int)(t2/t1);
	puntos_curva=(Punto*)calloc(n,sizeof(Punto));
	count=0;
	e1=e2=cose=1.0;
	angulo=xt=yt=seno=0.0;
	type=CUR_B;
}

void Curva_bezier::draw(){
	int xc=(min_max(true,true)+min_max(false,true))>>1;
	int yc=(min_max(true,false)+min_max(false,false))>>1;
	if (i==1){
		line(puntos[0].x,puntos[0].y,puntos[1].x,puntos[1].y,xc,yc,cose,seno,patron);
	}else{
		if (count==0){
			Punto* aux = (Punto*)calloc(size,sizeof(Punto));;
			for (double t=0.0;t<1.0;t+=t1)
			{
				punto_en_curva(aux,t);
			}
			punto_en_curva(aux,1.0);
			free(aux);
		}
		if (llenado){
			glColor3f (fondo.r,fondo.g,fondo.b);
			for (GLushort t=0;t<i;t++)
			{
				line(puntos[t].x,puntos[t].y,puntos[t+1].x,puntos[t+1].y,xc,yc,cose,seno,0xFFFF);
			}
		}

		glColor3f (linea.r,linea.g,linea.b);
		for (unsigned int t=0;t<n;t++)
		{
			line(puntos_curva[t].x,puntos_curva[t].y,puntos_curva[t+1].x,puntos_curva[t+1].y,xc,yc,cose,seno,0xFFFF);
		}
	}
}
void Curva_bezier::punto_en_curva(Punto aux[],double t){
	for (unsigned short r=0;r<size;r++)
		aux[r]=puntos[r];

	for (int j=i;j;j--){
		for (int r=0;r<j;r++){
			aux[r].x=(GLint)((1-t)*aux[r].x+t*aux[r+1].x);
			aux[r].y=(GLint)((1-t)*aux[r].y+t*aux[r+1].y);
		}
	}
	puntos_curva[count++]=Punto(aux[0].x,aux[0].y);
}

void Curva_bezier::addpoint(){
	size++;
	puntos=(Punto*)realloc(puntos,size*sizeof(Punto));

}

GLint Curva_bezier::buscar(GLint x, GLint y){
	GLuint n,i;
	GLint xp,yp;
	for (i=0;i<size;i++){
		xp=puntos[i].x;
		yp=puntos[i].y;
		if((x>=xp-3 && x<=xp+3)&&(y>=yp-3 && y<=yp+3)){
			return i;
		}
	}
	return -1;
}

void Curva_bezier::setPoint(GLint i,GLint xf,GLint yf)
{
	puntos[i].x=xf;
	puntos[i].y=yf;
}

void Curva_bezier::eliminar(GLint asd)
{
	Punto *p=(Punto*)calloc(size-1,sizeof(Punto));
	int j=0;
	for (int d=0;d<size;d++){
		if (d!=asd){
			p[j++]=Punto(puntos[d].x,puntos[d].y);
		}
	}
	free(puntos);
	puntos=p;
}