#include "glsl\Shader.h"

Shader::Shader(void)
{
	shaders[VERTEX_SHADER]=0;
	shaders[FRAGMENT_SHADER]=0;
	locationList.clear();
}

Shader::~Shader(void)
{
	locationList.clear();
	glDeleteProgram(program);
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

	shaders[type] = (type==VERTEX_SHADER)?glCreateShader(GL_VERTEX_SHADER): glCreateShader(GL_FRAGMENT_SHADER);

	if (shaders[type]==GL_FALSE){
		perror("error Creando el shader\n");
		return;
	}

	const char * t = shaderReaded;

	glShaderSource(shaders[type], 1, &t, NULL);
	glGetShaderiv(shaders[type], GL_SHADER_SOURCE_LENGTH, &byteCount);//check del shadersource

	if (byteCount){
		glCompileShader(shaders[type]);
		glGetShaderiv(shaders[type], GL_COMPILE_STATUS, &byteCount);

		if (byteCount == GL_FALSE) {
			glGetShaderiv(shaders[type], GL_INFO_LOG_LENGTH, &byteCount);
			GLchar *infoLog= new GLchar[byteCount];
			glGetShaderInfoLog(shaders[type], byteCount, NULL, infoLog);
			fprintf(stderr,"Compile log: %s\n",infoLog);
			delete [] infoLog;
			glDeleteShader(shaders[type]);
			shaders[type]=0;
		}

		free(shaderReaded);
	}
}

void Shader::create() {
	GLint status;
	if (shaders[0]==0 || shaders[1]==0){
		fprintf(stderr,"falta Vertex/Fragment shaderSource");
		return;
	}
	program = glCreateProgram ();
	if (program==GL_FALSE){
		perror("error Creando el programa\n");
		return;
	}

	glAttachShader(program, shaders[VERTEX_SHADER]);
	glGetProgramiv (program, GL_ATTACHED_SHADERS, &status);
	if (status == 0) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &status);
		GLchar *infoLog= new GLchar[status];
		glGetProgramInfoLog (program, status, NULL, infoLog);
		fprintf(stderr,"Attach log: %s\n",infoLog);
		delete [] infoLog;
	}
	glAttachShader(program, shaders[FRAGMENT_SHADER]);
	glGetProgramiv (program, GL_ATTACHED_SHADERS, &status);
	if (status == 1) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &status);
		GLchar *infoLog= new GLchar[status];
		glGetProgramInfoLog (program, status, NULL, infoLog);
		fprintf(stderr,"Link log: %s\n",infoLog);
		delete [] infoLog;
	}

}

void Shader::link() {
	//link y check
	GLint status;
	glLinkProgram (program);
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &status);
		GLchar *infoLog= new GLchar[status];
		glGetProgramInfoLog (program, status, NULL, infoLog);
		fprintf(stderr,"Link log: %s\n",infoLog);
		delete [] infoLog;
	}
	glDetachShader(program,shaders[VERTEX_SHADER]);
	glDeleteShader(shaders[VERTEX_SHADER]);
	glDetachShader(program,shaders[FRAGMENT_SHADER]);
	glDeleteShader(shaders[FRAGMENT_SHADER]);
}

void Shader::create_Link() {
	GLint status;
	if (shaders[0]==0 || shaders[1]==0){
		fprintf(stderr,"falta Vertex/Fragment shaderSource");
		return;
	}
	program = glCreateProgram ();
	if (program==GL_FALSE){
		perror("error Creando el programa\n");
		return;
	}

	glAttachShader(program, shaders[VERTEX_SHADER]);
	glGetProgramiv (program, GL_ATTACHED_SHADERS, &status);
	if (status == 0) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &status);
		GLchar *infoLog= new GLchar[status];
		glGetProgramInfoLog (program, status, NULL, infoLog);
		fprintf(stderr,"Attach log: %s\n",infoLog);
		delete [] infoLog;
	}
	glAttachShader(program, shaders[FRAGMENT_SHADER]);
	glGetProgramiv (program, GL_ATTACHED_SHADERS, &status);
	if (status == 1) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &status);
		GLchar *infoLog= new GLchar[status];
		glGetProgramInfoLog (program, status, NULL, infoLog);
		fprintf(stderr,"Link log: %s\n",infoLog);
		delete [] infoLog;
	}

	glLinkProgram (program);
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &status);
		GLchar *infoLog= new GLchar[status];
		glGetProgramInfoLog (program, status, NULL, infoLog);
		fprintf(stderr,"Link log: %s\n",infoLog);
		delete [] infoLog;
	}

	glDetachShader(program,shaders[VERTEX_SHADER]);
	glDeleteShader(shaders[VERTEX_SHADER]);
	glDetachShader(program,shaders[FRAGMENT_SHADER]);
	glDeleteShader(shaders[FRAGMENT_SHADER]);
}

void Shader::enable() {
	glUseProgram(program);
}

void Shader::disable() {
	glUseProgram(0);
}

void Shader::AddAttribute(const string attribute) {
	locationList[attribute]= glGetAttribLocation(program, attribute.c_str());	
}

void Shader::AddUniform(const string uniform) {
	locationList[uniform]= glGetUniformLocation(program, uniform.c_str());	
}

GLuint Shader::getLocation(const string location){
	return locationList[location];
}

GLuint Shader::operator [](const string location) {
	return locationList[location];
}