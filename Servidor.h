#pragma once
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <tchar.h>


typedef struct {
	char board[20][20];
	unsigned int actualSize;
	char pecas[6];		//array that hold the available pieces;

	unsigned int begin[2];
	unsigned int end[2];

	unsigned int lastWaterXY[2];
	unsigned int nextWaterXY[2];
	char lastInsert;

	char desiredPiece;
	unsigned int win;
	unsigned int lose;
}PLAYER;

typedef struct {
	PLAYER player[2];
	unsigned int actualSize;		//actual size that we are playing
	DWORD numP;
}Board;



// 9473  ━
// 9475  ┃
// 9487  ┏
// 9491  ┓
// 9499  ┛
// 9495  ┗
#endif /*SERVIDOR_H*/