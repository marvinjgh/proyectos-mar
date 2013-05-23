#include <glsl\Shader.h>
#include <AntTweakBar.h>
#include <FreeImage.h>
#include <vector>

#define aporte(x1, x2, x) ((x2-x)/(x2-x1))

GLuint textura;

void crear_textura();

void crear_textura(){

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	FIBITMAP *dib(0);

	BYTE* bits(0);

	GLuint width(0), height(0);


	fif = FreeImage_GetFileType("u.ppm", 0);
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename("u.ppm");

	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, "u.ppm");

	if	(dib != NULL){

		dib = FreeImage_ConvertTo32Bits(dib);
		if (dib != NULL){
			bits = FreeImage_GetBits(dib);

			glEnable(GL_TEXTURE_1D);
			if (textura==-1)
				glGenTextures(1,&textura);
			glBindTexture(GL_TEXTURE_1D,textura);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, 256, 0,
				GL_BGRA_EXT, GL_UNSIGNED_BYTE, bits );
			glDisable(GL_TEXTURE_1D);
		}
	}

	FreeImage_Unload(dib);
}