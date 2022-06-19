#include "utils.h"


void placeWater(PLAYER* pData);
void setupBoard(MemDados* aux, DWORD actualSize, DWORD np) {
	srand(time(NULL));

	WaitForSingleObject(aux->mutexBoard, INFINITE);
	for (DWORD p = 0; p < np; p++) {
		aux->VBoard->actualSize = actualSize;
		aux->VBoard->player[p].win = 0;
		aux->VBoard->player[p].actualSize = actualSize;

		for (DWORD i = 0; i < aux->VBoard->actualSize; i++) {
			for (DWORD j = 0; j < aux->VBoard->actualSize; j++)
			{
				aux->VBoard->player[p].board[i][j] = '.';
			}
		}
	}
	aux->VBoard->numP = np;
		DWORD lineBegin1 = rand() % aux->VBoard->actualSize;
		DWORD lineEnd1 = rand() % aux->VBoard->actualSize;

		DWORD lineBegin = (rand() % ((aux->VBoard->actualSize - 2) - 1 + 1)) + 1;
		DWORD lineEnd = (rand() % ((aux->VBoard->actualSize - 2) - 1 + 1)) + 1;

		DWORD startingSide = rand() % 4; 

		switch (startingSide)
		{
		case 0: //canto esquerdo 
			aux->VBoard->player[0].board[lineBegin][0] = 'z';
			aux->VBoard->player[1].board[lineBegin][0] = 'z';

			aux->VBoard->player[0].board[lineEnd][aux->VBoard->actualSize - 1] = 'e';
			aux->VBoard->player[1].board[lineEnd][aux->VBoard->actualSize - 1] = 'e';

			aux->VBoard->player[0].end[0]= lineEnd;
			aux->VBoard->player[0].end[1] = aux->VBoard->actualSize - 1;

			aux->VBoard->player[1].end[0] = lineEnd;
			aux->VBoard->player[1].end[1] = aux->VBoard->actualSize - 1;

			aux->VBoard->player[0].begin[0] = lineBegin;
			aux->VBoard->player[0].begin[1] = 0;

			aux->VBoard->player[1].begin[0] = lineBegin;
			aux->VBoard->player[1].begin[1] = 0;


		
			break;
		case 1: //canto direito
			aux->VBoard->player[0].board[lineBegin][aux->VBoard->actualSize - 1] = 'z';
			aux->VBoard->player[1].board[lineBegin][aux->VBoard->actualSize - 1] = 'z';

			aux->VBoard->player[0].board[lineEnd][0] = 'e';
			aux->VBoard->player[1].board[lineEnd][0] = 'e';



			aux->VBoard->player[0].end[0] = lineEnd;
			aux->VBoard->player[0].end[1] = 0;

			aux->VBoard->player[1].end[0] = lineEnd;
			aux->VBoard->player[1].end[1] = 0;

			aux->VBoard->player[0].begin[0] = lineBegin;
			aux->VBoard->player[0].begin[1] = aux->VBoard->actualSize-1;

			aux->VBoard->player[1].begin[0] = lineBegin;
			aux->VBoard->player[1].begin[1] = aux->VBoard->actualSize-1;

	
			break;

		case 2: //canto superior
			aux->VBoard->player[0].board[0][lineBegin] = 'x';
			aux->VBoard->player[1].board[0][lineBegin] = 'x';

			aux->VBoard->player[0].board[aux->VBoard->actualSize - 1][lineEnd] = 'e';
			aux->VBoard->player[1].board[aux->VBoard->actualSize - 1][lineEnd] = 'e';



			aux->VBoard->player[0].end[0] = aux->VBoard->actualSize - 1;
			aux->VBoard->player[0].end[1] = lineEnd;

			aux->VBoard->player[1].end[0] = aux->VBoard->actualSize - 1;
			aux->VBoard->player[1].end[1] = lineEnd;

			aux->VBoard->player[0].begin[0] = 0;
			aux->VBoard->player[0].begin[1] = lineBegin;

			aux->VBoard->player[1].begin[0] = 0;
			aux->VBoard->player[1].begin[1] = lineBegin;
			break;

		case 3: //canto inferior
			aux->VBoard->player[0].board[aux->VBoard->actualSize - 1][lineBegin] = 'x';
			aux->VBoard->player[1].board[aux->VBoard->actualSize - 1][lineBegin] = 'x';

			aux->VBoard->player[0].board[0][lineEnd] = 'e';
			aux->VBoard->player[1].board[0][lineEnd] = 'e';

			aux->VBoard->player[0].end[0] = 0;
			aux->VBoard->player[0].end[1] = lineEnd;

			aux->VBoard->player[1].end[0] = 0;
			aux->VBoard->player[1].end[1] = lineEnd;

			aux->VBoard->player[0].begin[0] = aux->VBoard->actualSize-1;
			aux->VBoard->player[0].begin[1] = lineBegin;

			aux->VBoard->player[1].begin[0] = aux->VBoard->actualSize-1;
			aux->VBoard->player[1].begin[1] = lineBegin;

			break;
		default:
			break;
		}

		aux->VBoard->player[0].currentWaterXY[0] = aux->VBoard->player[0].begin[0];
		aux->VBoard->player[0].currentWaterXY[1] = aux->VBoard->player[0].begin[1];

		aux->VBoard->player[1].currentWaterXY[0] = aux->VBoard->player[1].begin[0];
		aux->VBoard->player[1].currentWaterXY[1] = aux->VBoard->player[1].begin[1];

		aux->VBoard->player[0].lastInsert = 'B';
		aux->VBoard->player[1].lastInsert = 'B';
	
	ReleaseMutex(aux->mutexBoard);

}


