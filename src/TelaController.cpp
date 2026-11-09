#include "TelaController.h"

TelaController::TelaController()
    : tft(nullptr),
      carregador(nullptr),
      menu(nullptr),
      estadoAtual(TelaEstado::Splash_Screen),
      estadoAnterior(TelaEstado::Splash_Screen),
      conectado(true),
      temTarefas(true),
      inicializado(false),
      tempoEntradaEstado(0),
      splashDurationMs(3000),
      estadoCallback(nullptr),
      acaoCallback(nullptr) {}

TelaController::TelaController(TFT_eSPI &tftRef, CarregadorDeTela &carregadorRef, MenuController &menuRef)
    : TelaController() {
    inicializar(tftRef, carregadorRef, menuRef);
}

void TelaController::inicializar(TFT_eSPI &tftRef, CarregadorDeTela &carregadorRef, MenuController &menuRef) {
    this->tft = &tftRef;
    this->carregador = &carregadorRef;
    this->menu = &menuRef;
    this->inicializado = true;
    this->tempoEntradaEstado = millis();

    Serial.println(F("[TelaController] State Machine inicializada."));
    entrarEstado(estadoAtual);
}

const char* TelaController::getEstadoNome(TelaEstado estado) {
    switch (estado) {
        case TelaEstado::Splash_Screen: return "Splash_Screen";
        case TelaEstado::Sem_Tarefa:    return "Sem_Tarefa";
        case TelaEstado::Sem_Conexao:   return "Sem_Conexao";
        case TelaEstado::Menu:          return "Menu";
        case TelaEstado::Nova_Tarefa:   return "Nova_Tarefa";
        case TelaEstado::Material:      return "Material";
        case TelaEstado::Notificacao:   return "Notificacao";
        default:                        return "Desconhecido";
    }
}

void TelaController::mudarEstado(TelaEstado novoEstado) {
    if (!inicializado) {
        estadoAtual = novoEstado;
        return;
    }

    if (novoEstado == estadoAtual) {
        return;
    }

    Serial.print(F("[TelaController] Transição de tela: "));
    Serial.print(getEstadoNome(estadoAtual));
    Serial.print(F(" -> "));
    Serial.println(getEstadoNome(novoEstado));

    sairEstado(estadoAtual);

    estadoAnterior = estadoAtual;
    estadoAtual = novoEstado;
    tempoEntradaEstado = millis();

    entrarEstado(estadoAtual);

    if (estadoCallback) {
        estadoCallback(estadoAnterior, estadoAtual);
    }
}

void TelaController::voltarEstadoAnterior() {
    if (estadoAnterior == TelaEstado::Splash_Screen ||
        (estadoAnterior == TelaEstado::Sem_Conexao && conectado)) {
        mudarEstado(temTarefas ? TelaEstado::Menu : TelaEstado::Sem_Tarefa);
    } else {
        mudarEstado(estadoAnterior);
    }
}

void TelaController::forcarRedesenho() {
    if (inicializado) {
        renderizarEstado(estadoAtual);
    }
}

void TelaController::entrarEstado(TelaEstado estado) {
    renderizarEstado(estado);

    if (estado == TelaEstado::Menu && menu != nullptr) {
        menu->resetarSelecao();
    }
}

void TelaController::sairEstado(TelaEstado estado) {
    // Ações de encerramento do estado se aplicável
}

void TelaController::renderizarEstado(TelaEstado estado) {
    if (tft == nullptr || carregador == nullptr) return;

    switch (estado) {
        case TelaEstado::Splash_Screen:
            carregador->CarregarSplashLogo(*tft);
            break;
        case TelaEstado::Sem_Tarefa:
            carregador->CarregarMenuSemTarefas(*tft);
            break;
        case TelaEstado::Sem_Conexao:
            carregador->CarregarMenuSemInternet(*tft);
            break;
        case TelaEstado::Menu:
            carregador->CarregarMenuComTarefas(*tft);
            break;
        case TelaEstado::Nova_Tarefa:
            carregador->CarregarMenuNovaTarefa(*tft);
            break;
        case TelaEstado::Material:
            carregador->CarregarMenuMateriais(*tft);
            break;
        case TelaEstado::Notificacao:
            carregador->CarregarMenuNotificacoes(*tft);
            break;
    }
}

bool TelaController::botaoPressionado(uint8_t pino) {
    if (digitalRead(pino) == LOW) {
        delay(25); // Debounce
        if (digitalRead(pino) == LOW) {
            while (digitalRead(pino) == LOW) {
                delay(10);
            }
            return true;
        }
    }
    return false;
}

