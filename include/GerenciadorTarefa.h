#pragma once

#include <array>
#include "Tarefa.h"

class GerenciadorTarefa{
    public:
    void AdicionarTarefa(int id, const std::string& descricao, TarefaStatus status);
    void AdicionarTarefa(const Tarefa& tarefa);
    size_t GetQuantidade() const;
    void TrocarTarefaAtual();
    Tarefa* GetTarefaAtual();
    const Tarefa* GetTarefaAtual() const;
    bool TemTarefas() const { return quantidadeOcupada > 0; }
    
    private:
    std::array<Tarefa, 16> tarefas;
    size_t quantidadeOcupada = 0;
    short tarefaAtual = -1;
};