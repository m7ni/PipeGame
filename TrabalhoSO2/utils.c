#include "utils.h"



void setupBoard(MemDados* dados) {
	srand(time(NULL));
	BOOL goUp = NULL;


	for (DWORD i = 0; i < dados->VBoard->actualSize; i++) {
		for (DWORD j = 0; j < dados->VBoard->actualSize; j++)
		{
			dados->VBoard->board[i][j] = '.';
		}
	}

	DWORD lineBegin = rand() % dados->VBoard->actualSize;
	DWORD lineEnd = rand() % dados->VBoard->actualSize;

	dados->VBoard->board[lineBegin][0] = 'B'; //begin position
	dados->VBoard->board[lineEnd][dados->VBoard->actualSize-1] = 'E'; //end position

	if (lineEnd > lineBegin) //temos que ir para baixo quando chegar ao lado direito da tabela
		goUp = FALSE;
	else if (lineEnd < lineBegin) //temos que ir para cim quando chegar ao lado direito da tabela
		goUp = TRUE;
									//goUP é null, por isso só temos que chegar ao lado direito do board e estamos na pos final


	for (DWORD j = 1; j < dados->VBoard->actualSize - 2; j++) {
		dados->VBoard->board[lineBegin][j] = '━';
	}

	switch (goUp) {
	case TRUE:
		dados->VBoard->board[lineBegin][dados->VBoard->actualSize - 1] = '┛';

		for (DWORD i = lineBegin - 1; i > lineEnd; i--) {
			dados->VBoard->board[i][dados->VBoard->actualSize - 1] = '┃';
		}
		break;
	case FALSE:
		dados->VBoard->board[lineBegin][dados->VBoard->actualSize - 1] = '┓';

		for (DWORD i = lineBegin; i < lineEnd; i++) {
			dados->VBoard->board[i][dados->VBoard->actualSize - 1] = '┃';
		}

		break;
	default:
		break;
	}

	dados->VBoard->pecas[0] = '━';
	dados->VBoard->pecas[1] = '┃';
	dados->VBoard->pecas[2] = '┏';
	dados->VBoard->pecas[3] = '━';
	dados->VBoard->pecas[4] = '┛';
	dados->VBoard->pecas[5] = '┗';

}

void printBoard(MemDados* dados) {
	for (DWORD i = 0; i < dados->VBoard->actualSize; i++) {
		printf("\n");
		for (DWORD j = 0; j < dados->VBoard->actualSize; j++)
		{
			printf("%c",dados->VBoard->board[i][j]);
		}
	}
}