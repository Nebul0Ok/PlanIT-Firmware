#pragma once
#include <string>
#include <ArduinoJson.h>

#include "TarefaStatus.h"


class Tarefa{
    public:
    Tarefa(int id, std::string descricao, TarefaStatus tarefa_status);
    std::string GetDescricao();
    void AtualizarStatus(TarefaStatus novo_status);
    JsonObject ConverterParaJSON(JsonDocument& doc);

    private:
    int id;
    std::string descricao;
    TarefaStatus tarefa_status;
};