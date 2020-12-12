#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#ifndef OS_LINUX
#include <Windows.h>
#pragma execution_character_set("utf-8")
#endif
#include "gL/gl.h"
#include "gl/glu.h"
#include "gl/glut.h"

#include "mymodel.h"
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

const double PI = acos(-1);
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
GLuint LoadTexture(const string filename);

#endif