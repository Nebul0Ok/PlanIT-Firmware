#include <Arduino.h>
#include <TFT_eSPI.h>

#include "CarregadorDeTela.h"
#include "MenuController.h"
#include "TelaController.h"
#include "GerenciadorTarefa.h"

// =============================================================================
// SIMULAÇÃO TEMPORÁRIA (FASE INICIAL DE DESENVOLVIMENTO)
// Altere para true ou false para o sistema pensar que tem WiFi ou Notificações:
//   SIMULAR_WIFI_CONECTADO: true -> com sinal de rede / false -> sem sinal
//   SIMULAR_TEM_NOTIFICACOES: true -> com notificações / false -> sem notificações
// =============================================================================
#ifndef SIMULAR_WIFI_CONECTADO
#define SIMULAR_WIFI_CONECTADO false
#endif

#ifndef SIMULAR_TEM_NOTIFICACOES
#define SIMULAR_TEM_NOTIFICACOES false
#endif

TFT_eSPI tft;
CarregadorDeTela Tela;
MenuController Menu;
TelaController telaController;
GerenciadorTarefa gerenciadorTarefa;

const int ID = 123456789;

void setup() {
  Serial.begin(115200);
  Serial.println("--== ESP32 INICIADO ==--");

  // Configuração dos botões físicos
  pinMode(BtnEsquerdo, INPUT_PULLUP);
  pinMode(BtnDireito, INPUT_PULLUP);
  pinMode(BtnConfirmar, INPUT_PULLUP);

  // Tarefa de teste (comente esta linha para simular o sistema SEM tarefas):
  gerenciadorTarefa.AdicionarTarefa(1, "Teste", TarefaStatus::Recebido);

  // Configurações iniciais do display TFT
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.setTextWrap(true, true);

  // Inicializa a State Machine para gerenciar as telas
  telaController.inicializar(tft, Tela, Menu, &gerenciadorTarefa);

  // Aplica o status de rede (simulado via #define), tarefas e notificações
  telaController.setConectado(SIMULAR_WIFI_CONECTADO);
  telaController.setTemNotificacoes(SIMULAR_TEM_NOTIFICACOES);
  telaController.setTemTarefas(gerenciadorTarefa.GetQuantidade() > 0);

  // Callback opcional de ações disparadas pelos botões do menu
  telaController.onAcaoMenu([](AcaoMenu acao) {
    switch (acao) {
      case AcaoMenu::Concluir:
        Serial.println("[App] Tarefa marcada como Concluída!");
        break;
      case AcaoMenu::Retornar:
        Serial.println("[App] Tarefa Retornada!");
        break;
      case AcaoMenu::Pausar:
        Serial.println("[App] Tarefa Pausada!");
        break;
      case AcaoMenu::Adiar:
        Serial.println("[App] Tarefa Adiada!");
        break;
      case AcaoMenu::Materiais:
        Serial.println("[App] Acessando Materiais!");
        break;
      case AcaoMenu::Notificacoes:
        Serial.println("[App] Acessando Notificações!");
        break;
      default:
        break;
    }
  });

  // Callback para monitorar transições de tela
  telaController.onMudancaEstado([](TelaEstado antigo, TelaEstado novo) {
    Serial.printf("[App] Tela alterada: %s -> %s\n", 
                  TelaController::getEstadoNome(antigo), 
                  TelaController::getEstadoNome(novo));
  });

  // Inicia o fluxo na Splash Screen (vai automaticamente para a próxima tela após 3s)
  telaController.mudarEstado(TelaEstado::Splash_Screen);
}

void loop() {
  // A State Machine gerencia o ciclo das telas, temporizadores e navegação
  telaController.atualizar();
}
