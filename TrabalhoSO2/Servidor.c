#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "../Memory.h"
#include "../Registry.h"
#include"utils.h"

#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")
#define SIZE 256
#define MAX_BOARDSIZE 20
#define MAX_TIMERWATER 30
#define MAX_PLAYERS 2

DWORD WINAPI Threadkeyboard(LPVOID param);
DWORD WINAPI ThreadInsertPipe(LPVOID param);
DWORD WINAPI ThreadComandsMonitor(LPVOID param);
DWORD WINAPI ThreadNamedPipes(LPVOID param);
DWORD WINAPI ThreadConectClient(LPVOID param);
DWORD WINAPI ThreadWaterRunning(LPVOID param);
typedef struct {				//struct that hold the data to the overlapped pipes
	HANDLE hInstance;
	OVERLAPPED overlap;
	BOOL active;
}PIPEDATA;

typedef struct {
	DWORD* continua;
	MemDados* memDados;			//access to data for the sharedMemory
	Sinc* sinc;

	DWORD id;
	HANDLE mutexP;
	HANDLE eventPipe;
	PIPEDATA hPipe;          //Handlers especificos para o pipe desse player
	Pipe* pipeData;			//Estrutura que é enviada atraves do pipe
}THREADGAME, *PTHREADGAME;

typedef struct {
	DWORD numPlayer;
	PIPEDATA hPipe[MAX_PLAYERS];
	HANDLE hEvents[MAX_PLAYERS];
	HANDLE hMutex;
	Pipe* pipeDataInitial;

	MemDados* memDados;			//access to data for the sharedMemory
	Sinc* sinc;

	DWORD timeR; //access to data from the registry
	DWORD* continua;

	THREADGAME* playerServ[2];
}THREADPIPE, * PTHREADPIPE;



