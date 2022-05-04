#include "Monitor.h"

#define SIZE 200


typedef struct {
	DWORD continua;
	MemDados memDados;			//access to data for the sharedMemory
}THREADTEC;

typedef struct { //thread thats responsible for showing the board
	DWORD continua;
}THREADPRINT;

DWORD WINAPI Threadkeyboard(LPVOID param) {
	THREADTEC* data = (THREADTEC*)param;
	TCHAR comand[SIZE];
	DWORD aux;

	while (data->continua)
	{
		_ftprintf(stdout, TEXT("Comand: "));
		_tscanf_s(TEXT("%s"), &comand, SIZE - 1);

		if (wcscmp(comand, TEXT("stop-water")) == 0) {
			//TODO
		}
		else if (wcscmp(comand, TEXT("insert-block")) == 0) {
			//TODO

		}
		else if (wcscmp(comand, TEXT("random")) == 0) {
			//TODO
		}
	}
}

DWORD WINAPI ThreadPrintBoard(LPVOID param) {
	THREADPRINT* data = (THREADPRINT*)param;
	while (data->continua)
	{
		//TODO:
	}
}
int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hthread[2];
	DWORD contThread = 0;
	MemDados memDados;
	THREADTEC threadtec;
	THREADPRINT threadprint;

	if (!abreFileMap(&memDados))
	{
		_ftprintf(stderr, TEXT("There is no Servidor open! Try later...\n"));
		return -1;
	}

	if (!criaMapViewOfFiles(&memDados)) //creating map views
		return -1;


	if (argc != 1)
	{
		_ftprintf(stdout, TEXT("This Process doesn't require any arguments\n\n"));
		return -1;
	}


}
