#pragma once
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <tchar.h>



typedef struct {
	char board[20][20];
	unsigned int actualSize;		//actual size that we are playing
	char pecas[6];		//array that hold the available pieces;
	unsigned int begin[2];
	unsigned int lastWaterXY[2];
	char lastInsert;
	unsigned int win;
}Board;


// 9473  ━
// 9475  ┃
// 9487  ┏
// 9491  ┓
// 9499  ┛
// 9495  ┗
#endif /*SERVIDOR_H*/