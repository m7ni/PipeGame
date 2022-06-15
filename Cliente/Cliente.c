#include <windows.h>
#include <tchar.h>
#include "../Memory.h";
#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")

#define HORIZONTAL_PIPE TEXT("pipes/2.bmp")
#define VERTICAL_PIPE TEXT("pipes/1.bmp")
#define START_PIPE TEXT("pipes/start.bmp")
#define BLANK TEXT("blank.bmp")
#define END_PIPE TEXT("pipes/end.bmp")
#define LEFT90 TEXT("pipes/4.bmp")
#define RIGHT90 TEXT("pipes/3.bmp")
#define RIGHT90_1 TEXT("6.bmp")
#define LEFT90_1 TEXT("pipes/5.bmp")
#define WATER TEXT("pipes/water.bmp")
#define BARRIER TEXT("pipes/barrier.bmp")

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);

TCHAR szProgName[] = TEXT("Base");


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;		// hWnd é o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;		// MSG é uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX é uma estrutura cujos membros servem para 
			  // definir as características da classe da janela
	HANDLE hThread;
	HANDLE hPipe;
	Pipe dados;
	DWORD ret,n;
	HDC memDC = NULL;
	// ============================================================================
	// 1. Definição das características da janela "wcApp" 
	//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
	// ============================================================================
	wcApp.cbSize = sizeof(WNDCLASSEX);      // Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;		         // Instância da janela actualmente exibida 
								   // ("hInst" é parâmetro de WinMain e vem 
										 // inicializada daí)
	wcApp.lpszClassName = szProgName;       // Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;       // Endereço da função de processamento da janela
											// ("TrataEventos" foi declarada no início e
											// encontra-se mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW;  // Estilo da janela: Fazer o redraw se for
											// modificada horizontal ou verticalmente

	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // "hIcon" = handler do ícon normal
										   // "NULL" = Icon definido no Windows
										   // "IDI_AP..." Ícone "aplicação"
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION); // "hIconSm" = handler do ícon pequeno
										   // "NULL" = Icon definido no Windows
										   // "IDI_INF..." Ícon de informação
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato) 
							  // "NULL" = Forma definida no Windows
							  // "IDC_ARROW" Aspecto "seta" 
	wcApp.lpszMenuName = NULL;			// Classe do menu que a janela pode ter
							  // (NULL = não tem menu)
	wcApp.cbClsExtra = 0;				// Livre, para uso particular
	wcApp.cbWndExtra = 0;				// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcApp.cbWndExtra = sizeof(Pipe*);

	// "hbrBackground" = handler para "brush" de pintura do fundo da janela. Devolvido por
	// "GetStockObject".Neste caso o fundo será branco


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


	dados.hPipe = hPipe;
	dados.mutexCliente = CreateMutex(NULL, FALSE, NULL);
	dados.hWnd = hWnd;
	dados.memDC = &memDC;
	CreateThread(NULL, 0, printTabuleiro, &dados, 0, NULL);

	SetWindowLongPtr(hWnd, 0, (LONG_PTR)&dados);
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido por 
					  // "CreateWindow"; "nCmdShow"= modo de exibição (p.e. 
					  // normal/modal); é passado como parâmetro de WinMain()
	UpdateWindow(hWnd);		// Refrescar a janela (Windows envia à janela uma 
	

	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);	// Pré-processamento da mensagem (p.e. obter código 
					   // ASCII da tecla premida)
		DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta ao Windows, que
					   // aguarda até que a possa reenviar à função de 
					   // tratamento da janela, CALLBACK TrataEventos (abaixo)
	}



	return((int)lpMsg.wParam);	// Retorna sempre o parâmetro wParam da estrutura lpMsg
}

