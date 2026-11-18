#include "LeitorComunicacao.h"
#include <ArduinoJson.h>
#include <ctime>
#include <sys/time.h>
#include <Arduino.h>

extern GerenciadorTarefa gerenciadorTarefa;

LeitorComunicacao::LeitorComunicacao() : gerenciador(&gerenciadorTarefa) {}

LeitorComunicacao::LeitorComunicacao(GerenciadorTarefa& gerenciador) : gerenciador(&gerenciador) {}

void LeitorComunicacao::inicializar(GerenciadorTarefa& gerenciador){
    this->gerenciador = &gerenciador;
}

void LeitorComunicacao::setGerenciador(GerenciadorTarefa& gerenciador){
    this->gerenciador = &gerenciador;
}

void LeitorComunicacao::lerSerial(){
    while(Serial.available() > 0){
        char letra = Serial.read();

        if(letra == '\n' || letra == '\r'){
            buffer[posicao] = '\0';
            lerBuffer();
            posicao = 0;
        }
        else{
            if(posicao < 999){
                buffer[posicao] = letra;
                posicao++;
            }
        }
    }
}

void LeitorComunicacao::lerBuffer(){
    JsonDocument doc;

    DeserializationError erro = deserializeJson(doc, buffer);

    if(erro){
    Serial.print("Erro ao ler JSON: ");
    Serial.println(erro.c_str());
    return;
    }

    int idPlanit = doc["idPlanit"] | 0;
    int tipo = doc["tipo"] | 0;

    switch (tipo)
    {
    case 0:{
        time_t tempo = doc["tempo"] | 0;
        struct timeval tv = {0};
        tv.tv_sec = tempo;
        tv.tv_usec = 0;

        settimeofday(&tv, nullptr);

        Serial.println(F("\n--- [CASE 0] Atualizacao de Tempo ---"));
        Serial.print(F("Timestamp recebido: "));
        Serial.println((long)tempo);
        Serial.print(F("Status settimeofday: "));
        
        break;
    }

    case 1:{ 
        int idTarefa = doc["idTarefa"] | 0;
        const char* descricao = doc["descricao"] | "Sem descricao";
        JsonArray materiais = doc["materiais"];

        Tarefa t = Tarefa(idTarefa, descricao, TarefaStatus::Recebido);
        
        if (!materiais.isNull() || materiais.size() != 0) {
            for (JsonVariant v : materiais) {
                const char* nome = v["nome"] | "Sem nome";
                int quantidade = v["quantidade"] | 0;

                t.AdicionarMateriais(nome, quantidade);
            }
        }

        if(gerenciador != nullptr){
            gerenciador->AdicionarTarefa(idTarefa, descricao, TarefaStatus::Recebido);
        }

        Serial.println(F("\n--- [CASE 1] Nova Tarefa Recebida ---"));
        Serial.print(F("ID da Tarefa: "));
        Serial.println(idTarefa);
        Serial.print(F("Descricao: "));
        Serial.println(descricao);

        Serial.print(F("Materiais ("));
        Serial.print(materiais.size());
        Serial.println(F(" itens):"));

        if (materiais.isNull() || materiais.size() == 0) {
            Serial.println(F("  -> Nenhum material listado."));
        } else {
            // Percorre e imprime cada item do JsonArray
            for (JsonVariant v : materiais) {
                Serial.print(F("  - "));
                // O serializeJson garante que imprima certo seja texto, número ou outro objeto
                serializeJson(v, Serial); 
                Serial.println();
            }
        }

        break;
    }
    
    default:
        Serial.print("Tipo de mensagem desconhecido: ");
        Serial.println(tipo);
        break;
    }

    


}

/*  ---------========= Apenas para teste ===========---------

    Serial.println("--- NOVA TAREFA ---");
    Serial.print("ID: "); Serial.println(idTarefa);
    Serial.print("Tipo: "); Serial.println(tipo);
    Serial.print("Descricao: "); Serial.println(descricao);
    Serial.println("Materiais necessarios:");

    for (JsonObject item : materiais) {
        const char* nomeMaterial = item["material"] | "Desconhecido";
        int quantidade = item["quantidade"] | 0;

        Serial.print(" - ");
        Serial.print(nomeMaterial);
        Serial.print(" (Qtd: ");
        Serial.print(quantidade);
        Serial.println(")");
    }

    ---------========= Apenas para teste ===========---------
*/  