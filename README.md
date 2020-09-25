# MANUAL MAP INJECTOR

O Manual Map, de forma bem resumida, ele emula o tipo de implementação "LoadLibraryA",
porém o que será feito pelo Manual Map será as seguintes coisas:

1.Carregar todos os dados binários brutos.
2.Mapear seções no processo de destino.
3.Fazer a injeção de shellcode.
3.Fazer as realocações necessárias.
4.Corrigir às importações.
5.Realizar os retornos de chamada TLS
6.Chamar a DllMain
7.Limpar tudo

O tipo injeção via Manual Map é de grande utilidade para contornar vários anti-cheats...
pois sua biblioteca de vínculo dinâmico ficará oculta de processos que faz a leitura da memória, tais como o 
ToolHelp32Snapshot. E também, ele percorrerá à lista de módulos que estão vinculados no PEB.

![](Resources/Imagens/bg.png)
