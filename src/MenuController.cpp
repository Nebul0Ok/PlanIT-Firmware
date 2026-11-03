#include "MenuController.h"
#include <Arduino.h>

MenuController::MenuController(){
    this->Botoes.push_back(Botao{8,157,70,70,true, "Concluir"});
    this->Botoes.push_back(Botao{88,157,70,70,true, "Retornar"});
    this->Botoes.push_back(Botao{167,157,70,70,true, "Pausar"});
    this->Botoes.push_back(Botao{246,157,70,70, true, "Adiar"});
    this->Botoes.push_back(Botao{256, 89, 50, 50, false, "Materiais"});
    this->Botoes.push_back(Botao{256, 23, 50, 50, false, "Notificacoes"});

    this->IndiceBotaoAtual = 0;
}

void MenuController::ProximoBotao(){
    IndiceBotaoAtual = (IndiceBotaoAtual + 1)%6;
}

void MenuController::BotaoAnterior(){
    
}

void MenuController::VerificarBotaoApertado(){
    if(digitalRead(BtnEsquerdo) == LOW){

    }
    else if(digitalRead(BtnEsquerdo) == LOW){

    }

    else if(digitalRead(BtnConfirmar) == LOW){
        
    }
}