#pragma once
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <tchar.h>



typedef struct {
	char board[20][20];
	int actualSize;		//actual size that we are playing
	char pecas[6];		//array that hold the available pieces;
}Board;


// 9473  ━
// 9475  ┃
// 9487  ┏
// 9491  ┓
// 9499  ┛
// 9495  ┗
#endif /*SERVIDOR_H*/