typedef struct {
	DWORD numPlayer;

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
	HANDLE threadC;
	DWORD contThread = 0;
	DWORD continua = 1;
	HANDLE hPipe, hThread, hEventTemp;

	THREADTEC KB;			//enviada para a thread quer trata do teclado
	THREADCONS CONSUMER;	//a ser enviada para receber os comandos do monitor
	
	THREADPIPE TP;

	THREADGAME TG1;			//a ser enviada para a Thread que insere peças
	Pipe pipeData1;

	THREADGAME TG2;
	Pipe pipeData2;

	MemDados sem;
	Board board;
	Sinc sinc;

	KB.continua = &continua;
	CONSUMER.continua = &continua;
	TG1.continua = &continua;
	TG2.continua = &continua;
	TP.continua = &continua;

#ifdef UNICODE
	(void)_setmode(_fileno(stdin), _O_WTEXT);
	(void)_setmode(_fileno(stdout), _O_WTEXT);
	(void)_setmode(_fileno(stderr), _O_WTEXT);
#endif

	if (argc != 3) //user doesn't define inital values, so we go to registry to obtain them
	{
		verificaChave(&KB.registoDados);
	}
	else {
		if ((KB.registoDados.actualSize = _ttoi(argv[1])) >= 0 && (KB.registoDados.actualTime = _ttoi(argv[2])) >= 0) { //user defines initial values 
			if (KB.registoDados.actualSize > MAX_BOARDSIZE || KB.registoDados.actualTime > MAX_TIMERWATER) {
				_ftprintf(stdout, TEXT("Size of the Board or Time invalid <MAX Board 20> <MAX time 30>!\n"));
				return -1;
			}
			else {
				atualizaChave(KB.registoDados.actualSize, KB.registoDados.actualTime);
			}

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


	/*if (!criaSincClient(&pipeData)) // Criar Sinc Pipe
		return -1;*/

	_ftprintf(stderr, TEXT("\n---Servidor Opened---\n\nWaiting for Players...\n\n"));

	TG1.sinc = &sinc;
	TG2.sinc = &sinc;

	KB.sinc = &sinc;
	CONSUMER.sinc = &sinc;
	TP.pipeDataInitial = &pipeData1;
	TP.sinc = &sinc;
	KB.memDados.semMonitor = sem.semMonitor;
	KB.memDados.semServer = sem.semServer;
	KB.memDados.mutexSEM = sem.mutexSEM;
	KB.memDados.flagMonitorComand = 0;
	KB.memDados.timeMonitorComand = 0;
	KB.memDados.VBufCircular->in = 0;
	KB.memDados.VBufCircular->out = 0;
	KB.memDados.VBoard->actualSize = KB.registoDados.actualSize;
	TP.numPlayer = 0;
	TP.timeR = KB.registoDados.actualTime;
	
	CONSUMER.memDados = &KB.memDados;
	TG1.memDados = &KB.memDados;
	TG2.memDados = &KB.memDados;
	TP.memDados = &KB.memDados;

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

	//Antes de criar a thread da água temos que saber se o jogo vai ser solo ou comp (vamos criar uma ou duas instancias) 
	TP.hMutex = CreateMutex(NULL, FALSE, NULL);
	if (TP.hMutex == NULL) {
		_tprintf(_T("\n[ERRO] Criar Mutex! (CreateMutex)"));
		exit(-1);
	}

	// Named Pipe creation
	for ( DWORD i = 0; i < MAX_PLAYERS; i++) { //Creating the copys of the pipe, one for each Client
		_tprintf(_T("[Server] Creating copy of pipe '%s'... (CreateNamedPipe)\n"), PIPE_NAME);
		hEventTemp = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hEventTemp == NULL) {
			_tprintf(_T("[ERROR] Creating Event! (CreateEvent)"));
			exit(-1);
		}
		hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, MAX_PLAYERS, 256 * sizeof(TCHAR), 256 * sizeof(TCHAR), 1000, NULL);
		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(_T("[ERROR] Creating Named Pipe! (CreateNamedPipe)"));
			exit(-1);
		}
		ZeroMemory(&TP.hPipe[i].overlap, sizeof(TP.hPipe[i].overlap));
		TP.hPipe[i].hInstance = hPipe;
		TP.hPipe[i].overlap.hEvent = hEventTemp;
		TP.hEvents[i] = hEventTemp;
		TP.hPipe[i].active = FALSE;

		if (ConnectNamedPipe(hPipe, &TP.hPipe[i].overlap)) {
			_tprintf(_T("[ERROR] Server Conection! (ConnectNamedPipe)\n"));
			exit(-1);
		}
		//esperar que os jogadores cheguem
	}
	pipeData1.eventRead = CreateEvent(NULL,
		TRUE,
		FALSE,
		EVENT_READ_ONE);

	pipeData2.eventRead = &pipeData1.eventRead;

	TG1.pipeData = &pipeData1;
	TG2.pipeData = &pipeData2;
	TP.playerServ[0] = &TG1;
	TP.playerServ[1] = &TG2;
	//abrir thread para esperar que os jogadores entrem
	if ((threadC = CreateThread(NULL, 0, ThreadConectClient, &TP, 0, NULL)) == NULL)
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Conection of players\n"));
		return -1;
	}

	WaitForSingleObject(threadC, INFINITE); // necessario saber se é solo se comp
	_ftprintf(stderr, TEXT("já temos a resposta do jogador ->> %d\n"),TP.pipeDataInitial->solo);


	setupBoard(&KB.memDados, KB.registoDados.actualSize,TP.numPlayer); //dependendo de ser solo ou comp, vai criar um ou dois tabuleiros



	
	Board aux;
	WaitForSingleObject(KB.memDados.mutexBoard, INFINITE);
	CopyMemory(&aux, KB.memDados.VBoard, sizeof(Board));
	ReleaseMutex(KB.memDados.mutexBoard);

	

	TG1.id = 0;
	TG1.pipeData->player = aux.player[0];
	TG1.hPipe = TP.hPipe[0];
	TG1.eventPipe = TP.hEvents[0];
	TG1.mutexP = TP.hMutex;
	aux.player[0].actualSize = aux.actualSize;

	if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadInsertPipe, &TG1, 0, NULL)) == NULL)
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Pipes\n"));
		return -1;
	}

	if (TP.pipeDataInitial->nPlayer == 2) {
		TG2.hPipe = TP.hPipe[1];
		TG2.eventPipe = TP.hEvents[1];
		TG2.mutexP = TP.hMutex;
		TG2.pipeData->player = aux.player[1];
		TG1.id = 1;
		aux.player[1].actualSize = aux.actualSize;
		if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadInsertPipe, &TG2, 0, NULL)) == NULL)
		{
			_ftprintf(stderr, TEXT("Error creating Thread responsible for thePipes\n"));
			return -1;
		}
	}
	
	//Thread responsible for the Water
	if ((hthread[contThread++] = CreateThread(NULL, 0, ThreadWaterRunning, &TP, 0, NULL)) == NULL)
	{
		_ftprintf(stderr, TEXT("Error creating Thread responsible for the Water\n"));
		return -1;
	}



	WaitForMultipleObjects(contThread, hthread, TRUE, INFINITE);

	// Dar close dos pipes
	CloseViewFile(&KB.memDados);
	CloseHandleMem(&KB.memDados);
	CloseSinc(&sinc);
	CloseSem(&KB.memDados);
}

