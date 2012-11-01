#include "..\include\glsl\Shader.h"

Shader::Shader(void)
{
	shaders[VERTEX_SHADER]=0;
	shaders[FRAGMENT_SHADER]=0;
	locationList.clear();
}

Shader::~Shader(void)
{
	locationList.clear();
	glDeleteProgramsARB(1,&program);
}

void Shader::loadShader(GLenum type, const char* source) {

	FILE *file;
	char *shaderReaded = NULL;
	int byteCount = 0;

	fopen_s( &file, source, "rt" );
	if( file != NULL ){
		fseek(file, 0, SEEK_END);
		byteCount = ftell( file );
		rewind( file );

		if( byteCount ){
			shaderReaded = (char *) malloc( sizeof(char)*( byteCount+1 ) );
			byteCount = (int)fread( shaderReaded, sizeof(char), byteCount, file );
			shaderReaded[ byteCount ] = '\0';
		}
		fclose(file);
	}
	
	shaders[type] = (type==VERTEX_SHADER)?glCreateShaderObjectARB(GL_VERTEX_SHADER): glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
	CheckSLError(shaders[type]);
	if (shaders[type]==GL_FALSE){
		perror("error Creando el shader\n");
		return;
	}

	const char * t = shaderReaded;

	glShaderSourceARB(shaders[type], 1, &t, NULL);
	CheckSLError(shaders[type]);

	glCompileShader(shaders[type]);
	glGetObjectParameterivARB(shaders[type], GL_OBJECT_COMPILE_STATUS_ARB, &byteCount);
	CheckSLError(shaders[type]);
	if (byteCount == GL_FALSE){
		glDeleteObjectARB(shaders[type]);
	}
	free(shaderReaded);
	
}

void Shader::create() {
	
	if (shaders[0]==0 || shaders[1]==0){
		fprintf(stderr,"falta Vertex/Fragment shaderSource");
		return;
	}
	program = glCreateProgramObjectARB ();
	if (program==GL_FALSE){
		perror("error Creando el programa\n");
		return;
	}

	glAttachObjectARB(program, shaders[VERTEX_SHADER]);
	CheckSLError(program);
	glAttachObjectARB(program, shaders[FRAGMENT_SHADER]);
	CheckSLError(program);

}

void Shader::link() {
	//link y check

	glLinkProgramARB (program);
	CheckSLError(program);
	glDetachObjectARB(program,shaders[VERTEX_SHADER]);
	glDeleteShader(shaders[VERTEX_SHADER]);
	glDetachObjectARB(program,shaders[FRAGMENT_SHADER]);
	glDeleteShader(shaders[FRAGMENT_SHADER]);
}

void Shader::create_Link() {
	
	if (shaders[0]==0 || shaders[1]==0){
		fprintf(stderr,"falta Vertex/Fragment shaderSource");
		return;
	}
	program = glCreateProgramObjectARB ();
	CheckSLError(program);
	if (program==GL_FALSE){
		perror("error Creando el programa\n");
		return;
	}

	glAttachObjectARB(program, shaders[VERTEX_SHADER]);
	CheckSLError(program);
	glAttachObjectARB(program, shaders[FRAGMENT_SHADER]);
	CheckSLError(program);
	glLinkProgramARB (program);
	CheckSLError(program);

	glDetachShader(program,shaders[VERTEX_SHADER]);
	glDeleteShader(shaders[VERTEX_SHADER]);
	glDetachShader(program,shaders[FRAGMENT_SHADER]);
	glDeleteShader(shaders[FRAGMENT_SHADER]);
}

void Shader::enable() {
	glUseProgramObjectARB(program);
}

void Shader::disable() {
	glUseProgramObjectARB(0);
}

void Shader::AddAttribute(const string attribute) {
	locationList[attribute]= glGetAttribLocationARB(program, attribute.c_str());	
}

void Shader::AddUniform(const string uniform) {
	locationList[uniform]= glGetUniformLocationARB(program, uniform.c_str());	
}

GLuint Shader::getLocation(const string location){
	return locationList[location];
}

GLuint Shader::operator [](const string location) {
	return locationList[location];
}

void Shader::CheckSLError(GLhandleARB &object){
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(object, infologLength, &charsWritten, infoLog);
		fprintf(stderr,"log: %s",infoLog);
		free(infoLog);
	}
}