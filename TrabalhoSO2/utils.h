#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include "../Memory.h"
#include <time.h>


LARGE_INTEGER intToLargeInt(int i);
void setupBoard(MemDados* aux, DWORD actualSize, DWORD np);
DWORD insertWater(PLAYER* pData);
DWORD placePeca(Board* b, MemDados* aux, char peca, int posX, int posY, int player);
void printBoard(Board* aux);
DWORD putWall(Board* b, MemDados* aux, DWORD posX, DWORD posY);