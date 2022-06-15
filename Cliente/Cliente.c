#include <windows.h>
#include <tchar.h>
#include "../Memory.h";
#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")

/* ===================================================== */
/* Programa base (esqueleto) para aplica��es Windows     */
/* ===================================================== */
// Cria uma janela de nome "Janela Principal" e pinta fundo de branco
// Modelo para programas Windows:
//  Composto por 2 fun��es: 
//	WinMain()     = Ponto de entrada dos programas windows
//			1) Define, cria e mostra a janela
//			2) Loop de recep��o de mensagens provenientes do Windows
//     TrataEventos()= Processamentos da janela (pode ter outro nome)
//			1) � chamada pelo Windows (callback) 
//			2) Executa c�digo em fun��o da mensagem recebida

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);

// Nome da classe da janela (para programas de uma s� janela, normalmente este nome � 
// igual ao do pr�prio programa) "szprogName" � usado mais abaixo na defini��o das 
// propriedades do objecto janela


TCHAR szProgName[] = TEXT("Base");

// ============================================================================
// FUN��O DE IN�CIO DO PROGRAMA: WinMain()
// ============================================================================
// Em Windows, o programa come�a sempre a sua execu��o na fun��o WinMain()que desempenha
// o papel da fun��o main() do C em modo consola WINAPI indica o "tipo da fun��o" (WINAPI
// para todas as declaradas nos headers do Windows e CALLBACK para as fun��es de
// processamento da janela)
// Par�metros:
//   hInst: Gerado pelo Windows, � o handle (n�mero) da inst�ncia deste programa 
//   hPrevInst: Gerado pelo Windows, � sempre NULL para o NT (era usado no Windows 3.1)
//   lpCmdLine: Gerado pelo Windows, � um ponteiro para uma string terminada por 0
//              destinada a conter par�metros para o programa 
//   nCmdShow:  Par�metro que especifica o modo de exibi��o da janela (usado em  
//        	   ShowWindow()

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;		// hWnd � o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;		// MSG � uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX � uma estrutura cujos membros servem para 
			  // definir as caracter�sticas da classe da janela
	HANDLE hThread;
	HANDLE hPipe;
	Pipe dados;
	DWORD ret,n;
	HDC memDC = NULL;
	// ============================================================================
	// 1. Defini��o das caracter�sticas da janela "wcApp" 
	//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
	// ============================================================================
	wcApp.cbSize = sizeof(WNDCLASSEX);      // Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;		         // Inst�ncia da janela actualmente exibida 
								   // ("hInst" � par�metro de WinMain e vem 
										 // inicializada da�)
	wcApp.lpszClassName = szProgName;       // Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = TrataEventos;       // Endere�o da fun��o de processamento da janela
											// ("TrataEventos" foi declarada no in�cio e
											// encontra-se mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW;  // Estilo da janela: Fazer o redraw se for
											// modificada horizontal ou verticalmente

	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // "hIcon" = handler do �con normal
										   // "NULL" = Icon definido no Windows
										   // "IDI_AP..." �cone "aplica��o"
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION); // "hIconSm" = handler do �con pequeno
										   // "NULL" = Icon definido no Windows
										   // "IDI_INF..." �con de informa��o
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato) 
							  // "NULL" = Forma definida no Windows
							  // "IDC_ARROW" Aspecto "seta" 
	wcApp.lpszMenuName = NULL;			// Classe do menu que a janela pode ter
							  // (NULL = n�o tem menu)
	wcApp.cbClsExtra = 0;				// Livre, para uso particular
	wcApp.cbWndExtra = 0;				// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcApp.cbWndExtra = sizeof(Pipe*);

	// "hbrBackground" = handler para "brush" de pintura do fundo da janela. Devolvido por
	// "GetStockObject".Neste caso o fundo ser� branco


	if (!RegisterClassEx(&wcApp))
		return(0);
	
	hWnd = CreateWindow(
		szProgName,			// Nome da janela (programa) definido acima
		TEXT("PLAYER"),// Texto que figura na barra do t�tulo
		WS_OVERLAPPEDWINDOW,	// Estilo da janela (WS_OVERLAPPED= normal)
		400,		// Posi��o x pixels (default=� direita da �ltima)
		200,		// Posi��o y pixels (default=abaixo da �ltima)
		800,		// Largura da janela (em pixels)
		500,		// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir de
						// outra) ou HWND_DESKTOP se a janela for a primeira, 
						// criada a partir do "desktop"
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,		// handle da inst�ncia do programa actual ("hInst" � 
						// passado num dos par�metros de WinMain()
		0);				// N�o h� par�metros adicionais para a janela
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
		if (MessageBox(hWnd, TEXT("�s o Player UM\nO jogo vai ser solo ?"),
			TEXT("TIPO DE JOGO?"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		{
			dados.solo = 1;
		}

		if (!WriteFile(hPipe, &dados, sizeof(Pipe), &n, NULL))
			_tprintf(_T("[ERRO] Escrever no pipe! (WriteFile)\n"));
	}
	else {
		MessageBox(hWnd, TEXT("�s o Player[2], vais jogar um competitivo"), TEXT("Informa��o"), MB_ICONEXCLAMATION | MB_OK);
	}


		ReadFile(hPipe, &dados, sizeof(Pipe), &n, NULL);

	 


	dados.hPipe = hPipe;
	dados.mutexCliente = CreateMutex(NULL, FALSE, NULL);
	dados.hWnd = hWnd;
	dados.memDC = &memDC;
	SetWindowLongPtr(hWnd, 0, (LONG_PTR)&dados);
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido por 
					  // "CreateWindow"; "nCmdShow"= modo de exibi��o (p.e. 
					  // normal/modal); � passado como par�metro de WinMain()
	UpdateWindow(hWnd);		// Refrescar a janela (Windows envia � janela uma 
	

	while (GetMessage(&lpMsg, NULL, 0, 0)) {
		TranslateMessage(&lpMsg);	// Pr�-processamento da mensagem (p.e. obter c�digo 
					   // ASCII da tecla premida)
		DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta ao Windows, que
					   // aguarda at� que a possa reenviar � fun��o de 
					   // tratamento da janela, CALLBACK TrataEventos (abaixo)
	}



	return((int)lpMsg.wParam);	// Retorna sempre o par�metro wParam da estrutura lpMsg
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
				GetClientRect(dados->hWnd, &rect);
				FillRect(dados->memDC, &rect, CreateSolidBrush(RGB(255, 0, 0)));
				BitBlt(dados->memDC, dados->xBitmap, dados->yBitmap,
					dados->bmp.bmWidth, dados->bmp.bmHeight, dados->bmpDC, 0, 0, SRCCOPY);
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
		quadrado = (HBITMAP)LoadImage(NULL, TEXT("0.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(quadrado, sizeof(bmp), &bmp);

		hdc = GetDC(hWnd);
		bmpDC = CreateCompatibleDC(hdc);
		SelectObject(bmpDC, quadrado);
		ReleaseDC(hWnd, hdc);

		GetClientRect(hWnd, &rect);
		xBitmap = (0) - (0);
		yBitmap = (0) - (0);


		one = (HBITMAP)LoadImage(NULL, TEXT("1.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(one, sizeof(one), &one);

		two = (HBITMAP)LoadImage(NULL, TEXT("2.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(two, sizeof(two), &two);

		three = (HBITMAP)LoadImage(NULL, TEXT("3.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(quadrado, sizeof(quadrado), &quadrado);

		four = (HBITMAP)LoadImage(NULL, TEXT("4.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(four, sizeof(four), &four);

		five = (HBITMAP)LoadImage(NULL, TEXT("5.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(five, sizeof(five), &five);

		six = (HBITMAP)LoadImage(NULL, TEXT("6.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(six, sizeof(six), &six);

		begin = (HBITMAP)LoadImage(NULL, TEXT("begin.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(begin, sizeof(begin), &begin);

		end = (HBITMAP)LoadImage(NULL, TEXT("end.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(end, sizeof(end), &end);

		agua = (HBITMAP)LoadImage(NULL, TEXT("water.bmp"), IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
		GetObject(agua, sizeof(agua), &agua);
		CreateThread(NULL, 0, printTabuleiro, &dados, 0, NULL);
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

	case WM_LBUTTONDOWN:


		//lan�ar fun��o que envia o que ele clica
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
		// n�o � efectuado nenhum processamento, apenas se segue o "default" do Windows
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;  // break tecnicamente desnecess�rio por causa do return
	}
	return(0);}
