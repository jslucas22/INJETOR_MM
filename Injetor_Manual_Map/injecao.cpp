#include "injecao.h"

using namespace std;

bool MapeamentoManual(HANDLE hProcesso, const char* qtdDll) {
	
	BYTE*					pBuscarDados	  = nullptr;
	BYTE*					alvo			  = nullptr;
	IMAGE_NT_HEADERS*		pBakNTCabecalho   = nullptr;
	IMAGE_OPTIONAL_HEADER*  pBakOpCabecalho   = nullptr;
	IMAGE_FILE_HEADER*		pBackCabecalho    = nullptr;

	if (!GetFileAttributesA(qtdDll)) {
		return false;
	}
	
	ifstream File(qtdDll, ios::binary | ios::ate);

	if (File.fail()) {
		cout << "Falha ao abrir o arquivo: %X" << endl;
		(DWORD)File.rdstate();
		File.close();
		return false;
	}

	auto tamanhoArquivo = File.tellg();
	
	if (tamanhoArquivo < 0x1000) {
		cout << "O tamanho do arquivo é invalido." << endl;
		File.close();
		return false;
	}

	pBuscarDados = new BYTE[static_cast<UINT_PTR>(tamanhoArquivo)];
	
	if (!pBuscarDados) {
		cout << "Falha no alocamento de memoria" << endl;
		File.close();
		return false;
	}

	File.seekg(0, ios::beg);
	File.read(reinterpret_cast<char*>(pBuscarDados), tamanhoArquivo);
	File.close();

	if (reinterpret_cast<IMAGE_DOS_HEADER*>(pBuscarDados)->e_magic != 0x5A4D) {
		cout << "Arquivo invalido" << endl;
		delete[]pBuscarDados;
		return false;
	}
	pBakNTCabecalho = reinterpret_cast<IMAGE_NT_HEADERS*>(pBuscarDados + reinterpret_cast<IMAGE_DOS_HEADER*>(pBuscarDados)->e_lfanew);
	pBakOpCabecalho = &pBakNTCabecalho->OptionalHeader;
	pBackCabecalho = &pBakNTCabecalho->FileHeader;

#ifdef _WIN64

	if (pBackCabecalho->Machine != IMAGE_FILE_MACHINE_AMD64) {
		cout << "Plataforma Invalida" << endl;
		delete[] pBuscarDados;
		return false;
	}
#else
	if (pBackCabecalho->Machine != IMAGE_FILE_MACHINE_I386) {
		cout << "Plataforma Invalida" << endl;
		delete[] pBuscarDados;
		return false;
	}
#endif

}