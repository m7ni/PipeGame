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
#define TAM 3
#define TAM_COMAND 100

#define FICH_MEM_P_A TEXT("memoriaPartilhadaA")
#define FICH_MEM_P_B TEXT("memoriaPartilhadaB")

#define SEMAFORO_BUFFER_M TEXT("SEM_BUFFER_MONITOR")
#define SEMAFORO_BUFFER_S TEXT("SEM_BUFFER_SERVIDOR")
#define BUFFER_MUTEX TEXT("BUFFER_MUTEX")

#define BOARD_MUTEX TEXT("BOARD_MUTEX")

#define TIMER_START_EVENT TEXT("TIMER_START_EVENT")
#define PAUSE_RESUME_EVENT TEXT("PAUSE_RESUME_EVENT")

#define MUTEX TEXT("MUTEX")

typedef struct { //Ainda não sei bem para que é que vai ser preciso (Neste momento é para passar os comandos atravez de int)
	unsigned int code; //Passar comandos MONITOR -> SERVIDOR
	unsigned int time; 
} Comand;



typedef struct { //Ainda não sei bem para que é que vai ser preciso (Neste momento é para passar os comandos atravez de int)
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

	LPHANDLE FileMapBoard;			// File Map para a struct Board
	Board* VBoard;			// Vista para a struct Board
} MemDados;


typedef struct {
	HANDLE timerStartEvent; //water starts running 
	HANDLE pauseResumeEvent; //Server comand
} Sinc;

BOOL abreFileMap(MemDados* dados);
BOOL criaFileMap(MemDados* dados);
BOOL fechaHandleMem(MemDados* dados);
BOOL fechaViewFile(MemDados* dados);
BOOL criaSincBuffer( MemDados* sem);
BOOL criaMapViewOfFiles(MemDados* dados);

#endif /*MEMORY_H*/


/*
typedef struct {
	// Buffer Circular
	LPHANDLE FileMapAviao;				// File Map para a estrutura do buffer circular
	BufferCircular* BufCircular;		// Vista para a estrutura do buffer circular

	// Semaforos
	HANDLE semAviao;					// Semáforo para indicar que o avião pode escrever
	HANDLE semControl;					// Semáforo para indicar que o controlador tem de ler
	HANDLE mutex;						// Mutex para garantir acesso único ao buffer circular
	HANDLE mutexMensagens;				// Mutex para garantir que uma mensagem é lida pelo respetivo avião antes de ser escrito algo por cima
	// Memória aeroportos
	LPHANDLE FileMapAeroporto;			// File Map para o array de aeroportos
	Aeroporto* BufAeroportos;			// Vista para o array de aeroportos

	//Mensagens
	LPHANDLE FileMapMensagens;			// File Map para a estrutura de comunicação entre o controlador e os aviões
	Mensagens* BufMens;					// Vista para a estrutura de comunicação entre o controlador e os aviões
} MemDados;

typedef struct {
	HANDLE mutex;						// Mutex para garantir que não é escrito nenhum avião pelo controlador enquanto está a ser lido pelo aviao
	HANDLE eventoAceitaAviao[2];		// Evento para aceitar ou recusar novos aviões e semáforo com o número máximo de aviões que podem ser atendidos pelo controlador
} Sinc;


*/

