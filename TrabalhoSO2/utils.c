#include "utils.h"

void setupBoard(MemDados* aux, DWORD actualSize,DWORD np) {
	srand(time(NULL));

	WaitForSingleObject(aux->mutexBoard, INFINITE);
	for(DWORD p = 0; p<np;p++){
		aux->VBoard->actualSize = actualSize;
		aux->VBoard->player[p].win = 0;
		aux->VBoard->player[p].actualSize = actualSize;

		for (DWORD i = 0; i < aux->VBoard->actualSize; i++) {
			for (DWORD j = 0; j < aux->VBoard->actualSize; j++)
			{
				aux->VBoard->player[p].board[i][j] = '.';
			}
		}

		DWORD lineBegin1 = rand() % aux->VBoard->actualSize;
		DWORD lineEnd1 = rand() % aux->VBoard->actualSize;

		DWORD lineBegin = (rand() % ((aux->VBoard->actualSize - 2) - 1 + 1)) + 1;
		DWORD lineEnd = (rand() % ((aux->VBoard->actualSize - 2) - 1 + 1)) + 1;

		DWORD startingSide = rand() % 4; // respetivo canto 0=←, 2=↑, 1=→, 3=↓

		switch (startingSide)
		{
		case 0: //canto esquerdo 
			aux->VBoard->player[0].board[lineBegin][0] = 'z';
			aux->VBoard->player[1].board[lineBegin][0] = 'z';

			aux->VBoard->player[0].board[lineEnd][aux->VBoard->actualSize-1] = 'e';
			aux->VBoard->player[1].board[lineEnd][aux->VBoard->actualSize-1] = 'e';

			aux->VBoard->player[0].sentido = '→';
			aux->VBoard->player[1].sentido = '→';

			aux->VBoard->player[0].begin[0] = lineBegin;
			aux->VBoard->player[0].begin[1] = 0;

			aux->VBoard->player[1].begin[0] = lineBegin;
			aux->VBoard->player[1].begin[1] = 0;
			break;
		case 1: //canto direito
			aux->VBoard->player[0].board[lineBegin][aux->VBoard->actualSize-1] = 'z';
			aux->VBoard->player[1].board[lineBegin][aux->VBoard->actualSize-1] = 'z';

			aux->VBoard->player[0].board[lineEnd][0] = 'e';
			aux->VBoard->player[1].board[lineEnd][0] = 'e';

			aux->VBoard->player[0].sentido = '←';
			aux->VBoard->player[1].sentido = '←';

			aux->VBoard->player[0].begin[0] = lineBegin;
			aux->VBoard->player[0].begin[1] = aux->VBoard->actualSize;


			aux->VBoard->player[1].begin[0] = lineBegin;
			aux->VBoard->player[1].begin[1] = aux->VBoard->actualSize;
			break;

		case 2: //canto superior
			aux->VBoard->player[0].board[0][lineBegin] = 'x';
			aux->VBoard->player[1].board[0][lineBegin] = 'x';

			aux->VBoard->player[0].board[aux->VBoard->actualSize-1][lineEnd] = 'e';
			aux->VBoard->player[1].board[aux->VBoard->actualSize-1][lineEnd] = 'e';

			aux->VBoard->player[0].sentido = '↓';
			aux->VBoard->player[1].sentido = '↓';

			aux->VBoard->player[0].begin[0] = 0;
			aux->VBoard->player[0].begin[1] = lineBegin;

			aux->VBoard->player[1].begin[0] = 0;
			aux->VBoard->player[1].begin[1] = lineBegin;
			break;

		case 3: //canto inferior
			aux->VBoard->player[0].board[aux->VBoard->actualSize-1][lineBegin] = 'x';
			aux->VBoard->player[1].board[aux->VBoard->actualSize-1][lineBegin] = 'x';

			aux->VBoard->player[0].board[0][lineEnd] = 'e';
			aux->VBoard->player[1].board[0][lineEnd] = 'e';

			aux->VBoard->player[0].sentido = '↑';
			aux->VBoard->player[1].sentido = '↑';

			aux->VBoard->player[0].begin[0] = aux->VBoard->actualSize;
			aux->VBoard->player[0].begin[1] = lineBegin;

			aux->VBoard->player[1].begin[0] = aux->VBoard->actualSize;
			aux->VBoard->player[1].begin[1] = lineBegin;
			break;
		default:
			break;
		}
		aux->VBoard->player[0].lastInsert = 'B';
		aux->VBoard->player[1].lastInsert = 'B';

	}
	ReleaseMutex(aux->mutexBoard);

}

