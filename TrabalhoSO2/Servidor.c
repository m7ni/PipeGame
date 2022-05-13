#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "../Memory.h"
#include "../Registry.h"
#include"utils.h"

#define SIZE 200
#define MAX_BOARDSIZE 20
#define MAX_TIMERWATER 30

DWORD WINAPI Threadkeyboard(LPVOID param);
DWORD WINAPI ThreadWaterRunning(LPVOID param);
DWORD WINAPI ThreadComandsMonitor(LPVOID param);

typedef struct {
	DWORD* continua;
	MemDados* memDados;			//access to data for the sharedMemory
	Sinc* sinc;
}THREADWATER;

typedef struct {
	DWORD* continua;
	REGISTO_DADOS registoDados; //access to data from the registry
	MemDados memDados;			//access to data for the sharedMemory
	Sinc* sinc;
}THREADTEC,* PTHREADTEC;

typedef struct {
	DWORD* continua;
	Sinc* sinc;
	MemDados* memDados;			//access to data for the sharedMemory
}THREADCONS, * PTHREADCONS;



int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hthread[3];
	DWORD contThread = 0;
	DWORD continua = 1;

	THREADTEC KB;
	THREADCONS CONSUMER;
	THREADWATER TWater;

	MemDados sem;
	Board board;
	Sinc sinc;

	KB.continua = &continua;
	CONSUMER.continua = &continua;
	TWater.continua = &continua;
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	if (argc != 3) //user doesn't define inital values, so we go to registry to obtain them
	{
		verificaChave(&KB.registoDados);
	}
	else if ((KB.registoDados.actualSize = _ttoi(argv[1])) <= 0 && (KB.registoDados.actualTime = _ttoi(argv[2])) <= 0) { //user defines initial values 
		if (KB.registoDados.actualSize > MAX_BOARDSIZE || KB.registoDados.actualTime > MAX_TIMERWATER) {
			_ftprintf(stdout, TEXT("Size of the Board or Time invalid <MAX Board 20> <MAX time 30>!\n"));
			return -1;
		}
	}

	//Cheking if this is the first instance of Servidor
	if (abreFileMap(&KB.memDados)) {
		_ftprintf(stderr, TEXT("A Servidor is already open. Closing...\n"));
		return -1;
	}

	if (!criaSincBuffer(&sem))
		return -1;

	if (!criaFileMap(&KB.memDados)) // Criar FileMaps
		return -1;

	if (!criaSincGeral(&sinc, 1)) // Criar Vistas
		return -1;

	if (!criaMapViewOfFiles(&KB.memDados)) // Criar Vistas
		return -1;


	TWater.sinc = &sinc;

	KB.sinc = &sinc;
	CONSUMER.sinc = &sinc;

	KB.memDados.semMonitor = sem.semMonitor;
	KB.memDados.semServer = sem.semServer;
	KB.memDados.mutexSEM = sem.mutexSEM;
	KB.memDados.flagMonitorComand = 0;
	KB.memDados.timeMonitorComand = 0;
	KB.memDados.VBufCircular->in = 0;
	KB.memDados.VBufCircular->out = 0;
	KB.memDados.VBoard->actualSize = KB.registoDados.actualSize;

	setupBoard(&KB.memDados, KB.registoDados.actualSize);

	CONSUMER.memDados = &KB.memDados;
	TWater.memDados = &KB.memDados;
	// Thread responsible for the keyboard
	
	if ((hthread[contThread++] = CreateThread(NULL, 0, Threadkeyboard, &KB, 0, NULL)) == NULL) {
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the keyboard\n"));
		return -1;
	}

	// Thread responsible for handling monitor input
	if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadComandsMonitor, &CONSUMER, 0, NULL)) == NULL)
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Monitor input\n"));
		return -1;
	}
	// Thread responsible for the Water
	
	if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadWaterRunning, &KB, 0, NULL)) == NULL)
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Monitor input\n"));
		return -1;
	}


	WaitForMultipleObjects(contThread, hthread, TRUE, INFINITE);
	CloseViewFile(&KB.memDados);
	CloseHandleMem(&KB.memDados);
	CloseSinc(&sinc);
	//CloseSem(&KB.memDados);
}

