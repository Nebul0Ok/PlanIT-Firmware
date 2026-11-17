#pragma once
#include <string>
#include <vector>
#include <ArduinoJson.h>

#include "TarefaStatus.h"

struct Material {
    std::string nome;
    int quantidade;
};

class Tarefa{
    public:
    Tarefa();
    Tarefa(int id, std::string descricao, TarefaStatus tarefa_status = TarefaStatus::Recebido);
    Tarefa(int id, std::string descricao, const std::vector<Material>& materiais, TarefaStatus tarefa_status = TarefaStatus::Recebido);

    int GetId() const;
    std::string GetDescricao() const;
    TarefaStatus GetStatus() const;
    const std::vector<Material>& GetMateriais() const;

    void AtualizarStatus(TarefaStatus novo_status);
    void AdicionarMaterial(const std::string& nome, int quantidade);
    void SetMateriais(const std::vector<Material>& mats);

    JsonObject ConverterParaJSON(JsonDocument& doc, int idPlanit = 123456789) const;
    String ObterJSONString(int idPlanit = 123456789) const;

    static bool CriarDeJSON(const JsonObjectConst& json, Tarefa& outTarefa);

    private:
    int id;
    std::string descricao;
    TarefaStatus tarefa_status;
    std::vector<Material> materiais;
};
