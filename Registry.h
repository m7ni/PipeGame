#ifndef REGISTRY_H
#define REGISTRY_H

typedef struct {
	unsigned int actualSize;
	unsigned int actualTime;
	int sizeBoardMAX;	// Initial size of the board
	int startAguaMAX;		// Time that takes for the water to start running
} REGISTO_DADOS;

int verificaChave(REGISTO_DADOS* pdados);
VOID atualizaChave(DWORD actualSize, DWORD actualTime);

#endif /*REGISTRY_H*/