#pragma once
#include <string>
#include <ArduinoJson.h>

#include "TarefaStatus.h"


class Tarefa{
    public:
    Tarefa() = default;
    Tarefa(int id, std::string descricao, TarefaStatus tarefa_status);
    const std::string& GetDescricao();
    void AtualizarStatus(TarefaStatus novo_status);
    JsonObject ConverterParaJSON(JsonDocument& doc) const;

    private:
    int id = -1;
    std::string descricao;
    TarefaStatus tarefa_status = TarefaStatus::Parado;
};