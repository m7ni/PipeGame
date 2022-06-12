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
void setupBoard(MemDados* aux, DWORD actualSize,DWORD p);
void printBoard(MemDados* dados, DWORD p);
DWORD putWall(Board* aux, DWORD posX, DWORD posY, DWORD p);