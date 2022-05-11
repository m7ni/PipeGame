#include "utils.h"

LARGE_INTEGER intToLargeInt(int i) {
	LARGE_INTEGER li;
	li.QuadPart = i;
	return li;
}

void setupBoard(Board * aux, DWORD actualSize) {
	srand(time(NULL));
	BOOL goUp = NULL;

	aux->actualSize = actualSize;
	
	for (DWORD i = 0; i < aux->actualSize; i++) {
		for (DWORD j = 0; j < aux->actualSize; j++)
		{
			aux->board[i][j] = '.';
		}
	}
	
	DWORD lineBegin = rand() % aux->actualSize;
	DWORD lineEnd = rand() % aux->actualSize;

	aux->begin[0] = lineBegin;
	aux->begin[1] = 0;
	aux->lastInsert = 'B';


	aux->board[lineBegin][0] = 'B'; //begin position
	aux->board[lineEnd][aux->actualSize-1] = 'E'; //end position

	if (lineEnd > lineBegin) //temos que ir para baixo quando chegar ao lado direito da tabela
		goUp = FALSE;
	else if (lineEnd < lineBegin) //temos que ir para cim quando chegar ao lado direito da tabela
		goUp = TRUE;
									//goUP é null, por isso só temos que chegar ao lado direito do board e estamos na pos final

	for (DWORD j = 1; j < aux->actualSize - 1; j++) {
		aux->board[lineBegin][j] = '━';
	}

	switch (goUp) {
	case TRUE:
		aux->board[lineBegin][aux->actualSize - 1] = '┛';

		for (DWORD i = lineBegin - 1; i > lineEnd; i--) {
			aux->board[i][aux->actualSize - 1] = '┃';
		}
		break;
	case FALSE:
		aux->board[lineBegin][aux->actualSize - 1] = '┓';

		for (DWORD i = lineBegin; i < lineEnd; i++) {
			aux->board[i][aux->actualSize - 1] = '┃';
		}

		break;
	default:
		break;
	}

	aux->pecas[0] = '━';
	aux->pecas[1] = '┃';
	aux->pecas[2] = '┏';
	aux->pecas[3] = '━';
	aux->pecas[4] = '┛';
	aux->pecas[5] = '┗';
	
}

void insertWater(Board * board) {
	if (board->lastInsert == 'B') {
		if (board->board[board->begin[0]][board->begin[1]+1]) { //1 para o lado direito

		}
		else if (board->board[board->begin[0] + 1][board->begin[1]]) { //para baixo

		}
		else if (board->board[board->begin[0] -1][board->begin[1]]) {

		}
	}
	else {

	}
}

void printBoard(Board* aux) {

	for (DWORD i = 0; i < aux->actualSize; i++) {
		_tprintf(TEXT("\n"));
		for (DWORD j = 0; j < aux->actualSize; j++)
		{
			_tprintf(TEXT("%c"), aux->board[i][j]);
		}
	}
}

