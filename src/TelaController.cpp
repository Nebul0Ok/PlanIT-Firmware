#include "TelaController.h"

TelaController::TelaController()
    : tft(nullptr),
      carregador(nullptr),
      menu(nullptr),
      gerenciador(nullptr),
      estadoAtual(TelaEstado::Splash_Screen),
      estadoAnterior(TelaEstado::Splash_Screen),
      conectado(true),
      temTarefas(true),
      temNotificacoes(false),
      inicializado(false),
      tempoEntradaEstado(0),
      splashDurationMs(3000),
      estadoCallback(nullptr),
      acaoCallback(nullptr) {}

TelaController::TelaController(TFT_eSPI &tftRef, CarregadorDeTela &carregadorRef, MenuController &menuRef, GerenciadorTarefa* gerenciadorRef)
    : TelaController() {
    inicializar(tftRef, carregadorRef, menuRef, gerenciadorRef);
}

void TelaController::inicializar(TFT_eSPI &tftRef, CarregadorDeTela &carregadorRef, MenuController &menuRef, GerenciadorTarefa* gerenciadorRef) {
    this->tft = &tftRef;
    this->carregador = &carregadorRef;
    this->menu = &menuRef;
    this->gerenciador = gerenciadorRef;
    if (gerenciadorRef != nullptr) {
        this->temTarefas = (gerenciadorRef->GetQuantidade() > 0);
    }
    atualizarBotoesMenu();
    this->inicializado = true;
    this->tempoEntradaEstado = millis();

    Serial.println(F("[TelaController] State Machine inicializada."));
    entrarEstado(estadoAtual);
}

void TelaController::setGerenciador(GerenciadorTarefa* gerenciadorRef) {
    this->gerenciador = gerenciadorRef;
    if (gerenciadorRef != nullptr) {
        setTemTarefas(gerenciadorRef->GetQuantidade() > 0);
    }
    atualizarBotoesMenu();
}

TelaEstado TelaController::determinarEstadoPadrao() const {
    if (temTarefas) {
        return TelaEstado::Menu;
    } else {
        if (conectado) {
            return TelaEstado::Sem_Tarefa;
        } else {
            return TelaEstado::Sem_Conexao;
        }
    }
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
    if (estadoAnterior == TelaEstado::Splash_Screen) {
        mudarEstado(determinarEstadoPadrao());
    } else {
        mudarEstado(estadoAnterior);
    }
}

void TelaController::forcarRedesenho() {
    if (inicializado) {
        renderizarEstado(estadoAtual);
        if (estadoAtual == TelaEstado::Menu && menu != nullptr && tft != nullptr) {
            menu->desenharContornoCompleto(*tft);
        }
    }
}

void TelaController::atualizarBotoesMenu() {
    if (menu == nullptr) return;

    bool temMateriais = false;
    if (gerenciador != nullptr && gerenciador->GetQuantidade() > 0 && gerenciador->GetTarefaAtual() != nullptr) {
        temMateriais = gerenciador->GetTarefaAtual()->TemMateriais();
    }

    menu->setBotaoAtivo(AcaoMenu::Materiais, temMateriais);
    menu->setBotaoAtivo(AcaoMenu::Notificacoes, temNotificacoes);
}

void TelaController::entrarEstado(TelaEstado estado) {
    if (estado == TelaEstado::Menu) {
        atualizarBotoesMenu();
        if (menu != nullptr) {
            menu->resetarSelecao();
        }
    }

    renderizarEstado(estado);
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
            carregador->CarregarMenuSemTarefas(*tft, conectado);
            break;
        case TelaEstado::Sem_Conexao:
            carregador->CarregarMenuSemInternet(*tft);
            break;
        case TelaEstado::Menu:
            if (gerenciador != nullptr && gerenciador->GetQuantidade() > 0 && gerenciador->GetTarefaAtual() != nullptr) {
                carregador->CarregarMenuComTarefas(*tft, *(gerenciador->GetTarefaAtual()), temNotificacoes, conectado);
            } else {
                carregador->CarregarMenuComTarefas(*tft, temNotificacoes, conectado);
            }
            break;
        case TelaEstado::Nova_Tarefa:
            carregador->CarregarMenuNovaTarefa(*tft, conectado);
            break;
        case TelaEstado::Material:
            carregador->CarregarMenuMateriais(*tft, conectado);
            break;
        case TelaEstado::Notificacao:
            carregador->CarregarMenuNotificacoes(*tft, conectado);
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
                mudarEstado(determinarEstadoPadrao());
            }
            break;
        }

        case TelaEstado::Sem_Conexao: {
            if (botaoPressionado(BtnConfirmar)) {
                Serial.println(F("[TelaController] Botão confirmar: verificando conexão/tarefas..."));
                if (conectado || temTarefas) {
                    mudarEstado(determinarEstadoPadrao());
                }
            }
            break;
        }

        case TelaEstado::Sem_Tarefa: {
            if (botaoPressionado(BtnConfirmar)) {
                Serial.println(F("[TelaController] Botão confirmar: verificando tarefas/conexão..."));
                if (temTarefas || !conectado) {
                    mudarEstado(determinarEstadoPadrao());
                }
            }
            break;
        }

        case TelaEstado::Menu: {
            if (menu != nullptr && tft != nullptr) {
                atualizarBotoesMenu();
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

    if (estadoAtual == TelaEstado::Splash_Screen) return;

    if (!conectado) {
        if (!temTarefas) {
            mudarEstado(TelaEstado::Sem_Conexao);
        } else {
            forcarRedesenho();
        }
    } else {
        if (estadoAtual == TelaEstado::Sem_Conexao) {
            mudarEstado(determinarEstadoPadrao());
        } else {
            forcarRedesenho();
        }
    }
}

void TelaController::setTemTarefas(bool valor) {
    if (temTarefas == valor) return;
    temTarefas = valor;

    if (estadoAtual == TelaEstado::Splash_Screen) return;

    if (temTarefas) {
        if (estadoAtual == TelaEstado::Sem_Tarefa || estadoAtual == TelaEstado::Sem_Conexao) {
            mudarEstado(TelaEstado::Menu);
        }
    } else {
        if (estadoAtual == TelaEstado::Menu) {
            mudarEstado(conectado ? TelaEstado::Sem_Tarefa : TelaEstado::Sem_Conexao);
        }
    }
}

void TelaController::setTemNotificacoes(bool valor) {
    if (temNotificacoes == valor) return;
    temNotificacoes = valor;

    atualizarBotoesMenu();

    if (estadoAtual == TelaEstado::Menu) {
        forcarRedesenho();
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
