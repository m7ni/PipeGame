#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include "resource.h"
#include <winuser.h>
#include <wingdi.h>
#include "../Memory.h";
#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")

#define HORIZONTAL_PIPE TEXT("pipes/2.bmp")
#define VERTICAL_PIPE TEXT("pipes/1.bmp")
#define START_PIPE TEXT("pipes/begin.bmp")
#define BLANK TEXT("pipes/0.bmp")
#define END_PIPE TEXT("pipes/end.bmp")
#define LEFT90 TEXT("pipes/4.bmp")
#define RIGHT90 TEXT("pipes/3.bmp")
#define RIGHT90_1 TEXT("pipes/6.bmp")
#define LEFT90_1 TEXT("pipes/5.bmp")
#define WATER TEXT("pipes/water.bmp")
#define BARRIER TEXT("pipes/barrier.bmp")
#define BEGINU TEXT("pipes/beginU.bmp")
#define BEGINH TEXT("pipes/beginH.bmp")

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI printTabuleiro(LPVOID lparam);
HINSTANCE hInstance;
void swapImages(Pipe* dados);
TCHAR szProgName[] = TEXT("Base");
void loadImages(Image* image, HWND hWnd, TCHAR* image_path);
void init(Pipe* dados);
void changePipe(Pipe* dados, DWORD x, DWORD y);
void clear_cell(Pipe* dados, DWORD y, DWORD x);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd, hWnd2;		// hWnd é o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;		// MSG é uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para 
	HANDLE hThread;
	HANDLE hPipe;
	Pipe dados;
	DWORD ret,n;
	HDC memDC = NULL;


	wcApp.cbSize = sizeof(WNDCLASSEX);      
	wcApp.hInstance = hInst;		       						  					
	wcApp.lpszClassName = szProgName;       // Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;       // Endereço da função de processamento da janela
	wcApp.style = CS_HREDRAW | CS_VREDRAW;  // Estilo da janela: Fazer o redraw se for
	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // "hIcon" = handler do ícon normal
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION); // "hIconSm" = handler do ícon pequeno
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato) 
	wcApp.lpszMenuName = NULL;			// Classe do menu que a janela pode ter
	wcApp.cbClsExtra = 0;				// Livre, para uso particular
	wcApp.cbWndExtra = 0;				// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcApp.cbWndExtra = sizeof(Pipe*);

	if (!RegisterClassEx(&wcApp))
		return(0);

	
	hWnd = CreateWindow(
		szProgName,			// Nome da janela (programa) definido acima
		TEXT("PLAYER"),// Texto que figura na barra do título
		WS_OVERLAPPEDWINDOW,	// Estilo da janela (WS_OVERLAPPED= normal)
		400,		// Posição x pixels (default=à direita da última)
		200,		// Posição y pixels (default=abaixo da última)
		800,		// Largura da janela (em pixels)
		500,		// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir de			
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,		// handle da instância do programa actual ("hInst" é 	
		0);				// Não há parâmetros adicionais para a janela
	  // ============================================================================
	  // 4. Mostrar a janela
	  // ============================================================================
	
	hWnd2 =  CreateWindow(
		TEXT("SO2 - Client"),	// Nome da janela (programa) definido acima
		TEXT("SO2 - Client"),// Texto que figura na barra do t tulo
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,	// Estilo da janela (WS_OVERLAPPED= normal)
		CW_USEDEFAULT,		// Posi  o x pixels (default=  direita da  ltima)
		CW_USEDEFAULT,		// Posi  o y pixels (default=abaixo da  ltima)
		500,		// Largura da janela (em pixels)
		500,		// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir de
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,		// handle da inst ncia do programa actual ("hInst"   
		// passado num dos par metros de WinMain()
		0);


	if (!WaitNamedPipe(PIPE_NAME, 5000)) {
		exit(-1);
	}

	hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (CreateFile)\n"), PIPE_NAME);
		exit(-1);
	}
	ret = ReadFile(hPipe, &dados, sizeof(Pipe), &n, NULL);
	
	if (dados.nPlayer == 1) {
		dados.solo = 0;
		if (MessageBox(hWnd, TEXT("És o Player UM\nO jogo vai ser solo ?"),
			TEXT("TIPO DE JOGO?"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		{
			dados.solo = 1;
		}

		if (!WriteFile(hPipe, &dados, sizeof(Pipe), &n, NULL))
			_tprintf(_T("[ERRO] Escrever no pipe! (WriteFile)\n"));
	}
	else {
		MessageBox(hWnd, TEXT("És o Player[2], vais jogar um competitivo"), TEXT("Informação"), MB_ICONEXCLAMATION | MB_OK);
	}


		ReadFile(hPipe, &dados, sizeof(Pipe), &n, NULL);

		loadImages(&dados.imagensP.horizontal, hWnd, (TCHAR*)HORIZONTAL_PIPE);
		loadImages(&dados.imagensP.start, hWnd, (TCHAR*)START_PIPE);
		loadImages(&dados.imagensP.end, hWnd, (TCHAR*)END_PIPE);
		loadImages(&dados.imagensP.vertical, hWnd, (TCHAR*)VERTICAL_PIPE);
		loadImages(&dados.imagensP.Right90, hWnd, (TCHAR*)RIGHT90);
		loadImages(&dados.imagensP.Left90, hWnd, (TCHAR*)LEFT90);
		loadImages(&dados.imagensP.Left_1_90, hWnd, (TCHAR*)LEFT90_1);
		loadImages(&dados.imagensP.Right_1_90, hWnd, (TCHAR*)RIGHT90_1);
		loadImages(&dados.imagensP.blank, hWnd, (TCHAR*)BLANK);
		loadImages(&dados.imagensP.water, hWnd, (TCHAR*)WATER);
		loadImages(&dados.imagensP.barrier, hWnd, (TCHAR*)BARRIER);
		loadImages(&dados.imagensP.beginU, hWnd, (TCHAR*)BEGINU);
		loadImages(&dados.imagensP.beginH, hWnd, (TCHAR*)BEGINH);


	dados.hPipe = hPipe;
	dados.mutexCliente = CreateMutex(NULL, FALSE, NULL);
	dados.eventRead = CreateEvent(NULL,
		TRUE,
		FALSE,
		EVENT_READ_ONE);
	dados.hWnd = hWnd;
	dados.memDC = &memDC;
	dados.ft = 1;
	init(&dados);
	swapImages(&dados);

	if (CreateThread(NULL, 0, printTabuleiro, &dados, 0, NULL) == NULL)
	{
		return -1;
	}


	SetWindowLongPtr(hWnd, 0, (LONG_PTR)&dados);
	SetWindowLongPtr(hWnd2, 0, (LONG_PTR)&dados);

	ShowWindow(hWnd, nCmdShow);				
	UpdateWindow(hWnd);	

	ShowWindow(hWnd2, nCmdShow);
	UpdateWindow(hWnd2);
	
	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);	// Pré-processamento da mensagem (p.e. obter código 
					   // ASCII da tecla premida)
		DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta ao Windows, que
					   // aguarda até que a possa reenviar à função de 
					   // tratamento da janela, CALLBACK TrataEventos (abaixo)
	}

	return((int)lpMsg.wParam);	// Retorna sempre o parâmetro wParam da estrutura lpMsg
}

