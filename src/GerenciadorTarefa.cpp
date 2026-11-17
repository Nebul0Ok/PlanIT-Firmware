#include "GerenciadorTarefa.h"
#include <array>
#include "Tarefa.h"

void GerenciadorTarefa::AdicionarTarefa(int id, const std::string& descricao, TarefaStatus status){
    if(quantidadeOcupada >= tarefas.size()){
        return;
    }
    tarefas[quantidadeOcupada] = Tarefa(id,descricao,status);

    if(tarefaAtual == -1){
        tarefaAtual = 0;
    }

    quantidadeOcupada++;
}