DWORD WINAPI ThreadWaterRunning(LPVOID param) { //comum aos dois Players
	PTHREADPIPE data = (PTHREADPIPE)param;
	DWORD fl = 1;
	data->memDados->flagMonitorComand = 0;
	Board aux;
	SetEvent(data->sinc->printBoard);
	DWORD n, res, playerLost = 3, playerWon = 1;
	_tprintf(TEXT("ThreadWaterRunning\n"));

	
	_tprintf(TEXT("ThreadWaterRunning - Enviei pela primeira vez as boards para os clientes \n"));
	_ftprintf(stderr, TEXT("-----------> Water Running in %d seconds\n"), data->timeR);

	while (&data->continua) {


		for (DWORD i = 0; i < data->numPlayer; ++i) {
			WaitForSingleObject(data->hMutex, INFINITE);
			if (data->hPipe->active) {
				if (!WriteFile(data->hPipe[i].hInstance, data->playerServ[i]->pipeData, sizeof(Pipe), &n, &data->hPipe[i].overlap)) {
					_tprintf(TEXT("[ERRO] Escrever no pipe! Water Running 1 (WriteFile)\n"));
					exit(-1);
				}
			}
			ReleaseMutex(data->hMutex);
		}

		if (fl == 1) {
			WaitForSingleObject(data->sinc->timerStartEvent, INFINITE); //Comand Start
			Sleep(data->timeR * 1000);
			fl = 0;
		}
		

		WaitForSingleObject(data->sinc->pauseResumeEvent, INFINITE); //Pause Resume Comand

		Sleep(3000);

		if (data->memDados->flagMonitorComand) { //Monitor Comand
			_ftprintf(stderr, TEXT("-----------> Water Stoped for %d seconds\n"), data->memDados->timeMonitorComand);
			Sleep(data->memDados->timeMonitorComand * 1000);
			data->memDados->flagMonitorComand = 0;
		}
		
		WaitForSingleObject(data->memDados->mutexBoard, INFINITE);
		CopyMemory(&aux, data->memDados->VBoard, sizeof(Board));
		ReleaseMutex(data->memDados->mutexBoard);


		for (DWORD i = 0; i < data->numPlayer; ++i) {//vamos inserir água nos jogadores que existem
			WaitForSingleObject(data->hMutex, INFINITE);
			if (data->hPipe->active) { 
				res = waterMoving(&aux.player[i].board);
				if (res == -1) {
					playerLost = i;
				}
				else if(res == 1) {
			
				}
			}
			ReleaseMutex(data->hMutex);
		}

		_tprintf(TEXT("passei pelo update\n"));

				
		for (DWORD i = 0; i < data->numPlayer; ++i) {
			if (data->hPipe->active) {
				data->playerServ[i]->pipeData->player = aux.player[i];
			}
		}
				
		_tprintf(TEXT("vou meter na memoria\n"));

		WaitForSingleObject(data->memDados->mutexBoard, INFINITE);
		CopyMemory(data->memDados->VBoard, &aux, sizeof(Board));
		ReleaseMutex(data->memDados->mutexBoard);

		SetEvent(data->sinc->printBoard);
		ResetEvent(data->sinc->printBoard);

		for (DWORD i = 0; i < data->numPlayer; i++) {
			if (data->hPipe->active) {
				SetEvent(data->playerServ[i]->pipeData->eventRead);
			}
		}
		
		
		if (playerLost == 1 || playerLost ==0) {
			WaitForSingleObject(data->memDados->mutexBoard, INFINITE);
			data->memDados->VBoard->player[playerLost].lose = 1;
			ReleaseMutex(data->memDados->mutexBoard);
			_ftprintf(stderr, TEXT("\n\nYou Lost\n"));
			*data->continua = 0;
			ReleaseSemaphore(data->memDados->semServer, 1, NULL);
			return 1;
		}

		if (playerWon == 1 || playerWon == 0) {
			_ftprintf(stderr, TEXT("\n\nYou Won\n"));
			WaitForSingleObject(data->memDados->mutexBoard, INFINITE);
			data->memDados->VBoard->player[playerWon].win = 1;
			ReleaseMutex(data->memDados->mutexBoard);
			*data->continua = 0;
			ReleaseSemaphore(data->memDados->semServer, 1, NULL);
			return 1;
		}
		
	}
	for (DWORD i = 0; i < data->numPlayer; i++)
		SetEvent(data->hEvents[i]);
	return 0;
}