DWORD WINAPI Threadkeyboard(LPVOID param) {
	THREADTEC* data = (THREADTEC*)param;
	TCHAR comand[SIZE];
	DWORD aux;

	_ftprintf(stderr, TEXT("ThreadKeyboard Started\n"));
	while (*data->continua)
	{
		_ftprintf(stdout, TEXT("Comand: "));
		_tscanf_s(TEXT("%s"), &comand, SIZE - 1);

		if (*data->continua == 0) {
			_ftprintf(stderr, TEXT("ThreadKeyboard Ended\n"));
			return 1;
		}

		if (wcscmp(comand, TEXT("start")) == 0) {
			SetEvent(data->sinc->timerStartEvent);
			_ftprintf(stderr, TEXT("-----------> Started\n"));
		}
		else if (wcscmp(comand, TEXT("end")) == 0) {
			*data->continua = 0;
		}
		else if (wcscmp(comand, TEXT("pause")) == 0) {
			ResetEvent(data->sinc->pauseResumeEvent);
			_ftprintf(stderr, TEXT("-----------> Paused\n"));
		}
		else if (wcscmp(comand, TEXT("resume")) == 0) {
			SetEvent(data->sinc->pauseResumeEvent);
			_ftprintf(stderr, TEXT("-----------> Resumed\n"));
		}
	}

}

DWORD WINAPI ThreadWaterRunning(LPVOID param) { //thread responsible for startign the water running
	PTHREADTEC data = (PTHREADTEC)param;
	
	WaitForSingleObject(data->sinc->timerStartEvent, INFINITE); //Comand Start
	Sleep(0); //data->registoDados.actualTime*1000 <- Meter isto quando se entregar
	data->memDados.flagMonitorComand = 0;
	Board aux;
	SetEvent(data->sinc->printBoard);
	while (&data->continua) {
		WaitForSingleObject(data->sinc->pauseResumeEvent, INFINITE); //Pause Resume Comand

		Sleep(2000);

		if (data->memDados.flagMonitorComand) { //Monitor Comand
			_ftprintf(stderr, TEXT("-----------> Water Stoped for %d seconds\n"), data->memDados.timeMonitorComand);
			Sleep(data->memDados.timeMonitorComand * 1000);
			data->memDados.flagMonitorComand = 0;
		
		}

		
		WaitForSingleObject(data->memDados.mutexBoard, INFINITE);
		CopyMemory(&aux, data->memDados.VBoard, sizeof(Board));
		ReleaseMutex(data->memDados.mutexBoard);

		DWORD res= insertWater(&aux);
		
		WaitForSingleObject(data->memDados.mutexBoard, INFINITE);
		CopyMemory(data->memDados.VBoard, &aux, sizeof(Board));
		ReleaseMutex(data->memDados.mutexBoard);


		SetEvent(data->sinc->printBoard); // usar quando queremos avisar o monitor que pode imprimir
		
		if (res == 1) {
			WaitForSingleObject(data->memDados.mutexBoard, INFINITE);
			data->memDados.VBoard->win = 1;
			ReleaseMutex(data->memDados.mutexBoard);
			_ftprintf(stderr, TEXT("Ganhou\n"));
			*data->continua = 0;
			ReleaseSemaphore(data->memDados.semServer, 1, NULL);
			return 1;
		}
		else if (res == -1) {
			_ftprintf(stderr, TEXT("Perdeu\n"));
			WaitForSingleObject(data->memDados.mutexBoard, INFINITE);
			data->memDados.VBoard->win = -1;
			ReleaseMutex(data->memDados.mutexBoard);
			*data->continua = 0;
			ReleaseSemaphore(data->memDados.semServer, 1, NULL);
			return 1;
		}
		
	}

}

DWORD WINAPI ThreadComandsMonitor(LPVOID param) { //thread vai servir para ler do buffer circular os comandos do monitor
	THREADCONS* data = (THREADCONS*)param;
	TCHAR comand[SIZE];
	Comand aux;
	LARGE_INTEGER liDueTime;
	_ftprintf(stderr, TEXT("ThreadComandsMonitor Started\n"));
	
	while (*data->continua)
	{
		WaitForSingleObject(data->memDados->semServer, INFINITE);
		if (*data->continua == 0) {
			_ftprintf(stderr, TEXT("ThreadComandsMonitor Ended\n"));
			return 1;
		}
		WaitForSingleObject(data->memDados->mutexSEM, INFINITE);

		CopyMemory(&aux, &data->memDados->VBufCircular->UserComands[data->memDados->VBufCircular->out], sizeof(Comand));
		data->memDados->VBufCircular->out = (data->memDados->VBufCircular->out + 1) % TAM;

		ReleaseMutex(data->memDados->mutexSEM);
		ReleaseSemaphore(data->memDados->semMonitor, 1, NULL);
		switch (aux.code) {

		case 1:
			data->memDados->timeMonitorComand = aux.time;
			data->memDados->flagMonitorComand = 1;
			break;

		case 2:
			
			if (!putWall(data->memDados, aux.wallX, aux.wallY)) {
				_ftprintf(stderr, TEXT("Error placing wall\n"));
			}else
				SetEvent(data->sinc->printBoard);

			break;
		}

	}
	_ftprintf(stderr, TEXT("ThreadComandsMonitor Ended\n"));
}
