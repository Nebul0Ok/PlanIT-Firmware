#include "MenuController.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

MenuController::MenuController(){
    this->Botoes.push_back(Botao{8, 157, 70, 70, true, "Concluir"});
    this->Botoes.push_back(Botao{88, 157, 70, 70, true, "Retornar"});
    this->Botoes.push_back(Botao{167, 157, 70, 70, true, "Pausar"});
    this->Botoes.push_back(Botao{246, 157, 70, 70, true, "Adiar"});
    this->Botoes.push_back(Botao{256, 89, 50, 50, false, "Materiais"});
    this->Botoes.push_back(Botao{256, 23, 50, 50, false, "Notificacoes"});

    this->IndiceBotaoAtual = 0;
    this->IndiceBotaoAnterior = 0;
    this->ProxBotao = getProximoIndiceAtivo(0);
    this->AntBotao = getIndiceAnteriorAtivo(0);
}

int MenuController::getProximoIndiceAtivo(int indice) const {
    if (Botoes.empty()) return 0;
    int total = static_cast<int>(Botoes.size());
    for (int step = 1; step <= total; step++) {
        int proximo = (indice + step) % total;
        if (Botoes[proximo].ativo) {
            return proximo;
        }
    }
    return indice;
}

int MenuController::getIndiceAnteriorAtivo(int indice) const {
    if (Botoes.empty()) return 0;
    int total = static_cast<int>(Botoes.size());
    for (int step = 1; step <= total; step++) {
        int anterior = (indice - (step % total) + total) % total;
        if (Botoes[anterior].ativo) {
            return anterior;
        }
    }
    return indice;
}

void MenuController::ProximoBotao(){
    int proximo = getProximoIndiceAtivo(IndiceBotaoAtual);
    if (proximo != IndiceBotaoAtual) {
        IndiceBotaoAnterior = IndiceBotaoAtual;
        IndiceBotaoAtual = proximo;
        ProxBotao = getProximoIndiceAtivo(IndiceBotaoAtual);
        AntBotao = getIndiceAnteriorAtivo(IndiceBotaoAtual);
    }
}

void MenuController::BotaoAnterior(){
    int anterior = getIndiceAnteriorAtivo(IndiceBotaoAtual);
    if (anterior != IndiceBotaoAtual) {
        IndiceBotaoAnterior = IndiceBotaoAtual;
        IndiceBotaoAtual = anterior;
        ProxBotao = getProximoIndiceAtivo(IndiceBotaoAtual);
        AntBotao = getIndiceAnteriorAtivo(IndiceBotaoAtual);
    }
}

void MenuController::desenharContornoBotao(TFT_eSPI &tft, int indice, uint16_t cor){
    if (indice < 0 || indice >= static_cast<int>(Botoes.size())) return;
    tft.drawRect(Botoes[indice].x - 1, Botoes[indice].y - 1, Botoes[indice].w + 2, Botoes[indice].h + 2, cor);
    tft.drawRect(Botoes[indice].x - 2, Botoes[indice].y - 2, Botoes[indice].w + 4, Botoes[indice].h + 4, cor);
    tft.drawRect(Botoes[indice].x - 3, Botoes[indice].y - 3, Botoes[indice].w + 6, Botoes[indice].h + 6, cor);
}

void MenuController::DesenharContorno(TFT_eSPI &tft){
    if (IndiceBotaoAnterior >= 0 && IndiceBotaoAnterior < static_cast<int>(Botoes.size()) && IndiceBotaoAnterior != IndiceBotaoAtual) {
        desenharContornoBotao(tft, IndiceBotaoAnterior, TFT_DARKGREY);
    }
    if (IndiceBotaoAtual >= 0 && IndiceBotaoAtual < static_cast<int>(Botoes.size())) {
        desenharContornoBotao(tft, IndiceBotaoAtual, TFT_WHITE);
    }
    IndiceBotaoAnterior = IndiceBotaoAtual;
}

