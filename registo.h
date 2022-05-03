#ifndef REGISTO_H
#define REGISTO_H

#define MAX_BOARDSIZE 20
#define MAX_TIMERWATER 30

typedef struct {
	int actualSize;
	int actualTime;
	int sizeBoardMAX;	// Initial size of the board
	int startAguaMAX;		// Time that takes for the water to start running
} REGISTO_DADOS;

int verificaChave(REGISTO_DADOS* pdados);

#endif /*REGISTO_H*/
