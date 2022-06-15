#ifndef MEMORY_H
#define MEMORY_H

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "Registry.h"
#include "Servidor.h"
#include "WinBase.h"

#define BUFFERSIZE 255
#define MAX 20
#define TAM 15
#define TAM_COMAND 100

#define FICH_MEM_P_A TEXT("memoriaPartilhadaA")
#define FICH_MEM_P_B TEXT("memoriaPartilhadaB")

#define SEMAFORO_BUFFER_M TEXT("SEM_BUFFER_MONITOR")
#define SEMAFORO_BUFFER_S TEXT("SEM_BUFFER_SERVIDOR")
#define BUFFER_MUTEX TEXT("BUFFER_MUTEX")

#define BOARD_MUTEX TEXT("BOARD_MUTEX")

#define TIMER_START_EVENT TEXT("TIMER_START_EVENT")
#define PAUSE_RESUME_EVENT TEXT("PAUSE_RESUME_EVENT")
#define PAUSE_MONITOR_COMAND TEXT("PAUSE_MONITOR_COMAND")

#define BOARD_MUTEX TEXT("BOARD_MUTEX")
#define EVENT_BOARD TEXT("EVENT_BOARD")
#define EVENT_END TEXT("EVENT_END")

#define EVENT_READ_ONE TEXT("EVENT_READ_ONE")
#define EVENT_READ_TWO TEXT("EVENT_READ_ONE")

typedef struct {
	unsigned int code; //Passar comandos MONITOR -> SERVIDOR
	unsigned int time; 
	unsigned int wallX;
	unsigned int wallY;
} Comand;

typedef struct {   //comunicar pelo pipe
	unsigned int nPlayer;
	unsigned int solo;
	PLAYER player;
	
	
	HANDLE read;
	HANDLE *hPipe;
	HDC *memDC;
	HDC bmpDC;
	HANDLE mutexCliente;
	HWND hWnd;
	BITMAP bmp;
} Pipe;

typedef struct { 
	Comand UserComands[TAM]; //Passar comandos MONITOR -> SERVIDOR
	unsigned int in;					// Posição de escrita no buffer circular
	unsigned int out;				// Posição de leitura no buffer circular
} BufferCircular;

typedef struct {
	LPHANDLE FileBufCircular;				// File Map para a estrutura do buffer circular
	BufferCircular* VBufCircular;		// Vista para a estrutura do buffer circular 
	HANDLE semMonitor;					// Semáforo para indicar que o servidor tem de ler
	HANDLE semServer;					// Semáforo para indicar que o monitor pode escrever
	HANDLE mutexSEM;						// Mutex para garantir acesso único ao buffer circular

	HANDLE mutexBoard; //Mutex to control the access to the board shared memory
	LPHANDLE FileMapBoard;			// File Map para a struct Board
	Board* VBoard;			// Vista para a struct Board

	unsigned int flagMonitorComand;
	unsigned int timeMonitorComand;

} MemDados;

typedef struct {
	HANDLE timerStartEvent; //water starts running 
	HANDLE pauseResumeEvent; //Server comand
	HANDLE endMonitor; 
	HANDLE printBoard; //Event that informs the monitor to print the board
} Sinc;

BOOL abreFileMap(MemDados* dados);
BOOL criaFileMap(MemDados* dados);
BOOL CloseHandleMem(MemDados* dados);
BOOL CloseViewFile(MemDados* dados);
BOOL criaSincBuffer( MemDados* sem);
BOOL criaMapViewOfFiles(MemDados* dados);
BOOL criaSincGeral(Sinc* sinc, DWORD origin);
BOOL criaSincClient(Pipe* dados);
BOOL CloseSinc(Sinc* sinc);
BOOL CloseSem(MemDados* dados);
#endif /*MEMORY_H*/

