#pragma once
#ifndef CLIENTE_H
#define CLIENTE_H

#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h> 
#include <io.h>


typedef struct {
	BITMAP bitmap;
	HDC	DC;
} Image;

typedef struct {
	Image imgArray[6];
	Image horizontal;
	Image vertical;
	Image start;
	Image end;
	Image Left90;
	Image Right90;
	Image Left_1_90;
	Image Right_1_90;
	Image blank;
	Image water;
	Image beginU;
	Image beginH;
	Image win;
	Image lost;
	HANDLE mutex;
	Image barrier;
}Images;

typedef struct {
	int x;
	int y;
} vect2;

typedef struct {
	vect2 coord;
	Image* image;
}TabImagens;

#endif