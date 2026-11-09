#pragma once

#include <TFT_eSPI.h>

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240

class CarregadorDeTela{
    public:
    void CarregarSplashLogo(TFT_eSPI &tft);
    void CarregarMenuSemTarefas(TFT_eSPI &tft);
    void CarregarMenuComTarefas(TFT_eSPI &tft);
    void CarregarMenuSemInternet(TFT_eSPI &tft);
    void CarregarMenuMateriais(TFT_eSPI &tft);
    void CarregarMenuNotificacoes(TFT_eSPI &tft);
    void CarregarMenuNovaTarefa(TFT_eSPI &tft);
};
