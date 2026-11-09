#pragma once

#include <vector>
#include <string>
#include <TFT_eSPI.h>

#ifndef BtnEsquerdo
#define BtnEsquerdo 16
#endif
#ifndef BtnDireito
#define BtnDireito 21
#endif
#ifndef BtnConfirmar
#define BtnConfirmar 17
#endif

enum class AcaoMenu {
    Nenhuma = -1,
    Concluir = 0,
    Retornar = 1,
    Pausar = 2,
    Adiar = 3,
    Materiais = 4,
    Notificacoes = 5
};

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

    AcaoMenu VerificarBotaoApertado(TFT_eSPI &tft);
    void ProximoBotao();
    void BotaoAnterior();
    void DesenharContorno(TFT_eSPI &tft);
    void resetarSelecao();
    int getIndiceBotaoAtual() const;

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