DWORD insertWater(PLAYER* pData) 
{
	if (pData->lastInsert == 'B') {

		if (pData->sentido == '←') {
			if (pData->board[pData->begin[0]][pData->begin[1] - 1] == 'z') {
				pData->sentido = '←';
				pData->lastInsert = 'z';
			}
			else if (pData->board[pData->begin[0]][pData->begin[1] - 1] == 's') {
				pData->sentido = '↓';
				pData->lastInsert = 'z';
			}
			else if (pData->board[pData->begin[0]][pData->begin[1] - 1] == 'r') {
				pData->sentido = '↑';
				pData->lastInsert = 'z';
			}


		}
		else if (pData->sentido == '→') {
			if (pData->board[pData->begin[0]][pData->begin[1] + 1] == 'z') {
				pData->sentido = '→';
				pData->lastInsert = 'z';
			}
			else if (pData->board[pData->begin[0]][pData->begin[1] + 1] == 'd') {
				pData->sentido = '↓';
				pData->lastInsert = 'z';
			}
			else if (pData->board[pData->begin[0]][pData->begin[1] + 1] == 'l') {
				pData->sentido = '↑';
				pData->lastInsert = 'z';
			}


		}
		else if (pData->sentido == '↑') {
			if (pData->board[pData->begin[0] - 1][pData->begin[1]] == 'x') {
				pData->sentido = '↑';
				pData->lastInsert = 'x';
			}
			else if (pData->board[pData->begin[0] - 1][pData->begin[1]] == 'd') {
				pData->sentido = '↓';
				pData->lastInsert = 'x';
			}
			else if (pData->board[pData->begin[0] - 1][pData->begin[1]] == 's') {
				pData->sentido = '↑';
				pData->lastInsert = 'x';
			}


		}
		else if (pData->sentido == '↓') {
			if (pData->board[pData->begin[0] + 1][pData->begin[1]] == 'x') {
				pData->sentido = '↓';
				pData->lastInsert = 'x';
			}
			else if (pData->board[pData->begin[0] + 1][pData->begin[1]] == 'r') {
				pData->sentido = '↓';
				pData->lastInsert = 'x';
			}
			else if (pData->board[pData->begin[0] + 1][pData->begin[1]] == 'l') {
				pData->sentido = '↑';
				pData->lastInsert = 'x';
			}


		}

		pData->lastWaterXY[0] = pData->begin[0];
		pData->lastWaterXY[1] = pData->begin[1];

		if (pData->lastInsert == 'x') {
			pData->board[pData->begin[0]][pData->begin[1]] = 'X';
			return 0;
		}
		else if (pData->lastInsert == 'z')
		{
			pData->board[pData->begin[0]][pData->begin[1]] = 'Z';
			return 0;
		}

		return -1;

	}
	else {

		if (pData->lastWaterXY[0] == 0 && pData->lastWaterXY[1] < pData->actualSize - 1 && pData->lastWaterXY[1] > 0) {
			switch (pData->lastInsert)
			{
			case 'z':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				else if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] = 'a';
						return 1;
					}
				}
				break;
			case 'x':
				if (pData->sentido == '↑') {
					return -1;
				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						pData->sentido = '→';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						pData->sentido = '←';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}

				}
				break;
			case 's':
				if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						pData->sentido = '→';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						pData->sentido = '←';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				else if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						return -1;
					}
				}
				break;
			case 'r':
				return -1;
				break;
			case 'd':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						pData->sentido = '←';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						pData->sentido = '→';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				break;
			case 'l':
				return -1;
				break;
			}
		}
		else if (pData->lastWaterXY[0] == pData->actualSize - 1 && pData->lastWaterXY[1] < pData->actualSize - 1 && pData->lastWaterXY[1] > 0) {
			switch (pData->lastInsert)
			{
			case 'z':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				else if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] = 'a';
						return 1;
					}
				}
				break;
			case 'x':
				return -1;
				break;
			case 's':
				return -1;
				break;
			case 'r':
				return -1;
				break;
			case 'd':
				return -1;
				break;
			case 'l':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
						return 1;
					}

				}
				else if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						pData->sentido = '←';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						pData->sentido = '→';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
				}

				break;
			}
		}
		else if (pData->lastWaterXY[1] == 0 && pData->lastWaterXY[0] < pData->actualSize - 1 && pData->lastWaterXY[0] > 0) {
			switch (pData->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						pData->sentido = '→';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						pData->sentido = '→';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				break;
			case 's':
				if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						pData->sentido = '→';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] = 'a';
						return 1;
					}

				}
				break;
			case 'r':
				if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				else if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						pData->sentido = '→';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] = 'a';
						return 1;
					}

				}
				break;
			case 'd':
				return -1;
				break;
			case 'l':
				return -1;
				break;
			}
		}
		else if (pData->lastWaterXY[1] == pData->actualSize - 1 && pData->lastWaterXY[0] < pData->actualSize - 1 && pData->lastWaterXY[0] > 0) {
			switch (pData->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						pData->sentido = '←';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						pData->sentido = '←';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'a';
						return 1;
					}

				}
				break;
			case 's':
				return -1;
				break;
			case 'r':
				return -1;
				break;
			case 'd':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}

				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						pData->sentido = '←';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						return -1;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'a';
						return 1;

					}

				}
				break;
			case 'l':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}

				}
				else if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						pData->sentido = '←';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						return -1;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'a';
						return 1;

					}

				}
				break;
			}
		}
		else if (pData->lastWaterXY[0] == 0 && pData->lastWaterXY[1] == 0) {
			switch (pData->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				return -1;
				break;
			case 's':
				if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						pData->sentido = '→';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] = 'a';
						return 1;
					}
				}
				break;
			case 'r':
				return -1;
				break;
			case 'd':
				return -1;
				break;
			case 'l':
				return -1;
				break;
			}
		}
		else if (pData->lastWaterXY[0] == 0 && pData->lastWaterXY[1] == pData->actualSize - 1) {
			switch (pData->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				return -1;
				break;
			case 's':
				return -1;
				break;
			case 'r':
				return -1;
				break;
			case 'd':
				if (pData->sentido == '←') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
						pData->sentido = '←';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
						pData->sentido = '↓';

						pData->lastWaterXY[1]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
						return 1;
					}

				}
				else if (pData->sentido == '↓') {
					if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↓';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
						pData->sentido = '←';

						pData->lastWaterXY[0]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] = 'a';
						return 1;
					}
				}
				break;
			case 'l':
				return -1;
				break;
			}
		}
		else if (pData->lastWaterXY[0] == pData->actualSize - 1 && pData->lastWaterXY[1] == 0) {
			switch (pData->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				return -1;
				break;
			case 's':
				return -1;
				break;
			case 'r':
				if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				else if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						pData->sentido = '→';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] = 'a';
						return 1;
					}
				}
				break;
			case 'd':
				return -1;
				break;
			case 'l':
				return -1;
				break;
			}
		}
		else if (pData->lastWaterXY[0] == pData->actualSize - 1 && pData->lastWaterXY[1] == pData->actualSize - 1) {
			switch (pData->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				return -1;
				break;
			case 's':
				return -1;
				break;
			case 'r':
				return -1;
				break;
			case 'd':
				return -1;
				break;
			case 'l':
				if (pData->sentido == '→') {
					if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
						pData->sentido = '→';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
						pData->sentido = '↑';

						pData->lastWaterXY[1]++;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				if (pData->sentido == '↑') {
					if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
						pData->sentido = '↑';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
						pData->sentido = '←';

						pData->lastWaterXY[0]--;
						pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

						pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
						return -1;
					}
					else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
						pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] = 'a';
						return 1;
					}

				}
				else if (pData->sentido == '←') {
				}
				break;
			}
		}
		else {
		switch (pData->lastInsert)
		{
		case 'z':
			if (pData->sentido == '←') {
				if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
					pData->sentido = '←';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
					pData->sentido = '↓';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
					pData->sentido = '↑';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
					return 1;
				}
			}
			else if (pData->sentido == '→') {
				if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
					pData->sentido = '→';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
					pData->sentido = '↓';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
					pData->sentido = '↑';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
					return 1;
				}
			}
			break;
		case 'x':
			if (pData->sentido == '↑') {
				if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
					pData->sentido = '↑';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
					pData->sentido = '→';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
					pData->sentido = '←';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'a';
					return 1;
				}
			}
			else if (pData->sentido == '↓') {
				if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
					pData->sentido = '↓';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
					pData->sentido = '→';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
					pData->sentido = '←';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'a';
					return 1;
				}
			}
			break;
		case 's':
			if (pData->sentido == '→') {
				if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
					pData->sentido = '→';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
					pData->sentido = '↓';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
					pData->sentido = '↑';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'e') {
					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] = 'a';
					return 1;
				}

			}
			else if (pData->sentido == '↓') {
				if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
					pData->sentido = '↓';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
					pData->sentido = '→';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
					pData->sentido = '←';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] = 'a';
					return 1;
				}
			}
			break;
		case 'r':
			if (pData->sentido == '→') {
				if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'z') {
					pData->sentido = '→';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'd') {
					pData->sentido = '↓';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] + 1] == 'l') {
					pData->sentido = '↑';

					pData->lastWaterXY[1]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
			}
			else if (pData->sentido == '↑') {
				if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
					pData->sentido = '↑';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
					pData->sentido = '→';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
					pData->sentido = '←';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] = 'a';
					return 1;
				}
				break;
		case 'd':
			if (pData->sentido == '←') {
				if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
					pData->sentido = '←';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
					pData->sentido = '↓';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
					pData->sentido = '↑';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'e') {
					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
					return 1;
				}
			}
			if (pData->sentido == '↓') {
				if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'x') {
					pData->sentido = '↓';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'l') {
					pData->sentido = '←';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'r') {
					pData->sentido = '→';

					pData->lastWaterXY[0]++;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] + 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] = 'a';
					return 1;
				}
			}
			break;
		case 'l':
			if (pData->sentido == '←') {
				if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'z') {
					pData->sentido = '←';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 's') {
					pData->sentido = '↓';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'r') {
					pData->sentido = '↑';

					pData->lastWaterXY[1]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;
				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1] - 1] == 'a';
					return 1;

				}

			}
			else if (pData->sentido == '↑') {
				if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'x') {
					pData->sentido = '↑';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'd') {
					pData->sentido = '←';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 's') {
					pData->sentido = '→';

					pData->lastWaterXY[0]--;
					pData->lastInsert = pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]];

					pData->board[pData->lastWaterXY[0]][pData->lastWaterXY[1]] = 'a';
					return 0;

				}
				else if (pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'e') {
					pData->board[pData->lastWaterXY[0] - 1][pData->lastWaterXY[1]] == 'a';
					return 1;

				}

			}
			break;

			}
			}
		}
	}
}
		DWORD placePeca(Board *b,MemDados *aux, char peca, int posX, int posY, int player) {
		
			WaitForSingleObject(aux->mutexBoard, INFINITE);
			for (DWORD i = 0; i < b->numP;i++) {
				if (posX > aux->VBoard->actualSize || posY > aux->VBoard->actualSize || posX < 0 || posY < 0 || aux->VBoard->player[i].board[posX][posY] != '.') {
					ReleaseMutex(aux->mutexBoard);
					_ftprintf(stderr, TEXT("-----------> Cant place wall here: [%d][%d]\n"), posX, posX);
					return -1;
				}
				else {
					aux->VBoard->player[i].board[posX][posY] = 'W';
					_ftprintf(stderr, TEXT("-----------> Placed wall at [%d][%d]\n"), posX, posX);
				}
			}
			

			ReleaseMutex(aux->mutexBoard);
			
		}

		void printBoard(Board * aux) {
			for (DWORD i = 0; i < aux->numP; i++) {
				_tprintf(TEXT("--------PLAYER [i]---------"));
				for (DWORD i = 0; i < aux->actualSize; i++) {
					_tprintf(TEXT("\n"));
					for (DWORD j = 0; j < aux->actualSize; j++)
					{
						_tprintf(TEXT(" %c"), aux->player[i].board[i][j]);
					}
				}
				_tprintf(TEXT("\n"));
			}
		}

		DWORD putWall(Board* b, MemDados* aux, DWORD posX, DWORD posY) {
		
			WaitForSingleObject(aux->mutexBoard, INFINITE);
			for (DWORD i = 0; i < b->numP; i++) {
				if (posX > aux->VBoard->actualSize || posY > aux->VBoard->actualSize || posX < 0 || posY < 0 || aux->VBoard->player[i].board[posX][posY] != '.') {
					ReleaseMutex(aux->mutexBoard);
					_ftprintf(stderr, TEXT("-----------> Cant place wall here: [%d][%d]\n"), posX, posX);
					return -1;
				}
				else {
					aux->VBoard->player[i].board[posX][posY] = 'W';
					_ftprintf(stderr, TEXT("-----------> Placed wall at [%d][%d]\n"), posX, posX);
				}
			}
			ReleaseMutex(aux->mutexBoard);
			return 1;
		}