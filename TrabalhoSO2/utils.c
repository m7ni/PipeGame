#include "utils.h"

void setupBoard(MemDados* aux, DWORD actualSize) {
	srand(time(NULL));
	DWORD goUp;

	WaitForSingleObject(aux->mutexBoard, INFINITE);
	aux->VBoard->actualSize = actualSize;
	aux->VBoard->win = 0;
	for (DWORD i = 0; i < aux->VBoard->actualSize; i++) {
		for (DWORD j = 0; j < aux->VBoard->actualSize; j++)
		{
			aux->VBoard->board[i][j] = '.';
		}
	}

	DWORD lineBegin = rand() % aux->VBoard->actualSize;
	DWORD lineEnd = rand() % aux->VBoard->actualSize;

	DWORD VoH = rand() % 2;
	//DWORD VoH = 1;
	//_tprintf(TEXT("RAND> %d"), VoH);

	//DWORD lineBegin = 1;
	//DWORD lineEnd = 15;


	if (lineEnd > lineBegin) //temos que ir para baixo quando chegar ao lado direito da tabela
	{
		goUp = -1;
	}
	else if (lineEnd < lineBegin) {
		goUp = 1;
	} //temos que ir para cim quando chegar ao lado direito da tabela
	else {
		goUp = 0;
	}

	if (VoH == 0) { //caminho na horizontal 
		aux->VBoard->begin[0] = lineBegin;
		aux->VBoard->begin[1] = 0;
		aux->VBoard->lastInsert = 'B';


		aux->VBoard->board[lineBegin][0] = 'B'; //begin position
		aux->VBoard->board[lineEnd][aux->VBoard->actualSize - 1] = 'E'; //end position


		for (DWORD j = 1; j < aux->VBoard->actualSize - 1; j++) {
			aux->VBoard->board[lineBegin][j] = '-';
		}

		switch (goUp) {
		case 1:
			aux->VBoard->board[lineBegin][aux->VBoard->actualSize - 1] = 'S';

			for (DWORD i = lineBegin - 1; i > lineEnd; i--) {
				aux->VBoard->board[i][aux->VBoard->actualSize - 1] = '|';
			}
			break;
		case -1:
			aux->VBoard->board[lineBegin][aux->VBoard->actualSize - 1] = 'D';

			for (DWORD i = lineBegin + 1; i < lineEnd; i++) {
				aux->VBoard->board[i][aux->VBoard->actualSize - 1] = '|';
			}
			break;

		default:
			break;
		}

	}
	else { //caminho na vertical

		aux->VBoard->begin[0] = 0;
		aux->VBoard->begin[1] = lineBegin;
		aux->VBoard->lastInsert = 'B';


		aux->VBoard->board[0][lineBegin] = 'B'; //begin position
		aux->VBoard->board[aux->VBoard->actualSize - 1][lineEnd] = 'E'; //end position

		for (DWORD j = 1; j < aux->VBoard->actualSize - 1; j++) {
			aux->VBoard->board[j][lineBegin] = '|';
		}

		switch (goUp) {
		case 1:
			aux->VBoard->board[aux->VBoard->actualSize - 1][lineBegin] = 'L';

			for (DWORD i = lineBegin - 1; i > lineEnd; i--) {
				aux->VBoard->board[aux->VBoard->actualSize - 1][i] = '-';
			}
			break;
		case -1:
			aux->VBoard->board[aux->VBoard->actualSize - 1][lineBegin] = 'R';

			for (DWORD i = lineBegin + 1; i < lineEnd; i++) {
				aux->VBoard->board[aux->VBoard->actualSize - 1][i] = '-';
			}
			break;

		default:
			break;
		}

	}

	ReleaseMutex(aux->mutexBoard);

	/*
	aux->pecas[0] = '━';
	aux->pecas[1] = '┃';
	aux->pecas[2] = '┏';
	aux->pecas[3] = '━';
	aux->pecas[4] = '┛';
	aux->pecas[5] = '┗';
	*/



}

