#pragma once

#include <vector>
#include <functional>
#include <Arduino.h>
#include "Tarefa.h"
#include "TarefaStatus.h"

typedef std::function<void(const String& jsonMsg, TarefaStatus status, int idTarefa)> MensagemEnvioCallback;

class GerenciadorTarefas {
public:
    GerenciadorTarefas(int idPlanit = 123456789);

    void SetIdPlanit(int id);
    int GetIdPlanit() const;

    // Adição e navegação de tarefas
    void AdicionarTarefa(const Tarefa& tarefa, bool enviarConfirmacao = true);
    bool TemTarefas() const;
    int GetTotalTarefas() const;
    int GetIndiceAtual() const;
    Tarefa* GetTarefaAtual();
    const Tarefa* GetTarefaAtual() const;

    bool SelecionarTarefa(int indice);
    bool ProximaTarefa();
    bool TarefaAnterior();

    // Ações dos botões principais do Menu
    String IniciarTarefaAtual();    // Tipo 3 (Confirmar Início)
    String PausarTarefaAtual();     // Tipo 4 (Confirmar Pausa)
    String AdiarTarefaAtual();      // Tipo 5 (Confirmar Adiamento / Postergado)
    String ConcluirTarefaAtual();   // Tipo 6 (Confirmar Conclusão)
    String ConfirmarRecebimento(const Tarefa& tarefa); // Tipo 2 (Confirmar Recebimento)

    // Callbacks
    void OnEnviarMensagem(MensagemEnvioCallback callback);

    static const char* GetStatusNome(TarefaStatus status);

private:
    int idPlanit;
    std::vector<Tarefa> tarefas;
    int indiceAtual;
    MensagemEnvioCallback envioCallback;

    String dispararAtualizacao(Tarefa& tarefa, TarefaStatus novoStatus);
};