void loadImages(Image* image, HWND hWnd, TCHAR* image_path) {
	HDC hdc;
	HANDLE hBmp = (HBITMAP)LoadImage(NULL, image_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(image->bitmap), &image->bitmap);
	hdc = GetDC(hWnd);
	image->DC = CreateCompatibleDC(hdc);
	SelectObject(image->DC, hBmp);
	ReleaseDC(hWnd, hdc);
}

DWORD WINAPI printTabuleiro(LPVOID lparam)
{
	Pipe* dados = (Pipe*)lparam;
	PAINTSTRUCT ps;
	
	RECT rect;
	int sentido = 1;
	DWORD ret,n;
	
	while (1)
	{

		WaitForSingleObject(dados->mutexCliente, INFINITE);
		//WaitForSingleObject(dados->eventRead, INFINITE);
		ret = ReadFile(dados->hPipe, &dados, sizeof(Pipe), &n, NULL);
		if (ret != 0) {
			swapImages(dados);

			InvalidateRect(dados->hWnd, NULL, TRUE);
		}
		ReleaseMutex(dados->mutexCliente);
	}
}

LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	Pipe * dados = (Pipe *) GetWindowLongPtr(hWnd, 0);
	HDC hdc;
	vect2 pos;
	RECT rect;
	PAINTSTRUCT ps;
	MINMAXINFO* mmi;
	static BITMAP bmp;
	static HDC bmpDC;
	static int xBitmap;
	static int yBitmap;
	HWND button;

	switch (messg) {
	case WM_CREATE:
		//GetClientRect(hWnd, &rect);
		button = CreateWindow(TEXT("button"), TEXT("player name"), WS_VISIBLE | WS_CHILD,
			0, 150, 255, 50, hWnd, (HMENU)103, NULL, NULL);
		break;

	case WM_PAINT:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);

		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 0)));

		for (int y = 0; y < dados->player.actualSize; y++) {
			for (int x = 0; x < dados->player.actualSize; x++) {
				BitBlt(hdc, dados->tabImages[x][y].coord.x, dados->tabImages[x][y].coord.y, dados->tabImages[x][y].image->bitmap.bmWidth,
					dados->tabImages[x][y].image->bitmap.bmHeight, dados->tabImages[x][y].image->DC, 0, 0, SRCCOPY);
			}
		}

		EndPaint(hWnd, &ps);
	
		break;
	case WM_RBUTTONDOWN:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);

		pos.x = GET_X_LPARAM(lParam);
		pos.y = GET_Y_LPARAM(lParam);

		for (DWORD y = 0; y < dados->player.actualSize; y++) {
			for (DWORD x = 0; x < dados->player.actualSize; x++) {
				if (pos.x > dados->tabImages[y][x].coord.x && pos.x < dados->tabImages[y][x].coord.x + dados->tabImages[y][x].image->bitmap.bmWidth
					&&
					pos.y > dados->tabImages[y][x].coord.y && pos.y < dados->tabImages[y][x].coord.y + dados->tabImages[y][x].image->bitmap.bmHeight) {
					clear_cell(dados, y, x);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);
		pos.x = GET_X_LPARAM(lParam);
		pos.y = GET_Y_LPARAM(lParam);

		for (DWORD y = 0; y < dados->player.actualSize; y++) {
			for (DWORD x = 0; x < dados->player.actualSize; x++) {
				if (pos.x > dados->tabImages[y][x].coord.x && pos.x < dados->tabImages[y][x].coord.x + dados->tabImages[y][x].image->bitmap.bmWidth
					&&
					pos.y > dados->tabImages[y][x].coord.y && pos.y < dados->tabImages[y][x].coord.y + dados->tabImages[y][x].image->bitmap.bmHeight) {
					changePipe(dados, y, x);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
		}
		break;
	case WM_CLOSE:
			//fazer os closes
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
	default:
		
return(DefWindowProc(hWnd, messg, wParam, lParam));
break;  
	}
	return(0); }


	void swapImages(Pipe* dados) {
		
		for (DWORD x = 0; x < dados->player.actualSize; x++) {
			for (DWORD y = 0; y < dados->player.actualSize; y++) {
				if (dados->player.board[x][y] == '.')
				{
					dados->tabImages[x][y].image = &dados->imagensP.blank;
				}
				else if (dados->player.board[x][y] == 'w')
				{
					dados->tabImages[x][y].image = &dados->imagensP.barrier;
				}
				else if (dados->player.board[x][y] == 'z') {
					if (dados->ft == 1) {
						dados->tabImages[x][y].image = &dados->imagensP.beginH;
					}
					else {
						dados->tabImages[x][y].image = &dados->imagensP.horizontal;
					}

				}
				else if (dados->player.board[x][y] == 'x') {
					if (dados->ft == 1) {
						dados->tabImages[x][y].image = &dados->imagensP.beginU;
					}
					else {
						dados->tabImages[x][y].image = &dados->imagensP.vertical;
					}
				}
				else if (dados->player.board[x][y] == 's') {
					dados->tabImages[x][y].image = &dados->imagensP.Right90;
				}
				else if (dados->player.board[x][y] == 'r') {
					dados->tabImages[x][y].image = &dados->imagensP.Left_1_90;
				}
				else if (dados->player.board[x][y] == 'd') {
					dados->tabImages[x][y].image = &dados->imagensP.Left90;
				}
				else if (dados->player.board[x][y] == 'l') {
					dados->tabImages[x][y].image = &dados->imagensP.Right_1_90;
				}
				else if (dados->player.board[x][y] == 'a') {
					dados->tabImages[x][y].image = &dados->imagensP.water;
				}
				else if (dados->player.board[x][y] == 'e') {
					dados->tabImages[x][y].image = &dados->imagensP.end;
				}
			}
		}

		dados->ft = 0;
	
	}

	void changePipe(Pipe* dados, DWORD x, DWORD y) {
		TCHAR buf[256], value = NULL;
		DWORD n;

		if (dados->tabImages[x][y].image == &dados->imagensP.blank)
		{
			dados->tabImages[x][y].image = &dados->imagensP.horizontal;
			dados->player.peca.desiredPiece = 'z';
		}
		else if (dados->tabImages[x][y].image == &dados->imagensP.horizontal)
		{
			dados->tabImages[x][y].image = &dados->imagensP.vertical;
			dados->player.peca.desiredPiece = 'x';
		}
		else if (dados->tabImages[x][y].image == &dados->imagensP.vertical) {
			dados->tabImages[x][y].image = &dados->imagensP.Right90;
			dados->player.peca.desiredPiece = 's';
		}
		else if (dados->tabImages[x][y].image == &dados->imagensP.Right90) {
			dados->tabImages[x][y].image = &dados->imagensP.Left90;
			dados->player.peca.desiredPiece = 'd';

		}
		else if (dados->tabImages[x][y].image == &dados->imagensP.Left90) {
			dados->tabImages[x][y].image = &dados->imagensP.Left_1_90;
			dados->player.peca.desiredPiece = 'r';

		}
		else if (dados->tabImages[x][y].image == &dados->imagensP.Left_1_90) {
			dados->tabImages[x][y].image = &dados->imagensP.Right_1_90;
			dados->player.peca.desiredPiece = 'l';

		}
		else if (dados->tabImages[x][y].image == &dados->imagensP.Right_1_90) {
			dados->tabImages[x][y].image = &dados->imagensP.horizontal;
			dados->player.peca.desiredPiece = 'z';

		}

		dados->player.peca.x = x;
		dados->player.peca.y = y;


		if(!WriteFile(dados->hPipe, dados, sizeof(Pipe), &n, NULL)){
			return -1;
		}
}

void clear_cell(Pipe* dados, DWORD y, DWORD x) {
	if (dados->tabImages[y][x].image != &dados->imagensP.beginH && dados->tabImages[y][x].image != &dados->imagensP.beginU && dados->tabImages[y][x].image != &dados->imagensP.end && dados->tabImages[y][x].image != &dados->imagensP.barrier) {
		dados->tabImages[y][x].image = &dados->imagensP.blank;
	}
	
}

void init(Pipe* dados) {

	int pos_y = 0;
	int pos_x = 0;
	
	for (int y = 0; y < dados->player.actualSize; y++) {
		for (int x = 0; x < dados->player.actualSize; x++) {
			dados->tabImages[y][x].coord.y = pos_y;
			dados->tabImages[y][x].coord.x = pos_x;
			pos_x += 60;
		}
		pos_x = 0;
		pos_y += 60;
	}

}