DWORD WINAPI printTabuleiro(LPVOID lparam){
{
	PAINTSTRUCT ps;
	Pipe* dados = (Pipe*)lparam;
	RECT rect;
	int sentido = 1;
	DWORD ret,n;
	
	while (1)
	{
		WaitForSingleObject(dados->mutexCliente, INFINITE);
		ret = ReadFile(dados->hPipe, &dados, sizeof(Pipe), &n, NULL);

			if (dados->memDC != NULL)
			{
				/*GetClientRect(dados->hWnd, &rect);
				FillRect(dados->memDC, &rect, CreateSolidBrush(RGB(255, 0, 0)));
				BitBlt(dados->memDC, dados->xBitmap, dados->yBitmap,
					dados->bmp.bmWidth, dados->bmp.bmHeight, dados->bmpDC, 0, 0, SRCCOPY);
					*/
			}
			
	}

		ReleaseMutex(dados->mutexCliente);

		InvalidateRect(dados->hWnd, NULL, TRUE);
		Sleep(30);
	}
	
		
}
LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	Pipe * dados = (Pipe *) GetWindowLongPtr(hWnd, 0);
	HDC hdc;

	RECT rect;
	PAINTSTRUCT ps;
	MINMAXINFO* mmi;
	static HBITMAP quadrado,one,two,three,four,five,six,begin,end,agua;
	static BITMAP bmp;
	static HDC bmpDC;
	static int xBitmap;
	static int yBitmap;

	switch (messg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);

		if (dados->memDC == NULL) //primeira vez
		{
			dados->memDC = CreateCompatibleDC(hdc);
			quadrado = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			SelectObject(dados->memDC, quadrado);
			DeleteObject(quadrado);
			FillRect(dados->memDC, &rect, CreateSolidBrush(RGB(255, 0, 0)));
			BitBlt(dados->memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, bmpDC, 0, 0, SRCCOPY);

		}
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, dados->memDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		dados = (Pipe*)GetWindowLongPtr(hWnd, 0);
		switch (LOWORD(wParam))
		{
		case 0:
			swap_images(dados);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		break;
	case WM_LBUTTONDOWN:


		//changePipe(Client * client, int y, int x);
		break;
	case WM_CLOSE:
			//fazer os closes
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY: // Destruir a janela e terminar o programa 
	// "PostQuitMessage(Exit Status)"
		PostQuitMessage(0);
		break;
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar","maximizar","restaurar")
		// não é efectuado nenhum processamento, apenas se segue o "default" do Windows
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;  // break tecnicamente desnecessário por causa do return
	}
	return(0);}


void swap_images(Pipe* dados) {

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
				dados->tabImages[x][y].image = &dados->imagensP.horizontal;
			}
			else if (dados->player.board[x][y] == 'x') {
				dados->tabImages[x][y].image = &dados->imagensP.horizontal;
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
		}
	}
}

void changePipe(Pipe* dados, int y, int x) {
	TCHAR buf[256], value = NULL;
	DWORD n;

	if (dados->tabImages[x][y].image == &dados->imagensP.blank)
	{
		dados->tabImages[x][y].image = &dados->imagensP.horizontal;
		dados->player.desiredPiece.desiredPiece = 'z';
	}
	else if (dados->tabImages[x][y].image == &dados->imagensP.horizontal)
	{
		dados->tabImages[x][y].image = &dados->imagensP.vertical;
		dados->player.desiredPiece.desiredPiece = 'z';
	}
	else if (dados->tabImages[x][y].image == &dados->imagensP.vertical) {
		dados->tabImages[x][y].image = &dados->imagensP.Right90;
		dados->player.desiredPiece.desiredPiece = 'z';
	}
	else if (dados->tabImages[x][y].image == &dados->imagensP.Right90) {
		dados->tabImages[x][y].image == &dados->imagensP.Left90;
		dados->player.desiredPiece.desiredPiece = 'z';
	
	}
	else if (dados->tabImages[x][y].image == &dados->imagensP.Left90) {
		dados->tabImages[x][y].image == &dados->imagensP.Left_1_90;
		dados->player.desiredPiece.desiredPiece = 'z';
		
	}
	else if (dados->tabImages[x][y].image == &dados->imagensP.Left_1_90) {
		dados->tabImages[x][y].image == &dados->imagensP.Right_1_90;
		dados->player.desiredPiece.desiredPiece = 'z';
		
	}
	else if (dados->tabImages[x][y].image == &dados->imagensP.Right_1_90) {
		dados->tabImages[x][y].image == &dados->imagensP.blank;
		dados->player.desiredPiece.desiredPiece = 'z';
	
	}


	BOOL ret;
	DWORD nBytes;
	WriteFile(dados->hPipe, &dados, sizeof(Pipe), &n, NULL);
}