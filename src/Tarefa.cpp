#include "Tarefa.h"
#include <ArduinoJson.h>
#include <time.h>

Tarefa::Tarefa() 
    : id(0), descricao(""), tarefa_status(TarefaStatus::Recebido) {}

Tarefa::Tarefa(int id, std::string descricao, TarefaStatus tarefa_status)
    : id(id), descricao(descricao), tarefa_status(tarefa_status) {}

Tarefa::Tarefa(int id, std::string descricao, const std::vector<Material>& materiais, TarefaStatus tarefa_status)
    : id(id), descricao(descricao), tarefa_status(tarefa_status), materiais(materiais) {}

int Tarefa::GetId() const {
    return id;
}

std::string Tarefa::GetDescricao() const {
    return descricao;
}

TarefaStatus Tarefa::GetStatus() const {
    return tarefa_status;
}

const std::vector<Material>& Tarefa::GetMateriais() const {
    return materiais;
}

void Tarefa::AtualizarStatus(TarefaStatus novo_status) {
    this->tarefa_status = novo_status;
}

void Tarefa::AdicionarMaterial(const std::string& nome, int quantidade) {
    materiais.push_back({nome, quantidade});
}

void Tarefa::SetMateriais(const std::vector<Material>& mats) {
    this->materiais = mats;
}

JsonObject Tarefa::ConverterParaJSON(JsonDocument& doc, int idPlanit) const {
    JsonObject json = doc.to<JsonObject>();

    json["idPlanit"] = idPlanit;
    
    switch(tarefa_status){
        case TarefaStatus::Recebido:
            json["tipo"] = 2; // Confirmar Recebimento
            break;
        case TarefaStatus::Iniciado:
            json["tipo"] = 3; // Confirmar Início
            break;
        case TarefaStatus::Parado:
            json["tipo"] = 4; // Confirmar Pausa
            break;
        case TarefaStatus::Postergado:
            json["tipo"] = 5; // Confirmar Adiamento
            break;
        case TarefaStatus::Concluido:
            json["tipo"] = 6; // Confirmar Conclusão
            break;
    }

    json["idTarefa"] = id;
    
    time_t agora;
    time(&agora);

    if (agora > 1000000000) {
        json["timestamp"] = (long)agora;
    } else {
        json["timestamp"] = (long)(millis() / 1000);
    }

    return json;
}

String Tarefa::ObterJSONString(int idPlanit) const {
    JsonDocument doc;
    ConverterParaJSON(doc, idPlanit);
    String saida;
    serializeJson(doc, saida);
    return saida;
}

bool Tarefa::CriarDeJSON(const JsonObjectConst& json, Tarefa& outTarefa) {
    if (!json.containsKey("idTarefa") || !json.containsKey("tipo")) {
        return false;
    }

    int tipo = json["tipo"];
    if (tipo != 1) { // Tipo 1 = Receber Tarefa
        return false;
    }

    int id = json["idTarefa"];
    const char* desc = json["descricao"] | "Sem descricao";

    outTarefa = Tarefa(id, std::string(desc), TarefaStatus::Recebido);

    if (json.containsKey("materiais") && json["materiais"].is<JsonArrayConst>()) {
        JsonArrayConst matArray = json["materiais"];
        for (JsonObjectConst matObj : matArray) {
            const char* nomeMat = matObj["material"] | "";
            int qtd = matObj["quantidade"] | 1;
            outTarefa.AdicionarMaterial(nomeMat, qtd);
        }
    }

    return true;
}
