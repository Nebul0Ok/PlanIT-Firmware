#include <Arduino.h>
#include <TFT_eSPI.h>

#include "CarregadorDeTela.h"
#include "MenuController.h"
#include "TelaController.h"

TFT_eSPI tft;
CarregadorDeTela Tela;
MenuController Menu;
TelaController telaController;

void setup() {
  Serial.begin(115200);
  Serial.println("--== ESP32 INICIADO ==--");

  // Configuração dos botões físicos
  pinMode(BtnEsquerdo, INPUT_PULLUP);
  pinMode(BtnDireito, INPUT_PULLUP);
  pinMode(BtnConfirmar, INPUT_PULLUP);

  // Configurações iniciais do display TFT
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.setTextWrap(true, true);

  // Inicializa a State Machine para gerenciar as telas
  telaController.inicializar(tft, Tela, Menu);

  // Define status de rede e tarefas iniciais
  telaController.setConectado(true);
  telaController.setTemTarefas(true);

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
      default:
        break;
    }
  });

  // Callback opcional para monitorar transições de tela
  telaController.onMudancaEstado([](TelaEstado antigo, TelaEstado novo) {
    Serial.printf("[App] Tela alterada: %s -> %s\n", 
                  TelaController::getEstadoNome(antigo), 
                  TelaController::getEstadoNome(novo));
  });

  // Inicia o fluxo na Splash Screen (vai automaticamente para o Menu após 3s)
  telaController.mudarEstado(TelaEstado::Splash_Screen);
}

void loop() {
  // A State Machine gerencia o ciclo das telas, temporizadores e navegação
  telaController.atualizar();
}
