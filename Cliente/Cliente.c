#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include "resource.h"
#include <winuser.h>
#include <wingdi.h>
#include "../Memory.h";
#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")

#define HORIZONTAL_PIPEZERO TEXT("pipes/2.bmp")
#define VERTICAL_PIPEZERO TEXT("pipes/1.bmp")
#define START_PIPEZERO TEXT("pipes/begin.bmp")
#define BLANKZERO TEXT("pipes/0.bmp")
#define END_PIPEZERO TEXT("pipes/end.bmp")
#define LEFT90ZERO TEXT("pipes/4.bmp")
#define RIGHT90ZERO TEXT("pipes/3.bmp")
#define RIGHT90_1ZERO TEXT("pipes/5.bmp")
#define LEFT90_1ZERO TEXT("pipes/6.bmp")
#define WATERZERO TEXT("pipes/water.bmp")
#define BARRIERZERO TEXT("pipes/barrier.bmp")
#define BEGINUZERO TEXT("pipes/beginU.bmp")
#define BEGINHZERO TEXT("pipes/beginH.bmp")
#define WINZERO TEXT("pipes/lost.bmp")
#define LOSTZERO TEXT("pipes/win.bmp")

#define HORIZONTAL_PIPEONE TEXT("pipes1/2.bmp")
#define VERTICAL_PIPEONE TEXT("pipes1/1.bmp")
#define START_PIPEONE TEXT("pipes1/begin.bmp")
#define BLANKONE TEXT("pipes1/0.bmp")
#define END_PIPEONE TEXT("pipes1/end.bmp")
#define LEFT90ONE TEXT("pipes1/4.bmp")
#define RIGHT90ONE TEXT("pipes1/3.bmp")
#define RIGHT90_1ONE TEXT("pipes1/5.bmp")
#define LEFT90_1ONE TEXT("pipes1/6.bmp")
#define WATERONE TEXT("pipes1/water.bmp")
#define BARRIERONE TEXT("pipes1/barrier.bmp")
#define BEGINUONE TEXT("pipes1/beginU.bmp")
#define BEGINHONE TEXT("pipes1/beginH.bmp")
#define WINONE TEXT("pipes1/lost.bmp")
#define LOSTONE TEXT("pipes1/win.bmp")

