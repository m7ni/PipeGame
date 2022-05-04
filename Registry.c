
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "Registry.h"

#define TAM 256
#define BOARDSIZE 7
#define TIMERWATER 30

int verificaChave(REGISTO_DADOS* pdados) {
	HKEY chave;
	LONG lResult;
	TCHAR chave_nome[TAM], par_valor[TAM], dados[TAM];
	DWORD dataSize = sizeof(dados);
	_stprintf_s(chave_nome, TAM, TEXT("Software\\SO2\\Dados"));

	// Verificar se já existe esta chave registada
	lResult = RegOpenKeyEx(HKEY_CURRENT_USER, chave_nome, 0, KEY_ALL_ACCESS, &chave);

	if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			_tprintf(TEXT("There where not found previous data.. Using default values\n"));
			if (RegCreateKeyEx(HKEY_CURRENT_USER, chave_nome, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &chave, &lResult) == ERROR_SUCCESS) { // Cria a chave
				if (lResult == REG_CREATED_NEW_KEY) { // key created

					_stprintf_s(par_valor, TAM, TEXT("size"));
					_stprintf_s(dados, TAM, TEXT("%d"), BOARDSIZE);
					pdados->actualSize = BOARDSIZE;

					if (RegSetValueEx(chave, par_valor, 0, REG_SZ, dados, sizeof(TCHAR) * _tcslen(par_valor)) == ERROR_SUCCESS) {
						_tprintf(_T("Size of the board: % d\n"), BOARDSIZE);
					}
					else {
						_tprintf(_T("Erro creating value in Registry"));
						RegCloseKey(chave);
						return 0;
					}


					_stprintf_s(par_valor, TAM, TEXT("time"));
					_stprintf_s(dados, TAM, TEXT("%d"), TIMERWATER);
					pdados->actualTime = TIMERWATER;

					if (RegSetValueEx(chave, par_valor, 0, REG_SZ, dados, sizeof(TCHAR) * _tcslen(par_valor)) == ERROR_SUCCESS) {
						_tprintf(_T("Timer: %d\n"), TIMERWATER);
					}
					else {
						_tprintf(_T("Erro creating value in Registry"));
						RegCloseKey(chave);
						return 0;
					}
				}
			}
			else { // Error Creating key
				_tprintf(TEXT("Erro ao criar a chave...\n"));
				return 0;
			}
		}
		else { // It was not possible to open the key
			_tprintf(TEXT("Erro a abrir a chave...\n"));
			return 0;
		}
	}
	else { // Key already exists, getting values
		int aux;
		// Getting Board Size Values
		_stprintf_s(par_valor, TAM, TEXT("size"));
		if (RegQueryValueEx(chave, par_valor, NULL, NULL, (LPBYTE)dados, &dataSize) == ERROR_SUCCESS) {
			if ((aux = _ttoi(dados)) > 0) {
				pdados->actualSize = aux;
			}
			else {
				_tprintf(_T("Error consulting Board Size values\n"));
				RegCloseKey(chave);
				return 0;
			}
		}
		else {
			_tprintf(_T("Error consulting Board Size values\n"));
			RegCloseKey(chave);
			return 0;
		}

		// Getting Water Timer Values
		_stprintf_s(par_valor, TAM, TEXT("time"));
		if (RegQueryValueEx(chave, par_valor, NULL, NULL, (LPBYTE)dados, &dataSize) == ERROR_SUCCESS) {

			if ((aux = _ttoi(dados)) > 0) {
				pdados->actualTime = aux;
			}
			else {
				_tprintf(_T("Error consulting Water Timer values\n"));
				RegCloseKey(chave);
				return 0;
			}
		}
		else {
			_tprintf(_T("Error consulting Water Timer values\n"));
			RegCloseKey(chave);
			return 0;
		}
	}

	RegCloseKey(chave); // fecha a chave
	return 1;
}
