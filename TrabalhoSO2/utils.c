#include "utils.h"

void setupBoard(MemDados* aux, DWORD actualSize, DWORD numPlayers) {
	srand(time(NULL));
	DWORD goUp;

	WaitForSingleObject(aux->mutexBoard, INFINITE);
	aux->VBoard->actualSize = actualSize;
	aux->VBoard->player[0].win = 0;
	aux->VBoard->player[1].win = 0;

	

	for (DWORD p = 0; p < numPlayers; p++) {
		for (DWORD i = 0; i < aux->VBoard->actualSize; i++) {
			for (DWORD j = 0; j < aux->VBoard->actualSize; j++)
			{
				aux->VBoard->player[0].board[i][j] = '.';
			}
		}
	}
	

	DWORD lineBegin = rand() % aux->VBoard->actualSize;
	DWORD lineEnd = rand() % aux->VBoard->actualSize;


	for (DWORD p = 0; p < numPlayers; p++) {
		aux->VBoard->player[p].begin[0] = lineBegin;
		aux->VBoard->player[p].begin[1] = 0;
		aux->VBoard->player[p].end[0] = lineEnd;
		aux->VBoard->player[p].end[1] = aux->VBoard->actualSize;
	}


/*
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


		aux->VBoard->board1[lineBegin][0] = 'B'; //begin position
		aux->VBoard->board1[lineEnd][aux->VBoard->actualSize - 1] = 'E'; //end position


		for (DWORD j = 1; j < aux->VBoard->actualSize - 1; j++) {
			aux->VBoard->board1[lineBegin][j] = '-';
		}

		switch (goUp) {
		case 1:
			aux->VBoard->board1[lineBegin][aux->VBoard->actualSize - 1] = 'S';

			for (DWORD i = lineBegin - 1; i > lineEnd; i--) {
				aux->VBoard->board1[i][aux->VBoard->actualSize - 1] = '|';
			}
			break;
		case -1:
			aux->VBoard->board1[lineBegin][aux->VBoard->actualSize - 1] = 'D';

			for (DWORD i = lineBegin + 1; i < lineEnd; i++) {
				aux->VBoard->boar1[i][aux->VBoard->actualSize - 1] = '|';
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
	*/
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

DWORD insertWater(Board* board,DWORD p) {



	if (board->player[p].lastInsert == 'B') {

		board->player[p].lastWaterXY[0] = board->player[p].begin[0];
		board->player[p].lastWaterXY[1] = board->player[p].begin[1];

		if (board->player[p].board[board->player[p].begin[0]][board->player[p].begin[1] + 1] == '-')
			board->player[p].lastInsert = '-';
		else
			board->player[p].lastInsert = '|';

		board->player[p].board[board->player[p].begin[0]][board->player[p].begin[1]] = '*';

		return 0;

	}
	else {

		//_tprintf(TEXT("\nUltimo tubo: (%c) [%d][%d] \n"), board->player[p].lastInsert, board->player[p].lastWaterXY[0] + 1, board->player[p].lastWaterXY[1] + 1);

		switch (board->player[p].lastInsert)
		{
		case '-': {
			//topo
			if (board->player[p].lastWaterXY[0] == 0) {
				//direita
				if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == '-' || board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'S' || board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'D') {

					board->player[p].lastWaterXY[1]++;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] = '*';
					return 1;
				}

				//esquerda

				//baixo
				else
					return -1;

			}
			//Fundo
			else if (board->player[p].lastWaterXY[0] == board->actualSize - 1) {
				//direita
				if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == '-' || board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'S' || board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'D') {

					board->player[p].lastWaterXY[1]++;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] = '*';
					return 1;
				}
				//esquerda
				if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] - 1] == '-') {

					board->player[p].lastWaterXY[1]--;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] - 1] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] - 1] = '*';
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
				if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == '-' || board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'S' || board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'D') {

					board->player[p].lastWaterXY[1]++;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] = '*';
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
			if (board->player[p].lastWaterXY[0] == 0) {
				if (board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == '|') {

					board->player[p].lastWaterXY[0]++;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
			}

			//Fundo
			else if (board->player[p].lastWaterXY[0] == board->actualSize - 1) {
				return -1;
			}
			//meio
			else
			{
				//cima
				if (board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == '|' || board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == '┏' || board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == '┓') {

					board->player[p].lastWaterXY[0]--;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] = '*';
					return 1;
				}

				//baixo
				if (board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == '|' || board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == 'L' || board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == 'R') {
					board->player[p].lastWaterXY[0]++;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] = '*';
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
			if (board->player[p].lastWaterXY[0] == 0) {
				return -1;
			}
			else if (board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == 'E') {
				board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] = '*';
				return 1;
			}
			//fundo
			//meio
			else {
				if (board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == '|' || board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == '┏' || board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == '┓') {
					board->player[p].lastWaterXY[0]--;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0] - 1][board->player[p].lastWaterXY[1]] = '*';
					return 1;
				}
			}

			break;
		}

				// ┓
		case 'D': {
			//fundo
			if (board->player[p].lastWaterXY[0] == board->actualSize - 1) {
				return -1;
			}
			else if (board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == 'E') {
				board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] = '*';
				return 1;
			}
			//topo/meio
			else
			{
				if (board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == '|' || board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == '┛' || board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == '┗') {

					board->player[p].lastWaterXY[0]++;

					board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

					board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
					return 0;
				}
				else if (board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] == 'E') {
					board->player[p].board[board->player[p].lastWaterXY[0] + 1][board->player[p].lastWaterXY[1]] = '*';
					return 1;
				}

			}

			break;
		}


		case 'L': {
			if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] - 1] == '-') {

				board->player[p].lastWaterXY[1]--;

				board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

				board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
				return 0;
			}
			else if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] - 1] == 'E') {
				board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]-1] = '*';
				return 1;
			}
			break;
		}


		case 'R': {
			if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == '-') {

				board->player[p].lastWaterXY[1]++;

				board->player[p].lastInsert = board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]];

				board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1]] = '*';
				return 0;
			}
			else if (board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] == 'E') {
				board->player[p].board[board->player[p].lastWaterXY[0]][board->player[p].lastWaterXY[1] + 1] = '*';
				return 1;
			}
			break;
		}


		default:

			break;
		}

	}

	return -2;
}

