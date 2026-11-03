#include "MenuController.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

MenuController::MenuController(){
    this->Botoes.push_back(Botao{8,157,70,70,true, "Concluir"});
    this->Botoes.push_back(Botao{88,157,70,70,true, "Retornar"});
    this->Botoes.push_back(Botao{167,157,70,70,true, "Pausar"});
    this->Botoes.push_back(Botao{246,157,70,70, true, "Adiar"});
    this->Botoes.push_back(Botao{256, 89, 50, 50, false, "Materiais"});
    this->Botoes.push_back(Botao{256, 23, 50, 50, false, "Notificacoes"});

    this->IndiceBotaoAtual = 0;
    this->ProxBotao = 1;
    this->AntBotao = 5;
}

void MenuController::ProximoBotao(){
    IndiceBotaoAtual = (IndiceBotaoAtual + 1)%6;
    ProxBotao = (ProxBotao + 1)%6;
    AntBotao = (AntBotao + 1)%6;
}

void MenuController::BotaoAnterior(){
    IndiceBotaoAtual = (IndiceBotaoAtual - 1 + 6) % 6;
    ProxBotao = (ProxBotao - 1 + 6) % 6;
    AntBotao = (AntBotao - 1 + 6) % 6;
}

void MenuController::DesenharContorno(TFT_eSPI &tft){
    tft.drawRect(Botoes[AntBotao].x-1,Botoes[AntBotao].y-1,Botoes[AntBotao].w+2,Botoes[AntBotao].h+2, TFT_DARKGREY);
    tft.drawRect(Botoes[AntBotao].x-2,Botoes[AntBotao].y-2,Botoes[AntBotao].w+4,Botoes[AntBotao].h+4, TFT_DARKGREY);
    tft.drawRect(Botoes[AntBotao].x-3,Botoes[AntBotao].y-3,Botoes[AntBotao].w+6,Botoes[AntBotao].h+6, TFT_DARKGREY);

    tft.drawRect(Botoes[ProxBotao].x-1,Botoes[ProxBotao].y-1,Botoes[ProxBotao].w+2,Botoes[ProxBotao].h+2, TFT_DARKGREY);
    tft.drawRect(Botoes[ProxBotao].x-2,Botoes[ProxBotao].y-2,Botoes[ProxBotao].w+4,Botoes[ProxBotao].h+4, TFT_DARKGREY);
    tft.drawRect(Botoes[ProxBotao].x-3,Botoes[ProxBotao].y-3,Botoes[ProxBotao].w+6,Botoes[ProxBotao].h+6, TFT_DARKGREY);

    tft.drawRect(Botoes[IndiceBotaoAtual].x-1,Botoes[IndiceBotaoAtual].y-1,Botoes[IndiceBotaoAtual].w+2,Botoes[ProxBotao].h+2, TFT_WHITE);
    tft.drawRect(Botoes[IndiceBotaoAtual].x-2,Botoes[IndiceBotaoAtual].y-2,Botoes[IndiceBotaoAtual].w+4,Botoes[ProxBotao].h+4, TFT_WHITE);
    tft.drawRect(Botoes[IndiceBotaoAtual].x-3,Botoes[IndiceBotaoAtual].y-3,Botoes[IndiceBotaoAtual].w+6,Botoes[ProxBotao].h+6, TFT_WHITE);

}

void MenuController::VerificarBotaoApertado(TFT_eSPI &tft){
    if(digitalRead(BtnEsquerdo) == LOW){
        BotaoAnterior();
        DesenharContorno(tft);
    }
    else if(digitalRead(BtnEsquerdo) == LOW){
        ProximoBotao();
        DesenharContorno(tft);
    }
    else if(digitalRead(BtnConfirmar) == LOW){
        
    }

    delay(500);
}