#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "../Memory.h"
#include "../Registry.h"
#include"utils.h"
#include "synchapi.h"


#define SIZE 200
#define MAX_BOARDSIZE 20
#define MAX_TIMERWATER 30

typedef struct {
	int continua;
	REGISTO_DADOS registoDados; //access to data from the registry
	MemDados memDados;			//access to data for the sharedMemory
}THREADTEC;


typedef struct {
	int continua;
	MemDados* memDados;			//access to data for the sharedMemory

	//Sinc* sinc;
}THREADCONS, * PTHREADCONS;

DWORD WINAPI Threadkeyboard(LPVOID param) {
	THREADTEC* data = (THREADTEC*)param;
	TCHAR comand[SIZE];
	DWORD aux;

	while (data->continua)
	{
		_ftprintf(stdout, TEXT("Comand: "));
		_tscanf_s(TEXT("%s"), &comand, SIZE - 1);

		if (wcscmp(comand, TEXT("start")) == 0) {				
			createBoard(data->registoDados);

		}else if (wcscmp(comand, TEXT("acaba")) == 0) {
			data->continua = 0;

		}
	}
}

DWORD WINAPI ThreadComandsMonitor(LPVOID param) { //thread vai servir para ler do buffer circular os comandos do monitor
	THREADTEC* data = (THREADTEC*)param;
	TCHAR comand[SIZE];
	DWORD aux;

	while (data->continua)
	{
		//TODO
	}
}

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hthread[3];
	int contThread = 0;
	MemDados memDados;
	THREADTEC estruturaThread;
	THREADCONS threadcons;
	estruturaThread.continua = 1;


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	
	if (argc != 3) //user doesn't define inital values, so we go to registry to obtain them
	{
		verificaChave(&estruturaThread.registoDados);
	}
	else if ((estruturaThread.registoDados.actualSize = _ttoi(argv[1])) <= 0 && (estruturaThread.registoDados.actualTime = _ttoi(argv[2])) <= 0) { //user defines initial values 
		if (estruturaThread.registoDados.actualSize > MAX_BOARDSIZE || estruturaThread.registoDados.actualTime > MAX_TIMERWATER) {
			_ftprintf(stdout, TEXT("Size of the Board or Time invalid <MAX Board 20> <MAX time 30>!\n"));
			return -1;
		}
	}

	//Cheking if this is the first instance of Servidor
	if (abreFileMap(&estruturaThread.memDados)) {
		_ftprintf(stderr, TEXT("A Servidor is already open. Closing...\n"));
	
		return -1;
	}

	threadcons.memDados = &estruturaThread.memDados;

	if (!criaSinc(&memDados))
		return -1;

	threadcons.memDados->semMonitor = memDados.semMonitor;


	if (!criaFileMap(&estruturaThread.memDados)) // Criar FileMaps
		return -1;

	if (!criaMapViewOfFiles(&estruturaThread.memDados)) // Criar Vistas
		return -1;

	//TODO: verificar se já existe outra instancia (atravez da memoria partilhada) e criar mecs sincronização	
	if ((hthread[contThread++] = CreateThread(NULL, 0, Threadkeyboard, &threadcons, 0, NULL)) == NULL) // Thread responsible for the keyboard
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Monitor input\n"));
		return -1;
	}


	// Thread responsible for handling monitor input
	/*
	if ((hthread[contThread++] = CreateThread(NULL, 0, Threadkeyboard, &estruturaThread, 0, NULL)) == NULL) 
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the keyboard\n"));
		return -1;
	}
	*/

	WaitForMultipleObjects(contThread, hthread, TRUE, INFINITE);
	fechaViewFile(&estruturaThread.memDados);
	fechaHandleMem(&estruturaThread.memDados);
}


