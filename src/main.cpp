#include <Arduino.h>
#include <TFT_eSPI.h>

#include "CarregadorDeTela.h"
#include "MenuController.h"
#include "TelaController.h"
#include "GerenciadorTarefas.h"
#include "LeitorComunicacao.h"

TFT_eSPI tft;
CarregadorDeTela Tela;
MenuController Menu;
TelaController telaController;

const int ID = 123456789; // idPlanit do dispositivo

GerenciadorTarefas gerenciador(ID);
LeitorComunicacao leitor;

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

  // Tarefa inicial para teste e demonstração
  Tarefa tarefaInicial(ID, "Fabricar uma engrenagem", TarefaStatus::Recebido);
  tarefaInicial.AdicionarMaterial("macarico", 1);
  tarefaInicial.AdicionarMaterial("cobre em barra", 1);
  tarefaInicial.AdicionarMaterial("maquita", 1);
  gerenciador.AdicionarTarefa(tarefaInicial, false);

  // Callback ao receber tempo do back-end (Tipo 0)
  leitor.OnTempoRecebido([](time_t tempo) {
    Serial.print(F("[App] Hora sincronizada com sucesso: "));
    Serial.println(LeitorComunicacao::ObterDataHoraFormatada());
    telaController.forcarRedesenho();
  });

  // Callback ao receber nova tarefa do back-end (Tipo 1)
  leitor.OnTarefaRecebida([](const Tarefa& tarefa) {
    Serial.print(F("[App] Nova tarefa recebida do servidor: "));
    Serial.println(tarefa.GetDescricao().c_str());
    telaController.setTemTarefas(true);
    telaController.notificarNovaTarefa();
  });

  // Callback quando o gerenciador envia uma mensagem de atualização
  gerenciador.OnEnviarMensagem([](const String& jsonMsg, TarefaStatus status, int idTarefa) {
    Serial.print(F("[App -> BackEnd] "));
    Serial.println(jsonMsg);
  });

  // Inicializa a State Machine para gerenciar as telas
  telaController.inicializar(tft, Tela, Menu, &gerenciador);

  // Status de rede inicial
  telaController.setConectado(true);
  telaController.setTemTarefas(gerenciador.TemTarefas());

  // Callback de transições de tela
  telaController.onMudancaEstado([](TelaEstado antigo, TelaEstado novo) {
    Serial.printf("[App] Tela alterada: %s -> %s\n", 
                  TelaController::getEstadoNome(antigo), 
                  TelaController::getEstadoNome(novo));
  });

  // Inicia o fluxo na Splash Screen
  telaController.mudarEstado(TelaEstado::Splash_Screen);
}

void loop() {
  // Processa mensagens JSON recebidas via Serial (Back-end)
  leitor.ProcessarStream(Serial, gerenciador, ID);

  // Atualiza a State Machine das Telas e botões
  telaController.atualizar();
}
