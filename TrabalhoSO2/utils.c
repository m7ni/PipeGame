#include "utils.h"



int createBoard(MemDados* dados) {
		
	for (int i = 0; i < dados->VBoard->actualSize; i++) {
		for (int j = 0; j < dados->VBoard->actualSize; j++)
		{
			dados->VBoard->board[i][j] = "*";
		}
	}
//TODO determinar as duas celulas e a solução

}