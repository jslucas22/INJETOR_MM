 # 💉 MANUAL_MAP INJECTOR 💉


## Oque é isso ?
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

O tipo injeção via Manual Map é de grande utilidade para contornar vários anti-cheats...
pois a dll irá ficar escondida de vários processos que fazem a leitura da memória, tais como o 
ToolHelp32Snapshot... E também, ele percorrerá à lista de módulos que estão vinculados no 
Bloco de ambiente de processo (PEB).

![](Resources/Imagens/bg.png)
