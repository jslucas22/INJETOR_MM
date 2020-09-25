 # 💉 MANUAL_MAP INJECTOR 💉
 
 ```cpp
#include <iostream> 

using namespace std;

int main () { 

cout << "Um injetor de DLL que contorna vários anti cheats" << endl;

return 0;
}
```

## Oque é isso ? 🤔
O Manual Map, de forma bem resumida, ele é um tipo injeção de diretivas de vínculo dinâmicas
que emula o tipo de implementação "LoadLibraryA".
Porém o que é feito pelo nosso ManualMap são as seguintes coisas:

* Carregar todos os dados binários brutos.
* Mapear seções no processo de destino.
* Fazer a injeção do shellcode.
* Realocar.
* Corrigir às importações.
* Fazer o retorno de chamada TLS.
* "Invocar" a Dll Principal.
* Apagar os rastros.

## É realmente seguro? ✅
O tipo injeção via Manual Map é de grande utilidade para contornar vários anti-cheats...
pois a dll irá ficar escondida de vários processos que fazem a leitura da memória, tais como o 
ToolHelp32Snapshot... E também, ele percorrerá à lista de módulos que estão vinculados no 
Bloco de ambiente de processo (PEB).

# "Software Malicioso" ⚠️
Bom, caso o seu anti malware venha a bloquear este software, a explicação é bem simples, 
caso seu antivírus ou anti malware acusou este software como um arquivo potencialmente indesejado,
é devido ao controle de memória que é tomado por este... além dele realizar a leitura de tudo 
que está na memória, ele também irá escrever e "libertar" espaço na memória de acesso randômica

# Licença 📃

```
MIT License

Copyright (c) 2020 José Lucas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

![](Resources/Imagens/bg.png)

```
Baseado no Injetor do GUIDEDHACKING
Visite: GUIDEDHACKING.COM
```