void pause_water(Pipe* dados, vect2 selected);
void unpause_water(Pipe* dados, vect2 selected);
LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI printTabuleiro(LPVOID lparam);
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
	DWORD ret,n,dwMode;
	HDC memDC = NULL;
	BOOL fSuccess;

	wcApp.cbSize = sizeof(WNDCLASSEX);      
	wcApp.hInstance = hInst;		       						  					
	wcApp.lpszClassName = szProgName;       // Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;       // Endereço da função de processamento da janela
	wcApp.style = CS_HREDRAW | CS_VREDRAW;  // Estilo da janela: Fazer o redraw se for
	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // "hIcon" = handler do ícon normal
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION); // "hIconSm" = handler do ícon pequeno
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato) 
	wcApp.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);		// Classe do menu que a janela pode ter
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
	



	if (!WaitNamedPipe(PIPE_NAME, 1000)) {
		MessageBox(hWnd, TEXT("O servidor não está aberto/ o servido não aceita mais jogadores"), TEXT("Informação"), MB_ICONEXCLAMATION | MB_OK);
		exit(-1);
	}

	hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED, NULL);
	if (hPipe == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (CreateFile)\n"), PIPE_NAME);
		exit(-1);
	}

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe, // handle para o pipe
		&dwMode, // novo pipe mode
		NULL, // maximum bytes = NULL (não altera)
		NULL);

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

		loadImages(&dados.imagensP[0].horizontal, hWnd, (TCHAR*)HORIZONTAL_PIPEZERO);
		loadImages(&dados.imagensP[0].start, hWnd, (TCHAR*)START_PIPEZERO);
		loadImages(&dados.imagensP[0].end, hWnd, (TCHAR*)END_PIPEZERO);
		loadImages(&dados.imagensP[0].vertical, hWnd, (TCHAR*)VERTICAL_PIPEZERO);
		loadImages(&dados.imagensP[0].Right90, hWnd, (TCHAR*)RIGHT90ZERO);
		loadImages(&dados.imagensP[0].Left90, hWnd, (TCHAR*)LEFT90ZERO);
		loadImages(&dados.imagensP[0].Left_1_90, hWnd, (TCHAR*)LEFT90_1ZERO);
		loadImages(&dados.imagensP[0].Right_1_90, hWnd, (TCHAR*)RIGHT90_1ZERO);
		loadImages(&dados.imagensP[0].blank, hWnd, (TCHAR*)BLANKZERO);
		loadImages(&dados.imagensP[0].water, hWnd, (TCHAR*)WATERZERO);
		loadImages(&dados.imagensP[0].barrier, hWnd, (TCHAR*)BARRIERZERO);
		loadImages(&dados.imagensP[0].beginU, hWnd, (TCHAR*)BEGINUZERO);
		loadImages(&dados.imagensP[0].beginH, hWnd, (TCHAR*)BEGINHZERO);
		loadImages(&dados.imagensP[0].beginU, hWnd, (TCHAR*)BEGINUZERO);
		loadImages(&dados.imagensP[0].beginH, hWnd, (TCHAR*)BEGINHZERO);
		loadImages(&dados.imagensP[0].win, hWnd, (TCHAR*)WINZERO);
		loadImages(&dados.imagensP[0].lost, hWnd, (TCHAR*)LOSTZERO);

		loadImages(&dados.imagensP[1].horizontal, hWnd, (TCHAR*)HORIZONTAL_PIPEONE);
		loadImages(&dados.imagensP[1].start, hWnd, (TCHAR*)START_PIPEONE);
		loadImages(&dados.imagensP[1].end, hWnd, (TCHAR*)END_PIPEONE);
		loadImages(&dados.imagensP[1].vertical, hWnd, (TCHAR*)VERTICAL_PIPEONE);
		loadImages(&dados.imagensP[1].Right90, hWnd, (TCHAR*)RIGHT90ONE);
		loadImages(&dados.imagensP[1].Left90, hWnd, (TCHAR*)LEFT90ONE);
		loadImages(&dados.imagensP[1].Left_1_90, hWnd, (TCHAR*)LEFT90_1ONE);
		loadImages(&dados.imagensP[1].Right_1_90, hWnd, (TCHAR*)RIGHT90_1ONE);
		loadImages(&dados.imagensP[1].blank, hWnd, (TCHAR*)BLANKONE);
		loadImages(&dados.imagensP[1].water, hWnd, (TCHAR*)WATERONE);
		loadImages(&dados.imagensP[1].barrier, hWnd, (TCHAR*)BARRIERONE);
		loadImages(&dados.imagensP[1].beginU, hWnd, (TCHAR*)BEGINUONE);
		loadImages(&dados.imagensP[1].beginH, hWnd, (TCHAR*)BEGINHONE);
		loadImages(&dados.imagensP[1].win, hWnd, (TCHAR*)WINONE);
		loadImages(&dados.imagensP[1].lost, hWnd, (TCHAR*)LOSTONE);


	
	dados.currentSet = 0;
	dados.hPipe = hPipe;
	dados.mutexCliente = CreateMutex(NULL, FALSE, NULL);
	dados.hWnd = hWnd;
	dados.memDC = &memDC;
	dados.ft = 1;
	dados.hover = 0;
	if (MessageBox(hWnd, TEXT("O set de pipes atualmente escolhido é o 1, deseja trocar para o segundo ?"),
		TEXT("PIPE SET"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
	{
		dados.currentSet = 1;
	}
	criaSincClient(&dados);
	init(&dados);
	swapImages(&dados);
	

	if (CreateThread(NULL, 0, printTabuleiro, &dados, 0, NULL) == NULL)
	{
		return -1;
	}


	SetWindowLongPtr(hWnd, 0, (LONG_PTR)&dados);

	ShowWindow(hWnd, nCmdShow);				
	UpdateWindow(hWnd);	

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
	OVERLAPPED ov;
	HANDLE eventOp = CreateEvent(NULL,
		TRUE,
		FALSE,
		NULL);

	while (1)
	{
		ZeroMemory(&ov, sizeof(ov));
		ov.hEvent = eventOp;

		WaitForSingleObject(dados->mutexCliente, INFINITE);
		ret = ReadFile(dados->hPipe, dados, sizeof(Pipe), &n, &ov);
		if (ret) {
			
		}
		if (!ret && GetLastError() == ERROR_IO_PENDING) {

			WaitForSingleObject(ov.hEvent, INFINITE);

		}
	
		swapImages(dados);

		InvalidateRect(dados->hWnd, NULL, TRUE);
		ReleaseMutex(dados->mutexCliente);
	}
}

INT_PTR CALLBACK DlgProc(HWND dlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	Pipe* dados = (Pipe*)GetWindowLongPtr(GetParent(dlg), 0);

	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemText(dlg, IDC_EDIT1, "");
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(dlg, IDC_EDIT1, dados->nome, 80);
		
		case IDCANCEL:
			EndDialog(dlg, IDCANCEL);
			return TRUE;
		default:
			return TRUE;
		}
	
	case WM_CLOSE:
		EndDialog(dlg,(INT_PTR)0);
		return TRUE;
	}
	return FALSE;
}

LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	Pipe * dados = (Pipe *) GetWindowLongPtr(hWnd, 0);
	HDC hdc;
	vect2 pos;
	RECT rect;
	PAINTSTRUCT ps;
	MINMAXINFO* mmi;
	BOOL fMouseTracking = FALSE;
	static BITMAP bmp;
	static HDC bmpDC;
	static int xBitmap;
	static int yBitmap;
	HWND button;
	INT_PTR ret;
	switch (messg) {
	case WM_CREATE:
		//GetClientRect(hWnd, &rect);
		
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
	case WM_MOUSEMOVE:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);
		pos.x = GET_X_LPARAM(lParam);
		pos.y = GET_Y_LPARAM(lParam);

		for (DWORD y = 0; y < dados->player.actualSize; y++) {
			for (DWORD x = 0; x < dados->player.actualSize; x++) {
				if (!fMouseTracking && pos.x > dados->tabImages[y][x].coord.x && pos.x < dados->tabImages[y][x].coord.x + dados->tabImages[y][x].image->bitmap.bmWidth
					&&
					pos.y > dados->tabImages[y][x].coord.y && pos.y < dados->tabImages[y][x].coord.y + dados->tabImages[y][x].image->bitmap.bmHeight)
				{
					dados->selected_cell.y = y;
					dados->selected_cell.x = x;

					TRACKMOUSEEVENT tme;

					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_HOVER;
					tme.hwndTrack = hWnd;
					tme.dwHoverTime = 1 * 1E3;
					fMouseTracking = TrackMouseEvent(&tme);
				}
			}
		}
		break;
	case WM_MOUSEHOVER:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);

		fMouseTracking = FALSE;
		dados->hover++;
		if (dados->hover <= 3) {
			pause_water(dados, dados->selected_cell);
		}
		break;
	case WM_MOUSELEAVE:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);
		fMouseTracking = FALSE;
		unpause_water(dados, dados->selected_cell);
		break;
	case WM_COMMAND:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);
		switch (wParam) {
		case ID_SETINGS_CHANGENAME:
			 ret = DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			if (ret == IDOK) {
				MessageBoxEx(NULL, dados->nome, _T("Resultado"), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL, 0);
			}
			break;
		case ID_SETINGS_SHOWNAME:
			MessageBoxEx(NULL, dados->nome, _T("INFORMAÇÂO"), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL, 0);
			break;
		case ID_SETINGS_CHANGEBITSET:

			if (dados->currentSet == 1)
				dados->currentSet = 0;
			else
				dados->currentSet = 1;
			swapImages(&dados);
			InvalidateRect(dados->hWnd, NULL, TRUE);
			break;
		case ID_SETINGS_EXIT:
			PostQuitMessage(0);
			break;
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
	return(0); 
}


