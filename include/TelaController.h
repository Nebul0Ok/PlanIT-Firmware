#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "CarregadorDeTela.h"
#include "MenuController.h"
#include "GerenciadorTarefa.h"

#ifndef BtnEsquerdo
#define BtnEsquerdo 16
#endif
#ifndef BtnDireito
#define BtnDireito 21
#endif
#ifndef BtnConfirmar
#define BtnConfirmar 17
#endif

enum class TelaEstado{
    Splash_Screen,
    Sem_Tarefa,
    Sem_Conexao,
    Menu,
    Nova_Tarefa,
    Material,
    Notificacao
};

// Tipos de Callbacks opcionais
typedef void (*EstadoChangeCallback)(TelaEstado antigo, TelaEstado novo);
typedef void (*MenuActionCallback)(AcaoMenu acao);

class TelaController{
    public:
    TelaController();
    TelaController(TFT_eSPI &tft, CarregadorDeTela &carregador, MenuController &menu, GerenciadorTarefa* gerenciador = nullptr);

    // Inicialização da State Machine
    void inicializar(TFT_eSPI &tft, CarregadorDeTela &carregador, MenuController &menu, GerenciadorTarefa* gerenciador = nullptr);

    // Ciclo principal da State Machine (chamar no loop do ESP32)
    void atualizar();

    // Transição de Estados
    void mudarEstado(TelaEstado novoEstado);
    void voltarEstadoAnterior();
    void forcarRedesenho();

    // Determina o estado padrão com base no status de tarefas e de rede
    TelaEstado determinarEstadoPadrao() const;

    // Consultas de Estado
    TelaEstado getEstadoAtual() const { return estadoAtual; }
    TelaEstado getEstadoAnterior() const { return estadoAnterior; }
    uint32_t getTempoNoEstado() const { return millis() - tempoEntradaEstado; }
    static const char* getEstadoNome(TelaEstado estado);

    // Gestão de Conexão e Tarefas (Eventos/Gatilhos)
    void setConectado(bool conectado);
    bool isConectado() const { return conectado; }

    void setTemTarefas(bool temTarefas);
    bool hasTarefas() const { return temTarefas; }

    void setGerenciador(GerenciadorTarefa* gerenciador);
    GerenciadorTarefa* getGerenciador() const { return gerenciador; }

    void setTemNotificacoes(bool temNotificacoes);
    bool hasNotificacoes() const { return temNotificacoes; }

    void notificarNovaTarefa();

    // Callbacks
    void onMudancaEstado(EstadoChangeCallback callback);
    void onAcaoMenu(MenuActionCallback callback);

    // Configurações
    void setSplashDuration(uint32_t duracaoMs) { splashDurationMs = duracaoMs; }

    private:
    TFT_eSPI* tft;
    CarregadorDeTela* carregador;
    MenuController* menu;
    GerenciadorTarefa* gerenciador;

    TelaEstado estadoAtual;
    TelaEstado estadoAnterior;

    bool conectado;
    bool temTarefas;
    bool temNotificacoes;
    bool inicializado;

    uint32_t tempoEntradaEstado;
    uint32_t splashDurationMs;

    EstadoChangeCallback estadoCallback;
    MenuActionCallback acaoCallback;

    // Métodos internos do ciclo de vida da FSM
    void entrarEstado(TelaEstado estado);
    void sairEstado(TelaEstado estado);
    void processarEntradaEstado();
    void renderizarEstado(TelaEstado estado);
    void atualizarBotoesMenu();

    bool botaoPressionado(uint8_t pino);
};
