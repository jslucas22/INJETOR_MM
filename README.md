# MANUAL MAP INJECTOR

O Manual Map, de forma bem resumida, ele emula o tipo de implementação "LoadLibraryA",
porém o que será feito pelo Manual Map será as seguintes coisas:

* Carregar todos os dados binários brutos.
* Mapear seções no processo de destino.
* Fazer a injeção de shellcode.
* Fazer as realocações necessárias.
* Corrigir às importações.
* Realizar os retornos de chamada TLS
* Chamar a DllMain
* Limpar tudo

O tipo injeção via Manual Map é de grande utilidade para contornar vários anti-cheats...
pois sua biblioteca de vínculo dinâmico ficará oculta de processos que faz a leitura da memória, tais como o 
ToolHelp32Snapshot. E também, ele percorrerá à lista de módulos que estão vinculados no PEB.

![](Resources/Imagens/bg.png)
