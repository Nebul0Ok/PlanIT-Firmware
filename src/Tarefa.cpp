#include "Tarefa.h"
#include <ArduinoJson.h>
#include <time.h>

#include "TarefaStatus.h"

Tarefa::Tarefa(int id, std::string descricao, TarefaStatus tarefa_status):
    id(id),
    descricao(descricao),
    tarefa_status(tarefa_status)
{}

std::string Tarefa::GetDescricao(){
    return descricao;
}

void Tarefa::AtualizarStatus(TarefaStatus novo_status){
    this->tarefa_status = novo_status;
}

JsonObject Tarefa::ConverterParaJSON(JsonDocument& doc){
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