#include "Memory.h"

BOOL abreFileMap(MemDados* dados) {
    dados->FileBufCircular = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, FICH_MEM_P_A);
    dados->FileMapBoard = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, FICH_MEM_P_B);


    if (dados->FileMapBoard == NULL || dados->FileBufCircular == NULL)
        return FALSE;
    return TRUE;
}

BOOL criaFileMap(MemDados* dados) {
    // =============================== FileMap Circular Buffer Struct ===============================
    dados->FileBufCircular = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(BufferCircular), FICH_MEM_P_A);
    if (dados->FileBufCircular == NULL) {
        _ftprintf(stderr, TEXT("Error creating shared Memory!\n"));
        return FALSE;
    }
    // =============================== FileMap Board Struct ===============================
    dados->FileMapBoard = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Board), FICH_MEM_P_B);
    if (dados->FileMapBoard == NULL) {
        _ftprintf(stderr, TEXT("Error creating shared Memory!\n"));
        return FALSE;
    }
 
    return TRUE;
}

BOOL CloseHandleMem(MemDados* dados) {
    return CloseHandle(dados->FileMapBoard) && CloseHandle(dados->FileBufCircular);
}

BOOL CloseViewFile(MemDados* dados) {
    return UnmapViewOfFile(dados->VBoard) && UnmapViewOfFile(dados->VBufCircular);
}

BOOL CloseSem(MemDados* dados) {
    return CloseHandle(dados->mutexSEM) && CloseHandle(dados->semMonitor) && CloseHandle(dados->semServer)&& CloseHandle(dados->mutexBoard);
}

BOOL CloseSinc(Sinc *sinc,DWORD flag) {
    if (flag == 1) {
        CloseHandle(sinc->pauseResumeEvent);
        CloseHandle(sinc->timerStartEvent);

    }


    return CloseHandle(sinc->printBoard);
}


BOOL criaSincBuffer(MemDados * sem) {
    sem->semMonitor = CreateSemaphore(NULL, TAM, TAM, SEMAFORO_BUFFER_M);  //escrita
    sem->semServer = CreateSemaphore(NULL, 0, TAM, SEMAFORO_BUFFER_S);     //leitura
    sem->mutexSEM = CreateMutex(NULL, FALSE, BUFFER_MUTEX);
    sem->mutexBoard = CreateMutex(NULL, FALSE, BOARD_MUTEX);
    if (sem->semMonitor == NULL || sem->semServer == NULL || sem->mutexSEM == NULL || sem->mutexBoard == NULL) {
        _ftprintf(stderr, TEXT("Erro na criação dos mecanismos de sincronização.\n"));
        return FALSE;
    }
    return TRUE;
}

BOOL criaSincGeral(Sinc* sinc, DWORD origin ) {
   
    if (origin == 1) { //only the server need this handles


        sinc->pauseResumeEvent = CreateEvent(NULL,
            TRUE,
            TRUE,
            PAUSE_RESUME_EVENT);

        if (sinc->timerStartEvent == NULL || sinc->pauseResumeEvent == NULL) {
            _ftprintf(stderr, TEXT("Erro na criação dos mecanismos de sincronização.\n"));
            return FALSE;
        }
    }

        sinc->timerStartEvent = CreateEvent(NULL,
            TRUE,
            FALSE,
            TIMER_START_EVENT);

    sinc->printBoard= CreateEvent(NULL,
        FALSE,
        FALSE,
        EVENT_BOARD);

    if (sinc->printBoard == NULL   ) {
        _ftprintf(stderr, TEXT("Erro na criação dos mecanismos de sincronização.\n"));
        return FALSE;
    }
    return TRUE;
}

BOOL criaMapViewOfFiles(MemDados* dados) { 

    if (dados->FileBufCircular == NULL || dados->FileMapBoard == NULL) {
        _ftprintf(stderr, TEXT("Error with the fileMaps!\n"));
        return FALSE;
    }

    // =============================== Colocar o ponteiro da estrutura a apontar para a vista do buffer ===============================
    dados->VBufCircular = (BufferCircular*)MapViewOfFile(dados->FileBufCircular, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (dados->VBufCircular == NULL)
    {
        _ftprintf(stderr, TEXT("Error creating view to buffer FileMap!\n"));
        CloseHandleMem(dados);
        return FALSE;
    }

    // =============================== Colocar o ponteiro da estrutura a apontar para a vista da Board===============================
    
        dados->VBoard = (Board*)MapViewOfFile(dados->FileMapBoard, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Board));
   

    if (dados->VBoard == NULL)
    {
        _ftprintf(stderr, TEXT("Error creating view to Board FileMap!\n"));
        CloseHandleMem(dados);
        return FALSE;
    }

  

    return TRUE;
}