void printBoard(Board* aux,DWORD p) {

	

	for (DWORD l = 0; l < p; l++) {
		_tprintf(TEXT(" -----------------PLAYER[p]-----------------"));
		for (DWORD i = 0; i < aux->actualSize; i++) {
			_tprintf(TEXT("\n"));
			for (DWORD j = 0; j < aux->actualSize; j++)
			{
				_tprintf(TEXT(" %c"), aux->player[p].board[i][j]);
			}
		}
		_tprintf(TEXT("\n"));
	
	}
}

DWORD putWall(MemDados* aux, DWORD posX, DWORD posY,DWORD p) {
	WaitForSingleObject(aux->mutexBoard, INFINITE);

	for (DWORD l = 0; l < p; l++) {
	if (posX > aux->VBoard->actualSize || posY > aux->VBoard->actualSize || posX < 0 || posY < 0 || aux->VBoard->player[p].board[posX][posY] != '.') {
		ReleaseMutex(aux->mutexBoard);
		_ftprintf(stderr, TEXT("-----------> Cant place wall here: [%d][%d]\n"), posX, posX);
		return -1;
	}else {
		aux->VBoard->player[p].board[posX][posY] = 'W';
		_ftprintf(stderr, TEXT("-----------> Placed wall at [%d][%d]\n"), posX, posX);
	}
	}
	ReleaseMutex(aux->mutexBoard);
	return 1;
}
