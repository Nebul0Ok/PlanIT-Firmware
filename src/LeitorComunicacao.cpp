#include "LeitorComunicacao.h"
#include <sys/time.h>
#include <time.h>

bool LeitorComunicacao::horaSincronizadaFlag = false;

LeitorComunicacao::LeitorComunicacao()
    : bufferSerial(""), tempoCallback(nullptr), tarefaCallback(nullptr) {}

bool LeitorComunicacao::ProcessarJSON(const String& jsonString, GerenciadorTarefas& gerenciador, int idPlanitEsperado) {
    if (jsonString.length() == 0) return false;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonString);

    if (err) {
        Serial.print(F("[LeitorComunicacao] Falha ao interpretar JSON: "));
        Serial.println(err.c_str());
        return false;
    }

    if (!doc.containsKey("tipo")) {
        Serial.println(F("[LeitorComunicacao] Campo 'tipo' ausente no JSON recebido."));
        return false;
    }

    int tipo = doc["tipo"];

    // Tipo 0: Receber Tempo do Back-end
    if (tipo == 0) {
        if (!doc.containsKey("tempo")) {
            Serial.println(F("[LeitorComunicacao] Tipo 0 sem campo 'tempo'."));
            return false;
        }

        time_t tempo = doc["tempo"].as<time_t>();
        ConfigurarHora(tempo);

        if (tempoCallback) {
            tempoCallback(tempo);
        }
        return true;
    }

    // Tipo 1: Receber Nova Tarefa do Back-end
    if (tipo == 1) {
        Tarefa novaTarefa;
        if (!Tarefa::CriarDeJSON(doc.as<JsonObjectConst>(), novaTarefa)) {
            Serial.println(F("[LeitorComunicacao] Falha ao criar Tarefa a partir do JSON recebido."));
            return false;
        }

        Serial.print(F("[LeitorComunicacao] Nova tarefa recebida via comunicacao: ID="));
        Serial.print(novaTarefa.GetId());
        Serial.print(F(", Descricao: "));
        Serial.println(novaTarefa.GetDescricao().c_str());

        // Adiciona ao gerenciador de tarefas (que já emite o Tipo 2 - Confirmar Recebimento)
        gerenciador.AdicionarTarefa(novaTarefa, true);

        if (tarefaCallback) {
            tarefaCallback(novaTarefa);
        }
        return true;
    }

    Serial.print(F("[LeitorComunicacao] Tipo de mensagem nao tratada para recepcao: "));
    Serial.println(tipo);
    return false;
}

void LeitorComunicacao::ProcessarStream(Stream& stream, GerenciadorTarefas& gerenciador, int idPlanitEsperado) {
    while (stream.available()) {
        char c = (char)stream.read();

        if (c == '\n' || c == '\r') {
            bufferSerial.trim();
            if (bufferSerial.length() > 0) {
                Serial.print(F("[LeitorComunicacao] Mensagem recebida: "));
                Serial.println(bufferSerial);
                ProcessarJSON(bufferSerial, gerenciador, idPlanitEsperado);
                bufferSerial = "";
            }
        } else {
            bufferSerial += c;
            // Proteção contra estouro de buffer
            if (bufferSerial.length() > 2048) {
                bufferSerial = "";
            }
        }
    }
}

void LeitorComunicacao::ConfigurarHora(time_t tempoEpoch) {
    struct timeval tv;
    tv.tv_sec = tempoEpoch;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    horaSincronizadaFlag = true;

    Serial.print(F("[LeitorComunicacao] Hora do sistema atualizada: "));
    Serial.println(ObterDataHoraFormatada());
}

bool LeitorComunicacao::HoraSincronizada() {
    return horaSincronizadaFlag;
}

String LeitorComunicacao::ObterHoraFormatada(const char* formato) {
    time_t agora;
    time(&agora);
    struct tm infoTempo;
    localtime_r(&agora, &infoTempo);

    char buffer[32];
    strftime(buffer, sizeof(buffer), formato, &infoTempo);
    return String(buffer);
}

String LeitorComunicacao::ObterDataHoraFormatada(const char* formato) {
    return ObterHoraFormatada(formato);
}

time_t LeitorComunicacao::ObterTimestampAtual() {
    time_t agora;
    time(&agora);
    return agora;
}

void LeitorComunicacao::OnTempoRecebido(TempoRecebidoCallback callback) {
    this->tempoCallback = callback;
}

void LeitorComunicacao::OnTarefaRecebida(TarefaRecebidaCallback callback) {
    this->tarefaCallback = callback;
}
