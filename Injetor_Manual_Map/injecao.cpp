#include "injecao.h"

using namespace std;

void __stdcall shellcode(MAPEAMENTO_MANUAL* pDados);

bool MapeamentoManual(HANDLE hProcesso, const char* qtdDll) {

	BYTE* pBuscarDados = nullptr;
	BYTE* alvo = nullptr;
	IMAGE_NT_HEADERS* pBakNTCabecalho = nullptr;
	IMAGE_OPTIONAL_HEADER* pBakOpCabecalho = nullptr;
	IMAGE_FILE_HEADER* pBackCabecalho = nullptr;

	if (!GetFileAttributesA(qtdDll)) {
		cout << "Arquivo não existe" << endl;
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

	if (tamanhoArquivo <= 0x0900) {
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

	alvo = reinterpret_cast<BYTE*>(VirtualAllocEx(hProcesso, reinterpret_cast<void*>
		(pBakOpCabecalho->ImageBase), pBakOpCabecalho->SizeOfImage,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	if (!alvo) {
		alvo = reinterpret_cast<BYTE*>(VirtualAllocEx(hProcesso, nullptr, pBakOpCabecalho->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!alvo) {
			cout << "Falha ao alocar 0x%X" << endl;
			delete[] pBuscarDados;
			return false;
		}
	}

	MAPEAMENTO_MANUAL data{ 0 };
	data.pLoadLibraryA = LoadLibraryA;
	data.pObterProcesso = reinterpret_cast<f_GetProcAddress>(GetProcAddress);
	auto* pSecaoCabecalho = IMAGE_FIRST_SECTION(pBakNTCabecalho);

	for (UINT i = 0; i != pBackCabecalho->NumberOfSections; i++, ++pSecaoCabecalho) {
		if (pSecaoCabecalho->SizeOfRawData) {
			if (!WriteProcessMemory(hProcesso, alvo + pSecaoCabecalho->VirtualAddress, pBuscarDados + pSecaoCabecalho->PointerToRawData, pSecaoCabecalho->SizeOfRawData, nullptr)) {
				cout << "Nao foi possivel mapear a secao" << endl;
				delete[]pBuscarDados;
				VirtualFreeEx(hProcesso, alvo, 0, MEM_RELEASE); //EM PROGRESSO
			}
		}
	}

	memcpy(pBuscarDados, &data, sizeof(data));
	WriteProcessMemory(hProcesso, alvo, pBuscarDados, 0x1000, nullptr);

	delete[] pBuscarDados;

	void* ptrShellCode = VirtualAllocEx(hProcesso, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!ptrShellCode) {
		cout << "Falha ao alocar na memoria" << endl;
		VirtualFreeEx(hProcesso, alvo, 0, MEM_RELEASE);		
		return false;
	}

	//WriteProcessMemory(hProcesso, alvo, &data, sizeof(data), nullptr);
	WriteProcessMemory(hProcesso, ptrShellCode, shellcode, 0x1000, nullptr);
	HANDLE remoteThread = CreateRemoteThread(hProcesso, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ptrShellCode), alvo, 0, nullptr);

	if (!remoteThread) {
		cout << "Falha ao criar a Thread" << endl;
		VirtualFreeEx(hProcesso, alvo, 0, MEM_RELEASE);
		VirtualFreeEx(hProcesso, ptrShellCode, 0, MEM_RELEASE);
		return false;
	}
	CloseHandle(remoteThread);
	HINSTANCE hVerificar = NULL;
	while (!hVerificar) {
		MAPEAMENTO_MANUAL dados_verificados{ 0 };
		ReadProcessMemory(hProcesso, alvo, &dados_verificados, sizeof(dados_verificados), nullptr);
		hVerificar = dados_verificados.hMod;
		Sleep(15);
	}
	VirtualFreeEx(hProcesso, ptrShellCode, 0, MEM_RELEASE);
	return true;
}

#define RELOC_FLAG32(infoRealocamento) ((infoRealocamento >> 0x0C) == IMAGE_REL_BASED_HIGHLOW)
#define RELOC_FLAG64(infoRealocamento) ((infoRealocamento >> 0x0C) == IMAGE_REL_BASED_DIR64)

#ifdef _WIN64
#define RELOC_FLAG RELOC_FLAG
#else
#define RELOC_FLAG RELOC_FLAG32
#endif

	void __stdcall shellcode(MAPEAMENTO_MANUAL * pDados) {
		if (!pDados) { return; }
		BYTE * byteBase = reinterpret_cast<BYTE*>(pDados);
		auto * pointerOpc = &reinterpret_cast<IMAGE_NT_HEADERS*>(byteBase + reinterpret_cast<IMAGE_DOS_HEADER*>(pDados)->e_lfanew)->OptionalHeader;	
		auto _CarregarBiblioteca = pDados->pLoadLibraryA;
		auto _ObterProcesso = pDados->pObterProcesso;
		auto _DLLprincipal = reinterpret_cast<f_DLL_ENTRY_POINT>(byteBase + pointerOpc->AddressOfEntryPoint);

		BYTE* lDelta = byteBase - pointerOpc->ImageBase;

		if (lDelta) {		
			if (!pointerOpc->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
				return;
			}

			auto* pRealocarDados = reinterpret_cast<IMAGE_BASE_RELOCATION*>(byteBase + pointerOpc->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		
			while (pRealocarDados->VirtualAddress) {
				UINT qtdEntradas = (pRealocarDados->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
				WORD* ptrInformacaoRelativa = reinterpret_cast<WORD*>(pRealocarDados + 1);
				
				for (UINT i = 0; i != qtdEntradas; ++i, ++ptrInformacaoRelativa) {
					if (RELOC_FLAG(*ptrInformacaoRelativa)) {
						UINT_PTR* pFragmento = reinterpret_cast<UINT_PTR*>(byteBase + pRealocarDados->VirtualAddress + ((*ptrInformacaoRelativa) & 0XFFF));
						*pFragmento += reinterpret_cast<UINT_PTR>(lDelta);
					}
				}
				pRealocarDados = reinterpret_cast<IMAGE_BASE_RELOCATION*>(reinterpret_cast<BYTE*>(pRealocarDados)) + pRealocarDados->SizeOfBlock;
			}
		}

		if (pointerOpc->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
			auto* impDescritor = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(byteBase + pointerOpc->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

			while (impDescritor->Name) {

				char* Modulo = reinterpret_cast<char*>(byteBase + impDescritor->Name);

				HINSTANCE hDll = _CarregarBiblioteca(Modulo);
				ULONG_PTR* ptrThunkReferencia = reinterpret_cast<ULONG_PTR*>(byteBase + impDescritor->OriginalFirstThunk);
				ULONG_PTR* ptrFR = reinterpret_cast<ULONG_PTR*>(byteBase + impDescritor->FirstThunk);

				if (!ptrThunkReferencia) {
					ptrThunkReferencia = ptrThunkReferencia;
				}

				for (; *ptrThunkReferencia; ++ptrThunkReferencia, ++ptrFR) {
					if (IMAGE_SNAP_BY_ORDINAL(*ptrThunkReferencia)) {
						*ptrFR = _ObterProcesso(hDll, reinterpret_cast<char*>(*ptrThunkReferencia & 0xFFFF));
					}
					else {
						auto* importar = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(byteBase + (*ptrThunkReferencia));
						*ptrFR = _ObterProcesso(hDll, importar->Name);
					}
				}
				++impDescritor;
			}
		}

		if (pointerOpc->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size) {

			auto* ptrTLS = reinterpret_cast<IMAGE_TLS_DIRECTORY*>(byteBase + pointerOpc->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
			auto* ptrChamada = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(ptrTLS->AddressOfCallBacks);

			for (; ptrChamada && *ptrChamada; ++ptrChamada) {
				(*ptrChamada)(byteBase, DLL_PROCESS_ATTACH, nullptr);
			}
		}
		_DLLprincipal(byteBase, DLL_PROCESS_ATTACH, nullptr);
		pDados->hMod = reinterpret_cast<HINSTANCE>(byteBase);
	}