void TelaController::processarEntradaEstado() {
    switch (estadoAtual) {
        case TelaEstado::Splash_Screen: {
            bool tempoEsgotado = (millis() - tempoEntradaEstado >= splashDurationMs);
            bool pulouSplash = botaoPressionado(BtnConfirmar);

            if (tempoEsgotado || pulouSplash) {
                if (!conectado) {
                    mudarEstado(TelaEstado::Sem_Conexao);
                } else if (!temTarefas) {
                    mudarEstado(TelaEstado::Sem_Tarefa);
                } else {
                    mudarEstado(TelaEstado::Menu);
                }
            }
            break;
        }

        case TelaEstado::Sem_Conexao: {
            if (botaoPressionado(BtnConfirmar)) {
                Serial.println(F("[TelaController] Botão confirmar: tentando reconectar..."));
                if (conectado) {
                    mudarEstado(temTarefas ? TelaEstado::Menu : TelaEstado::Sem_Tarefa);
                }
            }
            break;
        }

        case TelaEstado::Sem_Tarefa: {
            if (botaoPressionado(BtnConfirmar)) {
                Serial.println(F("[TelaController] Botão confirmar: verificando tarefas..."));
                if (temTarefas) {
                    mudarEstado(TelaEstado::Menu);
                }
            }
            break;
        }

        case TelaEstado::Menu: {
            if (menu != nullptr && tft != nullptr) {
                AcaoMenu acao = menu->VerificarBotaoApertado(*tft);
                if (acao != AcaoMenu::Nenhuma) {
                    if (acaoCallback) {
                        acaoCallback(acao);
                    }

                    switch (acao) {
                        case AcaoMenu::Materiais:
                            mudarEstado(TelaEstado::Material);
                            break;
                        case AcaoMenu::Notificacoes:
                            mudarEstado(TelaEstado::Notificacao);
                            break;
                        case AcaoMenu::Concluir:
                            Serial.println(F("[TelaController] Ação: Concluir tarefa"));
                            break;
                        case AcaoMenu::Retornar:
                            Serial.println(F("[TelaController] Ação: Retornar tarefa"));
                            break;
                        case AcaoMenu::Pausar:
                            Serial.println(F("[TelaController] Ação: Pausar tarefa"));
                            break;
                        case AcaoMenu::Adiar:
                            Serial.println(F("[TelaController] Ação: Adiar tarefa"));
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
        }

        case TelaEstado::Material: {
            if (botaoPressionado(BtnConfirmar) || botaoPressionado(BtnEsquerdo)) {
                voltarEstadoAnterior();
            }
            break;
        }

        case TelaEstado::Notificacao: {
            if (botaoPressionado(BtnConfirmar) || botaoPressionado(BtnEsquerdo)) {
                voltarEstadoAnterior();
            }
            break;
        }

        case TelaEstado::Nova_Tarefa: {
            if (botaoPressionado(BtnConfirmar)) {
                mudarEstado(TelaEstado::Menu);
            } else if (botaoPressionado(BtnEsquerdo)) {
                voltarEstadoAnterior();
            }
            break;
        }
    }
}

void TelaController::atualizar() {
    if (!inicializado) return;
    processarEntradaEstado();
}

void TelaController::setConectado(bool valor) {
    if (conectado == valor) return;
    conectado = valor;

    if (!conectado) {
        if (estadoAtual != TelaEstado::Splash_Screen) {
            mudarEstado(TelaEstado::Sem_Conexao);
        }
    } else {
        if (estadoAtual == TelaEstado::Sem_Conexao) {
            mudarEstado(temTarefas ? TelaEstado::Menu : TelaEstado::Sem_Tarefa);
        }
    }
}

void TelaController::setTemTarefas(bool valor) {
    temTarefas = valor;
    if (conectado) {
        if (!temTarefas && estadoAtual == TelaEstado::Menu) {
            mudarEstado(TelaEstado::Sem_Tarefa);
        } else if (temTarefas && estadoAtual == TelaEstado::Sem_Tarefa) {
            mudarEstado(TelaEstado::Menu);
        }
    }
}

void TelaController::notificarNovaTarefa() {
    temTarefas = true;
    mudarEstado(TelaEstado::Nova_Tarefa);
}

void TelaController::onMudancaEstado(EstadoChangeCallback callback) {
    estadoCallback = callback;
}

void TelaController::onAcaoMenu(MenuActionCallback callback) {
    acaoCallback = callback;
}
