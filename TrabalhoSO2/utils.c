#include "utils.h"

void setupBoard(MemDados* aux, DWORD actualSize) {
	srand(time(NULL));

	WaitForSingleObject(aux->mutexBoard, INFINITE);

	aux->VBoard->actualSize = actualSize;
	aux->VBoard->win = 0;

	for (DWORD i = 0; i < aux->VBoard->actualSize; i++) {
		for (DWORD j = 0; j < aux->VBoard->actualSize; j++)
		{
			aux->VBoard->board[i][j] = '.';
		}
	}

	DWORD lineBegin1 = rand() % aux->VBoard->actualSize;
	DWORD lineEnd1 = rand() % aux->VBoard->actualSize;

	DWORD lineBegin = (rand() %((aux->VBoard->actualSize - 2) - 1 + 1)) + 1;
	DWORD lineEnd = (rand() % ((aux->VBoard->actualSize - 2)- 1 + 1)) + 1;
	
	DWORD startingSide = rand() % 4; // respetivo canto 0=←, 2=↑, 1=→, 3=↓
	
	switch (startingSide)
	{
	case 0: //canto esquerdo 
		aux->VBoard->board[lineBegin][0] = '-';
		aux->VBoard->board[lineEnd][aux->VBoard->actualSize] = 'e';
		aux->VBoard->sentido = '→';
		aux->VBoard->begin[0] = lineBegin;
		aux->VBoard->begin[1] = 0;
		break;
	case 1: //canto direito
		aux->VBoard->board[lineBegin][aux->VBoard->actualSize] = '-';
		aux->VBoard->board[lineEnd][0] = 'e';
		aux->VBoard->sentido = '←';
		aux->VBoard->begin[0] = lineBegin;
		aux->VBoard->begin[1] = aux->VBoard->actualSize;
		break;

	case 2: //canto superior
		aux->VBoard->board[0][lineBegin] = '|';
		aux->VBoard->board[aux->VBoard->actualSize][lineEnd] = 'e';
		aux->VBoard->sentido = '↓';
		aux->VBoard->begin[0] = 0;
		aux->VBoard->begin[1] = lineBegin;
		break;
	
	case 3: //canto inferior
		aux->VBoard->board[aux->VBoard->actualSize][lineBegin] = '|';
		aux->VBoard->board[0][lineEnd] = 'e';
		aux->VBoard->sentido = '↑';
		aux->VBoard->begin[0] = aux->VBoard->actualSize;
		aux->VBoard->begin[1] = lineBegin;
		break;
	default:
		break;
	}
	aux->VBoard->lastInsert = 'B';
	

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

		if (board->sentido == '←') {
			if (board->board[board->begin[0]][board->begin[1] - 1] == 'z') {
				board->sentido = '←';
				board->lastInsert = 'z';
			}
			else if (board->board[board->begin[0]][board->begin[1] - 1] == 's'){
				board->sentido = '↓';
				board->lastInsert = 'z';
			}
			else if (board->board[board->begin[0]][board->begin[1] - 1] == 'r') {
				board->sentido = '↑';
				board->lastInsert = 'z';
			}

			
		}
		else if (board->sentido == '→') {
			if (board->board[board->begin[0]][board->begin[1] + 1] == 'z') {
				board->sentido = '→';
				board->lastInsert = 'z';
			}
			else if (board->board[board->begin[0]][board->begin[1] + 1] == 'd') {
				board->sentido = '↓';
				board->lastInsert = 'z';
			}
			else if (board->board[board->begin[0]][board->begin[1] + 1] == 'l') {
				board->sentido = '↑';
				board->lastInsert = 'z';
			}

			
		}
		else if (board->sentido == '↑') {
			if (board->board[board->begin[0] - 1][board->begin[1]] == 'x') {
				board->sentido = '↑';
				board->lastInsert = 'x';
			}
			else if (board->board[board->begin[0] - 1][board->begin[1]] == 'd') {
				board->sentido = '↓';
				board->lastInsert = 'x';
			}
			else if (board->board[board->begin[0] - 1][board->begin[1]] == 's') {
				board->sentido = '↑';
				board->lastInsert = 'x';
			}

			
		}
		else if (board->sentido == '↓') {
			if (board->board[board->begin[0] + 1][board->begin[1]] == 'x') {
				board->sentido = '↓';
				board->lastInsert = 'x';
			}
			else if (board->board[board->begin[0] + 1][board->begin[1]] == 'r') {
				board->sentido = '↓';
				board->lastInsert = 'x';
			}
			else if (board->board[board->begin[0] + 1][board->begin[1]] == 'l') {
				board->sentido = '↑';
				board->lastInsert = 'x';
			}

			
		}
	
		board->lastWaterXY[0] = board->begin[0];
		board->lastWaterXY[1] = board->begin[1];

		if (board->lastInsert == 'x') {
			board->board[board->begin[0]][board->begin[1]] = 'X';
			return 0;
		}else if (board->lastInsert == 'z')
		{
			board->board[board->begin[0]][board->begin[1]] = 'Z';
			return 0;
		}

		return -1;

	}
	else {

		if (board->lastWaterXY[0] == 0 && board->lastWaterXY[1] < board->actualSize - 1 && board->lastWaterXY[1] > 0) {
				switch (board->lastInsert)
				{
				case 'z':
					if (board->sentido == '←') {
						if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
							board->sentido = '←';

							board->lastWaterXY[1]--;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
							board->sentido = '↓';

							board->lastWaterXY[1]--;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
							return -1;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
							board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
							return 1;
						}
					}
					else if (board->sentido == '→') {
						if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
							board->sentido = '→';

							board->lastWaterXY[1]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
							board->sentido = '↓';

							board->lastWaterXY[1]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
							return -1;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
							board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = 'a';
							return 1;
						}
					}
					break;
				case 'x':
					if (board->sentido == '↑') {
							return -1;
					}
					else if (board->sentido == '↓') {
						if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1] ] == 'x') {
							board->sentido = '↓';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1] ] == 'r') {
							board->sentido = '→';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1] ] == 'l') {
							board->sentido = '←';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
					
					}
					break;
				case 's':
					if (board->sentido == '↓'){
						if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
							board->sentido = '↓';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
							board->sentido = '→';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
							board->sentido = '←';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
					}
					else if (board->sentido == '→') {
						if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
							board->sentido = '→';

							board->lastWaterXY[1]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
							board->sentido = '↓';

							board->lastWaterXY[1]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
							return -1;
						}
					}
					break;
				case 'r':
					return -1;
					break;
				case 'd':
					if (board->sentido == '←') {
						if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
							board->sentido = '←';

							board->lastWaterXY[1]--;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
							board->sentido = '↓';

							board->lastWaterXY[1]--;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
							return -1;
						}
						else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
							board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
							return 1;
						}
					}
					if (board->sentido == '↓') {
						if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
							board->sentido = '↓';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;

						}
						else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1] ] == 'l') {
							board->sentido = '←';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
						else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
							board->sentido = '→';

							board->lastWaterXY[0]++;
							board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

							board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
							return 0;
						}
					}
					break;
				case 'l':
					return -1;
					break;
				}
		}
		else if (board->lastWaterXY[0] == board->actualSize - 1 && board->lastWaterXY[1] < board->actualSize - 1 && board->lastWaterXY[1] > 0) {
			switch (board->lastInsert)
			{
			case 'z':
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				else if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = 'a';
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
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				
				}
				else if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1] ] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				
				break;
			}
		}
		else if (board->lastWaterXY[1] == 0 && board->lastWaterXY[0] < board->actualSize - 1 && board->lastWaterXY[0] > 0) {
			switch (board->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				if(board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						board->sentido = '→';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				break;
			case 's':
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0] ][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						board->sentido = '↓';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						board->sentido = '→';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] = 'a';
						return 1;
					}

				}
				break;
			case 'r':
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						board->sentido = '↓';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				else if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] = 'a';
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
		else if (board->lastWaterXY[1] == board->actualSize - 1 && board->lastWaterXY[0] < board->actualSize - 1 && board->lastWaterXY[0] > 0) {
			switch (board->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						board->sentido = '←';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'a';
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
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						board->sentido = '↓';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
				
				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						board->sentido = '←';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						return -1;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'a';
						return 1;

					}
				
				}
				break;
			case 'l':
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						board->sentido = '↓';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}

				}
				else if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						return -1;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'a';
						return 1;

					}

				}
				break;
			}
		}
		else if (board->lastWaterXY[0] == 0 && board->lastWaterXY[1] == 0) {
			switch (board->lastInsert)
			{
			case 'z':
				return -1;
				break;
			case 'x':
				return -1;
				break;
			case 's':
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] ][board->lastWaterXY[1] + 1] == 'd') {
						board->sentido = '↓';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						board->sentido = '→';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] = 'a';
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
		else if (board->lastWaterXY[0] == 0 && board->lastWaterXY[1] == board->actualSize - 1) {
			switch (board->lastInsert)
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
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						board->sentido = '↓';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}

				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						board->sentido = '←';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] = 'a';
						return 1;
					}
				}
				break;
			case 'l':
				return -1;
				break;
			}
		}
		else if (board->lastWaterXY[0] == board->actualSize - 1 && board->lastWaterXY[1] == 0) {
			switch (board->lastInsert)
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
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				else if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] = 'a';
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
		else if (board->lastWaterXY[0] == board->actualSize - 1 && board->lastWaterXY[1] == board->actualSize - 1) {
			switch (board->lastInsert)
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
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						return -1;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] = 'a';
						return 1;
					}

				}
				else if(board->sentido == '←') {
				}
				break;
			}
		}
		else {
			switch (board->lastInsert)
			{
			case 'z':
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						board->sentido = '↓';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				else if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						board->sentido = '↓';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				break;
			case 'x':
				if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						board->sentido = '→';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						board->sentido = '←';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'a';
						return 1;
					}
				}
				break;
			case 's':
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						board->sentido = '↓';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] = 'a';
						return 1;
					}

				}
				else if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						board->sentido = '→';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						board->sentido = '←';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] = 'a';
						return 1;
					}
				}
				break;
			case 'r':
				if (board->sentido == '→') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'z') {
						board->sentido = '→';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'd') {
						board->sentido = '↓';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] + 1] == 'l') {
						board->sentido = '↑';

						board->lastWaterXY[1]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
				}
				else if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] = 'a';
						return 1;
					}
				break;
			case 'd':
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						board->sentido = '↓';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				if (board->sentido == '↓') {
					if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↓';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'l') {
						board->sentido = '←';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'r') {
						board->sentido = '→';

						board->lastWaterXY[0]++;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] + 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] = 'a';
						return 1;
					}
				}
				break;
			case 'l':
				if (board->sentido == '←') {
					if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'z') {
						board->sentido = '←';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 's') {
						board->sentido = '↓';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'r') {
						board->sentido = '↑';

						board->lastWaterXY[1]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;
					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0]][board->lastWaterXY[1] - 1] == 'a';
						return 1;

					}

				}
				else if (board->sentido == '↑') {
					if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'x') {
						board->sentido = '↑';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'd') {
						board->sentido = '←';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 's') {
						board->sentido = '→';

						board->lastWaterXY[0]--;
						board->lastInsert = board->board[board->lastWaterXY[0]][board->lastWaterXY[1]];

						board->board[board->lastWaterXY[0]][board->lastWaterXY[1]] = 'a';
						return 0;

					}
					else if (board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'e') {
						board->board[board->lastWaterXY[0] - 1][board->lastWaterXY[1]] == 'a';
						return 1;

					}

				}
				break;
			}

		} 

		
}

DWORD placePeca(Board* board, char peca, int x, int y, int player) {
	//falta a parte de escolher em qual board é para meter a peça 
	if (board->board[x][y] != 'W' && board->board[x][y] != '*') {
		board->board[x][y] = peca;
	}
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

DWORD putWall(MemDados* aux, DWORD posX, DWORD posY) {
	WaitForSingleObject(aux->mutexBoard, INFINITE);
	if (posX > aux->VBoard->actualSize || posY > aux->VBoard->actualSize || posX < 0 || posY < 0 || aux->VBoard->board[posX][posY] != '.') {
		ReleaseMutex(aux->mutexBoard);
		_ftprintf(stderr, TEXT("-----------> Cant place wall here: [%d][%d]\n"), posX, posX);
		return -1;
	}else {
		aux->VBoard->board[posX][posY] = 'W';
		_ftprintf(stderr, TEXT("-----------> Placed wall at [%d][%d]\n"), posX, posX);
	}

	ReleaseMutex(aux->mutexBoard);
	return 1;
}
