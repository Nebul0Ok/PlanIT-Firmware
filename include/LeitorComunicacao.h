#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>
#include "GerenciadorTarefas.h"
#include "Tarefa.h"

typedef std::function<void(time_t tempo)> TempoRecebidoCallback;
typedef std::function<void(const Tarefa& tarefa)> TarefaRecebidaCallback;

class LeitorComunicacao {
public:
    LeitorComunicacao();

    // Processamento de JSON direto (string)
    bool ProcessarJSON(const String& jsonString, GerenciadorTarefas& gerenciador, int idPlanitEsperado = -1);

    // Processamento a partir de um Stream (ex: Serial ou WiFiClient)
    void ProcessarStream(Stream& stream, GerenciadorTarefas& gerenciador, int idPlanitEsperado = -1);

    // Configuração e consulta de Hora
    static void ConfigurarHora(time_t tempoEpoch);
    static bool HoraSincronizada();
    static String ObterHoraFormatada(const char* formato = "%H:%M:%S");
    static String ObterDataHoraFormatada(const char* formato = "%d/%m/%Y %H:%M:%S");
    static time_t ObterTimestampAtual();

    // Callbacks
    void OnTempoRecebido(TempoRecebidoCallback callback);
    void OnTarefaRecebida(TarefaRecebidaCallback callback);

private:
    String bufferSerial;
    TempoRecebidoCallback tempoCallback;
    TarefaRecebidaCallback tarefaCallback;
    static bool horaSincronizadaFlag;
};
