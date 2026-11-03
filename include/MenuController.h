#pragma once

#include <vector>
#include <string>
#include <TFT_eSPI.h>

#define BtnEsquerdo 16
#define BtnDireito 21
#define BtnConfirmar 17

struct Botao{
    int x;
    int y;
    int h;
    int w;
    bool ativo;
    std::string nome;
};

class MenuController{
    public:
    MenuController();

    void VerificarBotaoApertado(TFT_eSPI &tft);
    void ProximoBotao();
    void BotaoAnterior();
    void DesenharContorno(TFT_eSPI &tft);

    private:
    Botao Concluir;
    Botao Retornar;
    Botao Pausar;
    Botao Adiar;
    Botao Materiais;
    Botao Notificacao;
    std::vector<Botao> Botoes;
    int IndiceBotaoAtual;
    int ProxBotao;
    int AntBotao;
};