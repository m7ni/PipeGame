#include "Monitor.h"

#define SIZE 200


typedef struct {
	DWORD *continua;
	MemDados* memDados;			//access to data for the sharedMemory
	Sinc* sinc;
}THREADTEC;

typedef struct {			 	
	DWORD *continua;
	Sinc* sinc;
	MemDados* memDados;			
}THREADPRINT;

DWORD WINAPI Threadkeyboard(LPVOID param) {
	THREADTEC* data = (THREADTEC*)param;
	TCHAR comand[SIZE];
	Comand aux;
	//WaitForSingleObject(data->sinc->timerStartEvent, INFINITE);
	Sleep(1000);
	aux.code = 0;
	aux.random = 0;
	
	while (*data->continua)
	{
	
		_ftprintf(stdout, TEXT("Comand: "));
		_tscanf_s(TEXT("%s"), &comand, SIZE - 1);

		if (*data->continua == 0) {
			return 1;
		}

		if (wcscmp(comand, TEXT("stop-water")) == 0) {
			
			_ftprintf(stdout, TEXT("-----------> seconds: "));
			_tscanf_s(TEXT("%d"), &aux.time, sizeof(unsigned int));
			aux.code = 1;
		}
		else if (wcscmp(comand, TEXT("insert-block")) == 0) {
			aux.code = 2;
			_ftprintf(stdout, TEXT("-----------> x: "));
			_tscanf_s(TEXT("%d"), &aux.wallX);
			_ftprintf(stdout, TEXT("-----------> y: "));
			_tscanf_s(TEXT("%d"), &aux.wallY);
		}
		else if (wcscmp(comand, TEXT("random")) == 0) {
			aux.code = 3;
			if (aux.random == 1)
				aux.random = 0;
			else
				aux.random = 1;
		}
		if (aux.code != 0) {
			WaitForSingleObject(data->memDados->semMonitor, INFINITE);
			WaitForSingleObject(data->memDados->mutexSEM, INFINITE);

			CopyMemory(&data->memDados->VBufCircular->UserComands[data->memDados->VBufCircular->in], &aux, sizeof(Comand));
			data->memDados->VBufCircular->in = (data->memDados->VBufCircular->in + 1) % TAM;

			ReleaseMutex(data->memDados->mutexSEM);
			ReleaseSemaphore(data->memDados->semServer, 1, NULL);
		}
		aux.code = 0;
		//else if (wcscmp(comand, TEXT("random")) == 0) { //CLiente only is needed for META 2, so we don't have to implemenmt this yet
		//	//TODO
		//}
	}
}

DWORD WINAPI ThreadPrintBoard(LPVOID param) {
	THREADPRINT* data = (THREADPRINT*)param;
	Board aux;
	
	while (*data->continua)
	{
		WaitForSingleObject(data->sinc->printBoard, INFINITE); //Event
		if (*data->continua == 0) {
			return 1;
		}
		WaitForSingleObject(data->memDados->mutexBoard, INFINITE);
		CopyMemory(&aux, data->memDados->VBoard, sizeof(Board));
		ReleaseMutex(data->memDados->mutexBoard);

		printBoard(&aux, aux.numP); //enviamos o numero de jogadores e ele imprime, um ou dois tabuleiros
		Sleep(2000);

		
		for (DWORD i = 0; i < aux.numP; i++) {
			if (aux.player[i].win == 1) {
				_ftprintf(stderr, TEXT("\n\n Player[%d] Won\n"),i);
				*data->continua = 0;
			}
			else if (aux.player[i].win == -1) {
				_ftprintf(stderr, TEXT("\n\n Player[%d] Lost\n"), i);
				*data->continua = 0;
			}
		}
		
		
		

	}
	SetEvent(data->sinc->endMonitor);
}

DWORD WINAPI ThreadEnd(LPVOID param) {
	THREADPRINT* data = (THREADPRINT*)param;
	while (*data->continua)
	{
		WaitForSingleObject(data->sinc->endMonitor,INFINITE); //Event
		SetEvent(data->sinc->timerStartEvent);
		SetEvent(data->sinc->printBoard);
			*data->continua = 0;
	
	}
}

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hthread[3];
	DWORD contThread = 0;
	MemDados memDados;
	THREADTEC threadtec;
	THREADPRINT threadprint;
	Sinc sinc;
	DWORD continua = 1;

#ifdef UNICODE
	(void)_setmode(_fileno(stdin), _O_WTEXT);
	(void)_setmode(_fileno(stdout), _O_WTEXT);
	(void)_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if (!abreFileMap(&memDados))
	{
		_ftprintf(stderr, TEXT("There is no Servidor open! Try later...\n"));
		return -1;
	}

	if (!criaSincBuffer(&memDados))
		return -1;

	if (!criaSincGeral(&sinc, 0)) 
		return -1;
	
	if (!criaMapViewOfFiles(&memDados)) //creating map views
		return -1;

	threadtec.sinc = &sinc;
	threadprint.sinc = &sinc;
	threadtec.continua = &continua;
	threadtec.memDados = &memDados;
	threadprint.memDados = &memDados;
	threadprint.continua = &continua;
	_ftprintf(stderr, TEXT("\n---Monitor Opened---\n\n"));
	if (argc != 1)
	{
		_ftprintf(stdout, TEXT("This Process doesn't require any arguments\n\n"));
		return -1;
	}

	if ((hthread[contThread++] = CreateThread(NULL, 0, Threadkeyboard, &threadtec, 0, NULL)) == NULL) // Thread responsible for the keyboard
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Monitor input\n"));
		return -1;
	}

	if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadPrintBoard, &threadprint, 0, NULL)) == NULL) // Thread responsible for printing the board
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for printing the Board\n"));
		return -1;
	}
	if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadEnd, &threadprint, 0, NULL)) == NULL) // Thread responsible for printing the board
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for printing the Board\n"));
		return -1;
	}
	WaitForMultipleObjects(contThread, hthread, TRUE, INFINITE);
	CloseViewFile(&memDados);
	CloseHandleMem(&memDados);
	CloseSinc(&sinc,0);
	CloseSem(&memDados);
}
