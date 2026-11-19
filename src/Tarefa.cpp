#include "Tarefa.h"
#include <ArduinoJson.h>
#include <time.h>

#include "TarefaStatus.h"

Tarefa::Tarefa(int id, std::string descricao, TarefaStatus tarefa_status):
    id(id),
    descricao(descricao),
    tarefa_status(tarefa_status)
{}

const std::string& Tarefa::GetDescricao() const{
    return descricao;
}

void Tarefa::AtualizarStatus(TarefaStatus novo_status){
    this->tarefa_status = novo_status;
}

JsonObject Tarefa::ConverterParaJSON(JsonDocument& doc) const{
    JsonObject json = doc.to<JsonObject>();

    json["idPlanit"]= id;
    
    switch(tarefa_status){
        case TarefaStatus::Recebido:
        json["tipo"] = 2;
        break;

        case TarefaStatus::Iniciado:
        json["tipo"] = 3;
        break;

        case TarefaStatus::Parado:
        json["tipo"] = 4;
        break;

        case TarefaStatus::Postergado:
        json["tipo"] = 5;
        break;

        case TarefaStatus::Concluido:
        json["tipo"] = 6;
        break;
    }

    json["idTarefa"] = id;
    
    time_t agora;
    time(&agora);

    if (agora > 1700000000) {
        json["timestamp"] = agora;
    } else {
        json["timestamp"] = nullptr;
    }

    return json;
}

const std::array<Materiais, 15>& Tarefa::GetMateriais() const {
    return ListaMateriais;
}

bool Tarefa::TemMateriais() const {
    return quantidadeMateriais > 0;
}

void Tarefa::AdicionarMateriais(const char* nome, int quantidade){
    if (quantidadeMateriais >= 15) return;
    Materiais m;
    
    strncpy(m.nome, nome, sizeof(m.nome) - 1);
    m.nome[sizeof(m.nome) - 1] = '\0';
    m.quantidade = quantidade;
    
    ListaMateriais[quantidadeMateriais] = m;
    quantidadeMateriais++;
}