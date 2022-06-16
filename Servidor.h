#pragma once
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <tchar.h>
typedef struct {
	char desiredPiece;
	unsigned int x,y;
}PLAY;

typedef struct {
	char board[20][20];
	unsigned int actualSize;
	char pecas[6];		//array that hold the available pieces;

	unsigned int begin[2];
	unsigned int end[2];

	unsigned int currentWaterXY[2];
	unsigned int previousWaterXY[2];
	char lastInsert;

	PLAY peca;
	unsigned int win;
	unsigned int lose;

	char nome[254];

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