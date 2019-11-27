# InterDomainRouting
Inter-AS Routing and Path Inflation. "Network Algorithms and Performance" course project.


é preciso checkar se vêm entradas repetidas no ficheiro de entrada?

será que rende, depois de ler o ficheiro, actualizar o graph->V para o real numero de vertices, pois assim no dijkstra alguns ciclos que iriam iterar até passavam só a iterar até esse nr de vertices? (tipo inicializar vetor de custos)

testar meter a função less no ficheiro heaps p ver se fica mais rapido (fazer externs dos recursos que forem necessários).

caso n seja necessário para a contagem de hops os wt[v], n preciso do lastcost, posso usar só o wt[v].

o commercially connected está a dar segfault se apagar a ligação 6-7 no ficehiro exemplos.

ver se há possibilidade de ver ASes que sejam 0