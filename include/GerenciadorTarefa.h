#pragma once

#include <array>
#include "Tarefa.h"

class GerenciadorTarefa{
    public:
    void AdicionarTarefa(int id, const std::string& descricao, TarefaStatus status);
    size_t GetQuantidade() const;
    void TrocarTarefaAtual();
    
    private:
    std::array<Tarefa, 16> tarefas;
    size_t quantidadeOcupada = 0;
    short tarefaAtual = -1;
};