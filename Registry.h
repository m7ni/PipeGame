#ifndef REGISTRY_H
#define REGISTRY_H

typedef struct {
	unsigned int actualSize;
	unsigned int actualTime;
} REGISTO_DADOS;

int verificaChave(REGISTO_DADOS* pdados);
VOID atualizaChave(DWORD actualSize, DWORD actualTime);

#endif /*REGISTRY_H*/