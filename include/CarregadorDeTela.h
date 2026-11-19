#pragma once

#include <TFT_eSPI.h>
#include <Tarefa.h>

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

class CarregadorDeTela{
    public:
    void CarregarSplashLogo(TFT_eSPI &tft);
    void CarregarMenuSemTarefas(TFT_eSPI &tft, bool conectado = true);
    void CarregarMenuComTarefas(TFT_eSPI &tft, bool conectado = true);
    void CarregarMenuComTarefas(TFT_eSPI &tft, bool notificacao, bool conectado);
    void CarregarMenuComTarefas(TFT_eSPI &tft, Tarefa tarefa, bool notificacao, bool conectado = true);
    void CarregarMenuSemInternet(TFT_eSPI &tft);
    void CarregarMenuMateriais(TFT_eSPI &tft, bool conectado = true);
    void CarregarMenuNotificacoes(TFT_eSPI &tft, bool conectado = true);
    void CarregarMenuNovaTarefa(TFT_eSPI &tft, bool conectado = true);
};