DWORD WINAPI ThreadInsertPipe(LPVOID param) { //uma thread destas para cada Player
	PTHREADGAME data = (PTHREADGAME)param;
	DWORD n,ret;
	Board aux;
	_tprintf(TEXT("inicio thread InsertPipe Player[%d]\n"),data->id);
	while (&data->continua) {
		ret = ReadFile(data->hPipe.hInstance, data->pipeData, sizeof(Pipe), &n, NULL);
		if (ret != 0) {

			placePeca(data->memDados, data->pipeData->player.peca.desiredPiece, data->pipeData->player.peca.x, data->pipeData->player.peca.y,data->id);

		}
		else {
			_tprintf(TEXT("[ERRO] LER no pipe! (InsertPipe) (WriteFile)\n"));
			exit(-1);
		}
		
	}
}


DWORD WINAPI ThreadConectClient(LPVOID param) {
	THREADPIPE* data = (THREADPIPE*)param;
	TCHAR comand[SIZE];
	DWORD n,i, nBytes,ret;
	while (1) {
		_tprintf(_T("[Servidor] Waiting for Player...\n"));
		DWORD result = WaitForMultipleObjects(MAX_PLAYERS, data->hEvents, FALSE, INFINITE);
		i = result - WAIT_OBJECT_0;
		if (i >= 0 && i < MAX_PLAYERS) {
			_tprintf(_T("[Player] Player %d connected...\n"), i);

			if (GetOverlappedResult(data->hPipe[i].hInstance, &data->hPipe[i].overlap, &nBytes, FALSE)) {
				ResetEvent(data->hEvents[i]);
				WaitForSingleObject(data->hMutex, INFINITE);
				data->hPipe[i].active = TRUE;
				ReleaseMutex(data->hMutex);
			}
			data->numPlayer++;
			data->pipeDataInitial->nPlayer = data->numPlayer;
		}
		WriteFile(data->hPipe[i].hInstance, data->pipeDataInitial, sizeof(Pipe), &n, NULL);

		if (data->numPlayer == 2)
			break;

		//WaitForSingleObject(data->pipeData->read, INFINITE);


		ret = ReadFile(data->hPipe[i].hInstance, data->pipeDataInitial, sizeof(Pipe), &n, NULL);
		if (data->pipeDataInitial->solo == 1)
			break; //we don't need to wait for another player
		
	}
	_ftprintf(stderr, TEXT("ThreadNamedPipes ended\n"));
}

DWORD WINAPI Threadkeyboard(LPVOID param) {
	THREADTEC* data = (THREADTEC*)param;
	TCHAR comand[SIZE];
	DWORD aux;

	while (*data->continua)
	{
		_ftprintf(stdout, TEXT("Comand: "));
		_tscanf_s(TEXT("%s"), &comand, SIZE - 1);

		if (*data->continua == 0) {
			return 1;
		}
		
		if (wcscmp(comand, TEXT("start")) == 0) {
			SetEvent(data->sinc->timerStartEvent);             
			_ftprintf(stderr, TEXT("-----------> Started\n"));
		}
		else
		if (wcscmp(comand, TEXT("end")) == 0) {
			SetEvent(data->sinc->endMonitor);
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
	_ftprintf(stderr, TEXT("ThreadKeyboard ended\n"));

}



DWORD WINAPI ThreadComandsMonitor(LPVOID param) { //thread vai servir para ler do buffer circular os comandos do monitor
	THREADCONS* data = (THREADCONS*)param;
	TCHAR comand[SIZE];
	Comand aux;
	LARGE_INTEGER liDueTime;

	while (*data->continua)
	{
		WaitForSingleObject(data->memDados->semServer, INFINITE);
		if (*data->continua == 0) {
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
			
			if (!putWall(data->memDados, aux.wallX, aux.wallY, 2)) { ////mudar isto
				_ftprintf(stderr, TEXT("Error placing wall\n"));
			}else
				SetEvent(data->sinc->printBoard);

			break;
		}
	
	}
}
