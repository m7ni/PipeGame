#include "Memory.h"

BOOL abreFileMap(MemDados* dados) {
    dados->FileMapBoard = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, FICH_MEM_P_A);
    dados->FileBufCircular = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, FICH_MEM_P_B);

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

BOOL fechaHandleMem(MemDados* dados) {
    return CloseHandle(dados->FileMapBoard) && CloseHandle(dados->FileBufCircular);
}

BOOL fechaViewFile(MemDados* dados) {
    return UnmapViewOfFile(dados->VBoard) && UnmapViewOfFile(dados->VBufCircular);
}

BOOL criaSinc(MemDados * sem) {
    //dados->mutexBoard = CreateMutex(NULL, FALSE, MUTEX_BOARD);
    sem->semMonitor = CreateSemaphore(NULL, TAM, TAM, SEMAFORO_BUFFER_M);//escrita
    sem->semServer = CreateSemaphore(NULL, 0, 1, SEMAFORO_BUFFER_S);     //leitura
    sem->mutexSEM = CreateMutex(NULL, FALSE, MUTEX);

    if (sem->semMonitor == NULL || sem->semServer == NULL || sem->mutexSEM == NULL) {
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
    dados->VBufCircular = (BufferCircular*)MapViewOfFile(dados->FileBufCircular, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(BufferCircular));
    if (dados->VBufCircular == NULL)
    {
        _ftprintf(stderr, TEXT("Error creating view to buffer FileMap!\n"));
        fechaHandleMem(dados);
        return FALSE;
    }

    // =============================== Colocar o ponteiro da estrutura a apontar para a vista da Board===============================
    
        dados->VBoard = (Board*)MapViewOfFile(dados->FileMapBoard, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Board));
   

    if (dados->VBoard == NULL)
    {
        _ftprintf(stderr, TEXT("Error creating view to Board FileMap!\n"));
        fechaHandleMem(dados);
        return FALSE;
    }

  

    return TRUE;
}
