#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>

using f_LoadLibraryA = HINSTANCE(WINAPI*)(const char* nomeArquivo);
using f_GetProcAddress = UINT_PTR(WINAPI*)(HINSTANCE hModule, const char* nomeProcesso);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* cDLL, DWORD dwRasao, void* pReservado);

struct MAPEAMENTO_MANUAL {
	f_LoadLibraryA			pLoadLibraryA;
	f_GetProcAddress		pObterProcesso;
	HINSTANCE				hMod;
};

bool MapeamentoManual(HANDLE hProcesso, const char* qtdDll);
