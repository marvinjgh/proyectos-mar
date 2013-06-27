#pragma once
#include <cmath>
#include <iostream>
#include <GL\glew.h>
#include <Gl\freeglut.h>
#include <vector>
using namespace std;
#define PI      3.141592654
#define _2PI    6.283185308     // 2 * PI
#define PI_2    1.570796327     // PI / 2
#define PI_3	1.047197551		// PI / 3
#define PI_4    0.785398163     // PI / 4 
#define PI_OVER_360	 0.0087266462599716478846184538424431

enum Sombreado { FLAT , GOURAUD, PHONG };

#define FOR(q,n) for(size_t q=0;q<n;q++)
