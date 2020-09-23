#include "injecao.h"

using namespace std;

const char qtdDll[] = "C:\\Users\\zLucas\\Documents\\Dev\\C++\\Projetos Pessoais\\INJETORES\\MANUAL_MAP\\Injetor_Manual_Map\\Teste.dll";
const char Processo[] = "teste.exe";

int main() {

	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		DWORD Erro = GetLastError();
		cout << "Falha no CreateToolhelp32Snapshot 0x%X\N" << endl;
		system("pause");
		return 0;
	}

	DWORD idProcesso = 0;
	BOOL bRet = Process32First(hSnap, &PE32);

	while (bRet) {
		if (!strcmp(Processo, PE32.szExeFile)) {
			idProcesso = PE32.th32ProcessID;
			break;
		}
		bRet = Process32Next(hSnap, &PE32);
	}

	CloseHandle(hSnap);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, idProcesso);


	if (!hProc) {
		DWORD Erro = GetLastError();
		cout << "Falha ao abrir o Processo: 0x%X\N" << endl;
		system("pause");
		return 0;
	}

	if (!MapeamentoManual(hProc, qtdDll)) {
		cout << "Falha na implementacao " << endl;
		system("pause");
		return 0;
	}
	CloseHandle(hProc);

	return 0;
}