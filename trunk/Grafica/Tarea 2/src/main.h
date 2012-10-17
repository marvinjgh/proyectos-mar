#ifndef __MAIN_H
#define __MAIN_H
#pragma comment (lib, "lib/freeglut.lib")
#include "GL/freeglut.h"
//#include "Clases.h"

extern int WIDTH, HEIGHT;

extern GLushort pt;

enum PT {OLD, NEW};

enum TipoFig { LINE, REC, TRI, CIRC ,ELIP , CUR_B };

enum Mystate { UP, DOWN};

enum L { NO_FILL, FILL};

bool isDrawpixel(GLushort , GLushort* );

void line(GLint ,GLint ,GLint ,GLint ,GLint ,GLint ,float ,float ,GLushort );

void lineH(GLint ,GLint ,GLint );

//void create_file();

//void read_file();

#endif