void swapImages(Pipe* dados) {

	for (DWORD x = 0; x < dados->player.actualSize; x++) {
		for (DWORD y = 0; y < dados->player.actualSize; y++) {
			if (!dados->player.lose || !dados->player.win) {
				if (dados->player.board[x][y] == '.')
				{
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].blank;
				}
				else if (dados->player.board[x][y] == 'w')
				{
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].barrier;
				}
				else if (dados->player.board[x][y] == 'z') {
					if (dados->ft == 1) {
						dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].beginH;
					}
					else {
						dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].horizontal;
					}

				}
				else if (dados->player.board[x][y] == 'x') {
					if (dados->ft == 1) {
						dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].beginU;
					}
					else {
						dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].vertical;
					}
				}
				else if (dados->player.board[x][y] == 's') {
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Right90;
				}
				else if (dados->player.board[x][y] == 'r') {
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Left_1_90;
				}
				else if (dados->player.board[x][y] == 'd') {
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Left90;
				}
				else if (dados->player.board[x][y] == 'l') {
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Right_1_90;
				}
				else if (dados->player.board[x][y] == 'a') {
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].water;
				}
				else if (dados->player.board[x][y] == 'e') {
					dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].end;
				}
			}
			else if (dados->player.lose) {
				dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].lost;

			}
			else if (dados->player.win) {
				dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].win;
			}
		}
	}

	dados->ft = 0;
	
}

void changePipe(Pipe* dados, DWORD x, DWORD y) {
	TCHAR buf[256], value = NULL;
	DWORD n,ret;
	OVERLAPPED ov;

	HANDLE eventOp = CreateEvent(NULL,
		TRUE,
		FALSE,
		NULL);

	if (dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].beginH &&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].beginU &&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].end &&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].barrier)  {

		if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].blank)
		{
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].horizontal;
			dados->player.peca.desiredPiece = 'z';
		}
		else if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].horizontal)
		{
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].vertical;
			dados->player.peca.desiredPiece = 'x';
		}
		else if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].vertical) {
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Right90;
			dados->player.peca.desiredPiece = 's';
		}
		else if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].Right90) {
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Left90;
			dados->player.peca.desiredPiece = 'd';

		}
		else if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].Left90) {
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Left_1_90;
			dados->player.peca.desiredPiece = 'r';

		}
		else if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].Left_1_90) {
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].Right_1_90;
			dados->player.peca.desiredPiece = 'l';S

		}
		else if (dados->tabImages[x][y].image == &dados->imagensP[dados->currentSet].Right_1_90) {
			dados->tabImages[x][y].image = &dados->imagensP[dados->currentSet].horizontal;
			dados->player.peca.desiredPiece = 'z';

		}

		dados->player.peca.x = x;
		dados->player.peca.y = y;
		

		ZeroMemory(&ov, sizeof(ov));
		ov.hEvent = eventOp;

		ret = WriteFile(dados->hPipe, dados, sizeof(Pipe), &n, &ov);

			
		if (!ret && GetLastError() == ERROR_IO_PENDING) {
			WaitForSingleObject(ov.hEvent, INFINITE);
		}
			
	}
}

void clear_cell(Pipe* dados, DWORD y, DWORD x) {
	DWORD n;
	if (dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].beginH &&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].beginU &&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].end &&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].water&&
		dados->tabImages[y][x].image != &dados->imagensP[dados->currentSet].barrier) {
		dados->tabImages[y][x].image = &dados->imagensP[dados->currentSet].blank;
		dados->player.peca.desiredPiece = '.';
	

	dados->player.peca.x = y;
	dados->player.peca.y = x;
	
	if (!WriteFile(dados->hPipe, dados, sizeof(Pipe), &n, NULL)) {
		return -1;
	}
	}
}

void pause_water(Pipe* dados, vect2 selected) {
	if (dados->tabImages[selected.x][selected.y].image == &dados->imagensP[dados->currentSet].water) {
		ResetEvent(dados->eventStopW);
	}
}

void unpause_water(Pipe* dados, vect2 selected) {


	SetEvent(dados->eventStopW);
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
