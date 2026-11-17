#include "LeitorComunicacao.h"
#include <ArduinoJson.h>
#include <ctime>
#include <sys/time.h>
#include <Arduino.h>

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
        
        break;
    }

    case 1:{ 
        int idTarefa = doc["idTarefa"] | 0;
        const char* descricao = doc["descricao"] | "Sem descricao";
        JsonArray materiais = doc["materiais"];
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