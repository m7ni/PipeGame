
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h> 
#include <io.h>
#define TAMANHO 200
#include "../memoria.h"
#include "../registo.h"

typedef struct {
	int continua;
	REGISTO_DADOS registoDados;
}THREADTEC;




DWORD WINAPI ThreadTeclado(LPVOID param) {
	THREADTEC* dados = (THREADTEC*)param;
	TCHAR comando[TAMANHO];
	while (dados->continua)
	{


		//TODO
	}
}


//vamos precisar de shared memory(comunicação com o monitor)
void initialize(){ 






}

int _tmain(int argc, TCHAR* argv[]) {
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	//ver o tamanho do argv[] para saber se vamos buscar os valores default sq


}


