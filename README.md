 # 💉 MANUAL_MAP INJECTOR 💉
 ### um injetor de DLL gratuito

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

# Anti Malware Bloqueando ⚠️
Bom, caso isso venha ocorrer, a explicação é bem simples, pode ficar tranquilo quanto a isso,
caso seu antivírus ou anti malware acusou este software como um arquivo potencialmente indesejado,
é devido ao controle de memória que é tomado por este... além dele realizar a leitura de tudo 
que está na memória, ele também irá escrever e "libertar" espaço na memória de acesso randômica

![](Resources/Imagens/bg.png)