DWORD waterMoving(PLAYER* pData) {

		if (top(pData)) {
		}
		else if (bottom(pData)) {
		}
		else if (left(pData)) {
		}
		else if (right(pData)) {
		}
		else {
			pData->win = -1;
			return 1;
		}

		placeWater(pData);
		if (pData->currentWaterXY[0] == pData->end[0] && pData->currentWaterXY[1] == pData->end[1]){

			pData->win = 1;
			return 1;
		}

		return 3;

}

void placeWater(PLAYER* pData) {
	pData->board[pData->currentWaterXY[0]][pData->currentWaterXY[1]] = TEXT('a');
}

BOOL top(PLAYER* pData) {
	int new_pos_y = pData->currentWaterXY[0] -1;
	if (new_pos_y < 0) {
		return FALSE;
	}

	if (new_pos_y == pData->previousWaterXY[0] &&
		pData->currentWaterXY[1] == pData->previousWaterXY[1]) {
		return FALSE;
	}

	if (pData->board[new_pos_y][pData->currentWaterXY[1]] == 'x' ||
		pData->board[new_pos_y][pData->currentWaterXY[1]] == 's' ||
		pData->board[new_pos_y][pData->currentWaterXY[1]] == 'd' ||
		pData->board[new_pos_y][pData->currentWaterXY[1]] == TEXT('e')) {
		pData->previousWaterXY[0] = pData->currentWaterXY[0];
		pData->previousWaterXY[1] = pData->currentWaterXY[1];
		pData->currentWaterXY[0] = new_pos_y;
		return TRUE;
	}
	return FALSE;
}

BOOL bottom(PLAYER* pData) {
	int new_pos_y = pData->currentWaterXY[0] + 1;
	if (new_pos_y > pData->actualSize) {
		return FALSE;
	}

	if (new_pos_y == pData->previousWaterXY[0] &&
		pData->currentWaterXY[1] == pData->previousWaterXY[1]) {
		return FALSE;
	}

	if (pData->board[new_pos_y][pData->currentWaterXY[1]] == 'x' ||
		pData->board[new_pos_y][pData->currentWaterXY[1]] == 'l' ||
		pData->board[new_pos_y][pData->currentWaterXY[1]] == 'r' ||
		pData->board[new_pos_y][pData->currentWaterXY[1]] == TEXT('e')) {
		pData->previousWaterXY[0] = pData->currentWaterXY[0];
		pData->previousWaterXY[1] = pData->currentWaterXY[1];
		pData->currentWaterXY[0] = new_pos_y;
		
		return TRUE;
	}
	return FALSE;
}

