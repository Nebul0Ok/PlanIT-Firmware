#pragma once
#include <string>
#include <ArduinoJson.h>
#include <array>

#include "TarefaStatus.h"

struct Materiais{
    char nome[30];
    int quantidade;
};

class Tarefa{
    public:
    Tarefa() = default;
    Tarefa(int id, std::string descricao, TarefaStatus tarefa_status);
    const std::string& GetDescricao() const;
    void AtualizarStatus(TarefaStatus novo_status);
    JsonObject ConverterParaJSON(JsonDocument& doc) const;
    const std::array<Materiais, 15>& GetMateriais() const;
    bool TemMateriais();
    void AdicionarMateriais(const char* nome, int quantidade);

    private:
    int id = -1;
    std::string descricao;
    TarefaStatus tarefa_status = TarefaStatus::Parado;
    std::array<Materiais, 15> ListaMateriais;
    int quantidadeMateriais=0;
};