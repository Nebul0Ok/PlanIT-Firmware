#pragma once

#include <vector>
#include <string>

#define BtnEsquerdo 21
#define BtnDireito 16
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

    void VerificarBotaoApertado();
    void ProximoBotao();
    void BotaoAnterior();

    private:
    Botao Concluir;
    Botao Retornar;
    Botao Pausar;
    Botao Adiar;
    Botao Materiais;
    Botao Notificacao;
    std::vector<Botao> Botoes;
    int IndiceBotaoAtual;
};