void MenuController::desenharContornoCompleto(TFT_eSPI &tft){
    for (size_t i = 0; i < Botoes.size(); i++) {
        if (static_cast<int>(i) == IndiceBotaoAtual) {
            desenharContornoBotao(tft, static_cast<int>(i), TFT_WHITE);
        } else {
            desenharContornoBotao(tft, static_cast<int>(i), TFT_DARKGREY);
        }
    }
    IndiceBotaoAnterior = IndiceBotaoAtual;
}

void MenuController::resetarSelecao(){
    int primeiroAtivo = 0;
    for (size_t i = 0; i < Botoes.size(); i++) {
        if (Botoes[i].ativo) {
            primeiroAtivo = static_cast<int>(i);
            break;
        }
    }
    this->IndiceBotaoAtual = primeiroAtivo;
    this->IndiceBotaoAnterior = primeiroAtivo;
    this->ProxBotao = getProximoIndiceAtivo(IndiceBotaoAtual);
    this->AntBotao = getIndiceAnteriorAtivo(IndiceBotaoAtual);
}

int MenuController::getIndiceBotaoAtual() const {
    return this->IndiceBotaoAtual;
}

void MenuController::setBotaoAtivo(int indice, bool ativo){
    if (indice < 0 || indice >= static_cast<int>(Botoes.size())) return;
    if (Botoes[indice].ativo == ativo) return;

    Botoes[indice].ativo = ativo;

    if (!ativo && IndiceBotaoAtual == indice) {
        IndiceBotaoAnterior = IndiceBotaoAtual;
        IndiceBotaoAtual = getProximoIndiceAtivo(indice);
        ProxBotao = getProximoIndiceAtivo(IndiceBotaoAtual);
        AntBotao = getIndiceAnteriorAtivo(IndiceBotaoAtual);
    } else {
        ProxBotao = getProximoIndiceAtivo(IndiceBotaoAtual);
        AntBotao = getIndiceAnteriorAtivo(IndiceBotaoAtual);
    }
}

void MenuController::setBotaoAtivo(AcaoMenu acao, bool ativo){
    setBotaoAtivo(static_cast<int>(acao), ativo);
}

void MenuController::setMateriaisAtivo(bool ativo){
    setBotaoAtivo(AcaoMenu::Materiais, ativo);
}

void MenuController::setNotificacoesAtivo(bool ativo){
    setBotaoAtivo(AcaoMenu::Notificacoes, ativo);
}

bool MenuController::isBotaoAtivo(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(Botoes.size())) return false;
    return Botoes[indice].ativo;
}

bool MenuController::isBotaoAtivo(AcaoMenu acao) const {
    return isBotaoAtivo(static_cast<int>(acao));
}

AcaoMenu MenuController::VerificarBotaoApertado(TFT_eSPI &tft){
    if (digitalRead(BtnEsquerdo) == LOW) {
        BotaoAnterior();
        DesenharContorno(tft);
        while (digitalRead(BtnEsquerdo) == LOW) { delay(10); }
        return AcaoMenu::Nenhuma;
    }
    else if (digitalRead(BtnDireito) == LOW) {
        ProximoBotao();
        DesenharContorno(tft);
        while (digitalRead(BtnDireito) == LOW) { delay(10); }
        return AcaoMenu::Nenhuma;
    }
    else if (digitalRead(BtnConfirmar) == LOW) {
        while (digitalRead(BtnConfirmar) == LOW) { delay(10); }
        if (IndiceBotaoAtual >= 0 && IndiceBotaoAtual < static_cast<int>(Botoes.size()) && Botoes[IndiceBotaoAtual].ativo) {
            return static_cast<AcaoMenu>(IndiceBotaoAtual);
        }
        return AcaoMenu::Nenhuma;
    }
    return AcaoMenu::Nenhuma;
}