BOOL left(PLAYER* pData) {
	int new_pos_x = pData->currentWaterXY[1] - 1;

	if (new_pos_x < 0) {
		return FALSE;
	}

	if (new_pos_x == pData->previousWaterXY[1] &&
		pData->currentWaterXY[0] == pData->previousWaterXY[0]) {
		return FALSE;
	}

	if (pData->board[pData->currentWaterXY[0]][new_pos_x] == 'z' ||
		pData->board[pData->currentWaterXY[0]][new_pos_x] == 's' ||
		pData->board[pData->currentWaterXY[0]][new_pos_x] == 'r' ||
		pData->board[pData->currentWaterXY[0]][new_pos_x] == TEXT('e')) {
		pData->previousWaterXY[0] = pData->currentWaterXY[0];
		pData->previousWaterXY[1] = pData->currentWaterXY[1];
		pData->currentWaterXY[1] = new_pos_x;
		return TRUE;
	}
	return FALSE;
}

BOOL right(PLAYER* pData) {
	int new_pos_x = pData->currentWaterXY[1] + 1;

	if (new_pos_x > pData->actualSize) {
		return FALSE;
	}

	if (new_pos_x == pData->previousWaterXY[1] &&
		pData->currentWaterXY[0] == pData->previousWaterXY[0]) {
		return FALSE;
	}

	if (pData->board[pData->currentWaterXY[0]][new_pos_x] == 'z' ||
		pData->board[pData->currentWaterXY[0]][new_pos_x] == 'l' ||
		pData->board[pData->currentWaterXY[0]][new_pos_x] == 'd' ||
		pData->board[pData->currentWaterXY[0]][new_pos_x] == TEXT('e')) {
		pData->previousWaterXY[0] = pData->currentWaterXY[0];
		pData->previousWaterXY[1] = pData->currentWaterXY[1];
		pData->currentWaterXY[1] = new_pos_x;
		return TRUE;
	}
	return FALSE;
}

DWORD placePeca(MemDados* aux, char peca, int posX, int posY,int id) {

	WaitForSingleObject(aux->mutexBoard, INFINITE);

		if (posX > aux->VBoard->actualSize || posY > aux->VBoard->actualSize || posX < 0 || posY < 0 || peca == '\0') {
			return -1;
		}
		else  {
			aux->VBoard->player[id].board[posX][posY] = peca;
			_ftprintf(stderr, TEXT("-----------> Placed pipe at [%d][%d]\n"), posX, posY,peca);
		}

		ReleaseMutex(aux->mutexBoard);
}

void printBoard(Board* aux) {
	for (DWORD m = 0; m< aux->numP; m++) {
		_tprintf(TEXT("--------PLAYER [%d]---------"),m);
		for (DWORD i = 0; i < aux->actualSize; i++) {
			_tprintf(TEXT("\n"));
			for (DWORD j = 0; j < aux->actualSize; j++)
			{
				_tprintf(TEXT(" %c"), aux->player[m].board[i][j]);
			}
		}
		_tprintf(TEXT("\n"));
	}
}

DWORD putWall( MemDados* aux, DWORD posX, DWORD posY) {

	WaitForSingleObject(aux->mutexBoard, INFINITE);
	for (DWORD i = 0; i < aux->VBoard->numP; i++) {
		if (posX > aux->VBoard->actualSize || posY > aux->VBoard->actualSize || posX < 0 || posY < 0 || aux->VBoard->player[i].board[posX][posY] != '.') {
			
			_ftprintf(stderr, TEXT("-----------> Cant place wall here: [%d][%d]\n"), posX, posY);
		}
		else {
			aux->VBoard->player[i].board[posX][posY] = 'w';
			_ftprintf(stderr, TEXT("-----------> Placed wall at [%d][%d]\n"), posX, posY);
		}
	
	}
	ReleaseMutex(aux->mutexBoard);
	return 1;
}
