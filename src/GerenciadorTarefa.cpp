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

void GerenciadorTarefa::AdicionarTarefa(const Tarefa& tarefa){
    if(quantidadeOcupada >= tarefas.size()){
        return;
    }
    tarefas[quantidadeOcupada] = tarefa;

    if(tarefaAtual == -1){
        tarefaAtual = 0;
    }

    quantidadeOcupada++;
}

size_t GerenciadorTarefa::GetQuantidade() const{
    return quantidadeOcupada;
}

void GerenciadorTarefa::TrocarTarefaAtual(){
    if(quantidadeOcupada == 0){
        tarefaAtual = -1;
        return;
    }
    tarefaAtual = (tarefaAtual + 1) % quantidadeOcupada;
}

Tarefa* GerenciadorTarefa::GetTarefaAtual(){
    if(tarefaAtual >= 0 && (size_t)tarefaAtual < quantidadeOcupada){
        return &tarefas[tarefaAtual];
    }
    return nullptr;
}

const Tarefa* GerenciadorTarefa::GetTarefaAtual() const{
    if(tarefaAtual >= 0 && (size_t)tarefaAtual < quantidadeOcupada){
        return &tarefas[tarefaAtual];
    }
    return nullptr;
}