#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <AntTweakBar.h>
#include <glm\glm.hpp>
#include <vector>

#define PI      3.141592654

#define FOR(q,n) for(size_t q=0;q<n;q++)

#define MAX(x,y) (x)>(y) ? (x):(y)
#define MIN(x,y) (x)<(y) ? (x):(y)
#define ABS(x) (x)>(0) ? (x):(-x)