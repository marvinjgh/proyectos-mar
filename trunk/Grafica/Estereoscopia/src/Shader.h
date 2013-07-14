#pragma once
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <GL\glew.h>
#include <Gl\freeglut.h>
using namespace std;

enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};

class Shader
{
public:
	/*Constructor*/
	Shader(void);
	/*Destructor*/
	~Shader(void);
	/*verifico si hay error*/
	void CheckSLError(GLhandleARB &object);
	/*Carga y compila el shader del archivo dado en source
	@param type tipo de shader
	@param source ubicacion del shader
	*/
	void loadShader(GLenum type, const char* source);
	/*Crea el programa y le anexa los shader*/
	void create();
	/*Enlaza el programa*/
	void link();
	/* Crea y enlaza el programa */
	void create_Link();
	/*Activar*/
	void enable();
	/*desactivar*/
	void disable();
	/*Agregar Attribute*/
	void AddAttribute(const string attribute);
	/*Agregar Uniform*/
	void AddUniform(const string uniform);
	/*obtener el id el progrma*/
	GLuint getId(void);
	/*obtener el id de la variable local*/
	GLuint getLocation(const string location);
	//sobreecarga del operador [] para un acceso mas transparente
	GLuint operator[](const string attribute);
	
private:

	GLhandleARB	program;
	GLhandleARB shaders[3];//0->vertex shader, 1->fragment shader, 2->geometry shader
	map<string,GLuint> locationList;
};