DWORD insertWater(Board* board) {
	if (board->lastInsert == 'B') {

		board->lastWaterXY[0] = board->begin[0];
		board->lastWaterXY[1] = board->begin[1];

		if (board->board[board->begin[0]][board->begin[1] + 1] == '-')
			board->lastInsert = '-';
		else
			board->lastInsert = '|';

		board->board[board->begin[0]][board->begin[1]] = '*';

		return 0;

	}
	else {

		//_tprintf(TEXT("\nUltimo tubo: (%c) [%d][%d] \n"), board->lastInsert, board->lastWaterXY[0] + 1, board->lastWaterXY[1] + 1);

		switch (board->lastInsert)
		{
		case '-': {
			//topo
			if (board->lastWaterXY[0] == 0) {
				//direita
				if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == '-' || board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'S' || board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'D') {

					board->lastWaterXY[1]++;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'E') {
					board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = '*';
					return 1;
				}

				//esquerda

				//baixo
				else
					return -1;

			}
			//Fundo
			else if (board->lastWaterXY[0] == board->actualSize - 1) {
				//direita
				if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == '-' || board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'S' || board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'D') {

					board->lastWaterXY[1]++;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'E') {
					board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = '*';
					return 1;
				}
				//esquerda
				if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == '-') {

					board->lastWaterXY[1]--;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'E') {
					board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = '*';
					return 1;
				}
				//cima
				else
					return -1;
			}

			//meio
			else
			{
				//direita
				if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == '-' || board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'S' || board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'D') {

					board->lastWaterXY[1]++;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'E') {
					board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = '*';
					return 1;
				}
				//esquerda

				//cima

				//baixo
				else
					return -1;
			}

			break;
		}

		case '|': {

			//topo
			if (board->lastWaterXY[0] == 0) {
				if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == '|') {

					board->lastWaterXY[0]++;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
			}

			//Fundo
			else if (board->lastWaterXY[0] == board->actualSize - 1) {
				return -1;
			}
			//meio
			else
			{
				//cima
				if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == '|' || board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == '┏' || board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == '┓') {

					board->lastWaterXY[0]--;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'E') {
					board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] = '*';
					return 1;
				}

				//baixo
				if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == '|' || board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'L' || board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'R') {
					board->lastWaterXY[0]++;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'E') {
					board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] = '*';
					return 1;
				}
				else
					return -1;
			}

			break;
		}

				// ┛
		case 'S': {
			//topo
			if (board->lastWaterXY[0] == 0) {
				return -1;
			}
			else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'E') {
				board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] = '*';
				return 1;
			}
			//fundo
			//meio
			else {
				if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == '|' || board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == '┏' || board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == '┓') {
					board->lastWaterXY[0]--;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}
			}

			break;
		}

				// ┓
		case 'D': {
			//fundo
			if (board->lastWaterXY[0] == board->actualSize - 1) {
				return -1;
			}
			else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'E') {
				board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] = '*';
				return 1;
			}
			//topo/meio
			else
			{
				if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == '|' || board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == '┛' || board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == '┗') {

					board->lastWaterXY[0]++;

					board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

					board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
					return 0;
				}

			}

			break;
		}


		case 'L': {
			if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == '-') {

				board->lastWaterXY[1]--;

				board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

				board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
				return 0;
			}
			break;
		}


		case 'R': {
			if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == '-') {

				board->lastWaterXY[1]++;

				board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

				board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = '*';
				return 0;
			}
			break;
		}


		default:

			break;
		}

	}

	return -2;
}

void printBoard(Board* aux) {

	_tprintf(TEXT("-----------------"));
	for (DWORD i = 0; i < aux->actualSize; i++) {
		_tprintf(TEXT("\n"));
		for (DWORD j = 0; j < aux->actualSize; j++)
		{
			_tprintf(TEXT(" %c"), aux->board[i][j]);
		}
	}
	_tprintf(TEXT("\n"));
}

DWORD putWall(MemDados* aux, DWORD pos[2]) {
	WaitForSingleObject(aux->mutexBoard, INFINITE);
	if (pos[1] > aux->VBoard->actualSize || pos[0] > aux->VBoard->actualSize) {
		ReleaseMutex(aux->mutexBoard);
		return -1;
	}
	if (aux->VBoard->board[pos[0]][pos[1]] != '.') {
		ReleaseMutex(aux->mutexBoard);
		return -1;
	}
		
	aux->VBoard->board[pos[0]][pos[1]] == 'W';

	ReleaseMutex(aux->mutexBoard);
	return 1;
}
