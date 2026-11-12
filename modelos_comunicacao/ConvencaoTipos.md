# Guia das numerações de tipos

Para reduzir a quantidade de dados enviados pela comunicação, o parâmetro '"tipo"' foi substituído por um dado do tipo inteiro para a sua otimização.

Código  |   Tipo                   |   Descrição

0           Receber Tempo               Dispositivo recebe o tempo do back-end

1           Receber Tarefa              Dispositivo recebe uma nova tarefa, por meio de 
                                        seu ID, descriçãoe a sua lista de materiais

2           Confirmar Recebimento       Dispositivo avisa ao back-end que recebeu a tarefa

3           Confirmar Início            Dispositivo avisa ao back-end que o usuário iniciou
                                        a tarefa.

4           Confirmar Pausa             Dispositivo avisa ao back-end que o usuário pausou 
                                        a tarefa.

5           Confirmar Adiamento         Dispositivo avisa ao back-end que o usuário adiou a
                                        tarefa.

6           Confirmar Conclusão         Dispositivo avisa ao back-end que o usuário 
                                        concluiu a tarefa.