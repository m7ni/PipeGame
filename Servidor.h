#pragma once
#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <tchar.h>

typedef struct {
	char board[20][20];
	int actualSize;		//actual size that we are playing
	char pecas[6];		//array that hold the available pieces;
	//falta pensar como guardar a solu��o
}Board;


#endif /*SERVIDOR_H*/