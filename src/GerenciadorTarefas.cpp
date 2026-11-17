#include "GerenciadorTarefas.h"

GerenciadorTarefas::GerenciadorTarefas(int idPlanit)
    : idPlanit(idPlanit), indiceAtual(-1), envioCallback(nullptr) {}

void GerenciadorTarefas::SetIdPlanit(int id) {
    this->idPlanit = id;
}

int GerenciadorTarefas::GetIdPlanit() const {
    return this->idPlanit;
}

void GerenciadorTarefas::AdicionarTarefa(const Tarefa& tarefa, bool enviarConfirmacao) {
    // Verifica se a tarefa já existe pelo ID
    for (size_t i = 0; i < tarefas.size(); i++) {
        if (tarefas[i].GetId() == tarefa.GetId()) {
            tarefas[i] = tarefa;
            indiceAtual = (int)i;
            if (enviarConfirmacao) {
                ConfirmarRecebimento(tarefas[i]);
            }
            return;
        }
    }

    tarefas.push_back(tarefa);
    if (indiceAtual == -1) {
        indiceAtual = 0;
    }

    if (enviarConfirmacao) {
        ConfirmarRecebimento(tarefas.back());
    }
}

bool GerenciadorTarefas::TemTarefas() const {
    return !tarefas.empty() && indiceAtual >= 0 && indiceAtual < (int)tarefas.size();
}

int GerenciadorTarefas::GetTotalTarefas() const {
    return (int)tarefas.size();
}

int GerenciadorTarefas::GetIndiceAtual() const {
    return indiceAtual;
}

Tarefa* GerenciadorTarefas::GetTarefaAtual() {
    if (!TemTarefas()) return nullptr;
    return &tarefas[indiceAtual];
}

const Tarefa* GerenciadorTarefas::GetTarefaAtual() const {
    if (!TemTarefas()) return nullptr;
    return &tarefas[indiceAtual];
}

bool GerenciadorTarefas::SelecionarTarefa(int indice) {
    if (indice >= 0 && indice < (int)tarefas.size()) {
        indiceAtual = indice;
        return true;
    }
    return false;
}

bool GerenciadorTarefas::ProximaTarefa() {
    if (tarefas.empty()) return false;
    indiceAtual = (indiceAtual + 1) % tarefas.size();
    return true;
}

bool GerenciadorTarefas::TarefaAnterior() {
    if (tarefas.empty()) return false;
    indiceAtual = (indiceAtual - 1 + tarefas.size()) % tarefas.size();
    return true;
}

String GerenciadorTarefas::dispararAtualizacao(Tarefa& tarefa, TarefaStatus novoStatus) {
    tarefa.AtualizarStatus(novoStatus);
    String json = tarefa.ObterJSONString(idPlanit);

    Serial.print(F("[GerenciadorTarefas] Enviando atualizacao (status: "));
    Serial.print(GetStatusNome(novoStatus));
    Serial.print(F("): "));
    Serial.println(json);

    if (envioCallback) {
        envioCallback(json, novoStatus, tarefa.GetId());
    }

    return json;
}

String GerenciadorTarefas::IniciarTarefaAtual() {
    if (!TemTarefas()) return "";
    return dispararAtualizacao(*GetTarefaAtual(), TarefaStatus::Iniciado);
}

String GerenciadorTarefas::PausarTarefaAtual() {
    if (!TemTarefas()) return "";
    return dispararAtualizacao(*GetTarefaAtual(), TarefaStatus::Parado);
}

String GerenciadorTarefas::AdiarTarefaAtual() {
    if (!TemTarefas()) return "";
    return dispararAtualizacao(*GetTarefaAtual(), TarefaStatus::Postergado);
}

String GerenciadorTarefas::ConcluirTarefaAtual() {
    if (!TemTarefas()) return "";
    return dispararAtualizacao(*GetTarefaAtual(), TarefaStatus::Concluido);
}

String GerenciadorTarefas::ConfirmarRecebimento(const Tarefa& tarefa) {
    // Gera JSON com tipo = 2 (Confirmar Recebimento)
    JsonDocument doc;
    JsonObject json = doc.to<JsonObject>();
    json["idPlanit"] = idPlanit;
    json["tipo"] = 2;
    json["idTarefa"] = tarefa.GetId();

    time_t agora;
    time(&agora);
    if (agora > 1000000000) {
        json["timestamp"] = (long)agora;
    } else {
        json["timestamp"] = (long)(millis() / 1000);
    }

    String saida;
    serializeJson(doc, saida);

    Serial.print(F("[GerenciadorTarefas] Enviando confirmacao de recebimento: "));
    Serial.println(saida);

    if (envioCallback) {
        envioCallback(saida, TarefaStatus::Recebido, tarefa.GetId());
    }

    return saida;
}

void GerenciadorTarefas::OnEnviarMensagem(MensagemEnvioCallback callback) {
    this->envioCallback = callback;
}

const char* GerenciadorTarefas::GetStatusNome(TarefaStatus status) {
    switch (status) {
        case TarefaStatus::Recebido:   return "Recebido";
        case TarefaStatus::Iniciado:   return "Iniciado";
        case TarefaStatus::Parado:     return "Parado / Pausado";
        case TarefaStatus::Postergado: return "Postergado / Adiado";
        case TarefaStatus::Concluido:  return "Concluído";
        default:                       return "Desconhecido";
    }
}
