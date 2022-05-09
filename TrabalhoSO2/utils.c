#include "utils.h"

LARGE_INTEGER intToLargeInt(int i) {
	LARGE_INTEGER li;
	li.QuadPart = i;
	return li;
}

void setupBoard(MemDados* data, DWORD actualSize) {
	srand(time(NULL));
	BOOL goUp = NULL;
	Board aux;
	aux.actualSize = actualSize;

	for (DWORD i = 0; i < aux.actualSize; i++) {
		for (DWORD j = 0; j < aux.actualSize; j++)
		{
			data->VBoard->board[i][j] = '.';
		}
	}

	DWORD lineBegin = rand() % aux.actualSize;
	DWORD lineEnd = rand() % aux.actualSize;

	data->VBoard->board[lineBegin][0] = 'B'; //begin position
	data->VBoard->board[lineEnd][aux.actualSize-1] = 'E'; //end position

	if (lineEnd > lineBegin) //temos que ir para baixo quando chegar ao lado direito da tabela
		goUp = FALSE;
	else if (lineEnd < lineBegin) //temos que ir para cim quando chegar ao lado direito da tabela
		goUp = TRUE;
									//goUP é null, por isso só temos que chegar ao lado direito do board e estamos na pos final

	for (DWORD j = 1; j < aux.actualSize - 2; j++) {
		data->VBoard->board[lineBegin][j] = '━';
	}

	switch (goUp) {
	case TRUE:
		aux.board[lineBegin][aux.actualSize - 1] = '┛';

		for (DWORD i = lineBegin - 1; i > lineEnd; i--) {
			aux.board[i][aux.actualSize - 1] = '┃';
		}
		break;
	case FALSE:
		aux.board[lineBegin][aux.actualSize - 1] = '┓';

		for (DWORD i = lineBegin; i < lineEnd; i++) {
			aux.board[i][aux.actualSize - 1] = '┃';
		}

		break;
	default:
		break;
	}

	aux.pecas[0] = '━';
	aux.pecas[1] = '┃';
	aux.pecas[2] = '┏';
	aux.pecas[3] = '━';
	aux.pecas[4] = '┛';
	aux.pecas[5] = '┗';

	WaitForSingleObject(data->mutexBoard, INFINITE);
	CopyMemory(&data->VBoard,&aux, sizeof(Board));
	ReleaseMutex(data->mutexBoard);
}

void printBoard(Board* aux) {
	for (DWORD i = 0; i < aux->actualSize; i++) {
		printf("\n");
		for (DWORD j = 0; j < aux->actualSize; j++)
		{
			printf("%c", aux->board[i][j]);
		}
	}
}

