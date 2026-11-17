#include "CarregadorDeTela.h"
#include "LeitorComunicacao.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Logo.h>
#include <Cores.h>
#include "WiFi_Com.h"
#include "WiFi_Sem.h"
#include <Segoe_UI20pt7b.h>
#include <Segoe_UI12pt7b.h>
#include <Segoe_UI8pt7b.h>
#include "Botoes.h"

void CarregadorDeTela::CarregarSplashLogo(TFT_eSPI &tft){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(95,160-LOGO_HEIGHT, LOGO_WIDTH, LOGO_HEIGHT, image_data_Logo, TFT_MAGENTA);
    tft.setFreeFont(&Segoe_UI20pt7b);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("PlanIT",110,180);
}

void CarregadorDeTela::CarregarMenuSemTarefas(TFT_eSPI &tft){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(0,0,WIFI_WIDTH,WIFI_HEIGHT, image_data_WiFi_Com, TFT_MAGENTA);

    if (LeitorComunicacao::HoraSincronizada()) {
        tft.setTextColor(TFT_BLACK, BACKGROUND_SPLASH_SCREEN);
        tft.setFreeFont(&Segoe_UI8pt7b);
        tft.drawString(LeitorComunicacao::ObterHoraFormatada("%H:%M"), 135, 4);
    }

    tft.fillRoundRect(9,27, SCREEN_WIDTH-27,126,5,BACKGROUND_MENU);

    tft.setViewport(15,29, SCREEN_WIDTH-27,126);
    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI20pt7b);
    tft.setCursor(0,50);
    tft.println("Sem tarefas por enquanto");
    tft.resetViewport();
}

void CarregadorDeTela::CarregarMenuSemInternet(TFT_eSPI &tft){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(0,0,WIFI_WIDTH,WIFI_HEIGHT, image_data_WiFi_Sem, TFT_MAGENTA);

    tft.fillRoundRect(9,27, SCREEN_WIDTH-27,126,5,BACKGROUND_MENU);

    tft.setViewport(15,29, SCREEN_WIDTH-27,126);
    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI20pt7b);
    tft.setCursor(0,50);
    tft.println("SEM CONEXAO A INTERNET");
    tft.resetViewport();
}

void CarregadorDeTela::CarregarMenuComTarefas(TFT_eSPI &tft, const Tarefa* tarefa){
    tft.fillScreen(TFT_DARKGREY);

    tft.pushImage(0,0,WIFI_WIDTH,WIFI_HEIGHT,image_data_WiFi_Com, TFT_MAGENTA);

    // Se a hora estiver sincronizada, exibe no topo
    if (LeitorComunicacao::HoraSincronizada()) {
        tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
        tft.setFreeFont(&Segoe_UI8pt7b);
        tft.drawString(LeitorComunicacao::ObterHoraFormatada("%H:%M"), 135, 4);
    }

    tft.fillRoundRect(14, 23, 228, 116, 4, 0xFF92);
    tft.drawRoundRect(14, 23, 228, 116, 4, 0xFF92);

    // Botões principais de ação
    tft.pushImage(8, 157, 70, 70, BtnConcluir);
    tft.drawRect(7,156,72,72, TFT_WHITE);
    tft.drawRect(6,155,74,74, TFT_WHITE);
    tft.drawRect(5,154,76,76, TFT_WHITE);

    tft.pushImage(88, 157, 70, 70, BtnRetornar);
    tft.pushImage(167, 157, 70, 70, BtnPausar);
    tft.pushImage(246, 157, 70, 70, BtnAdiar);

    // Ícone de Material (Com ou Sem material baseado na tarefa)
    bool temMateriais = (tarefa != nullptr && !tarefa->GetMateriais().empty());
    if (temMateriais) {
        tft.pushImage(256, 89, 50, 50, ComMaterial);
    } else {
        tft.pushImage(256, 89, 50, 50, SemMaterial);
    }

    // Ícone de Notificação
    tft.pushImage(256, 23, 50, 50, SemNotificacao);

    // Exibição da descrição da tarefa
    tft.setViewport(20, 30, 222, 100);
    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI8pt7b);
    tft.setCursor(15, 15);

    if (tarefa != nullptr) {
        tft.println(tarefa->GetDescricao().c_str());
    } else {
        tft.println("Sem tarefa ativa.");
    }
    tft.resetViewport();
}

void CarregadorDeTela::CarregarMenuMateriais(TFT_eSPI &tft, const Tarefa* tarefa){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(0, 0, WIFI_WIDTH, WIFI_HEIGHT, image_data_WiFi_Com, TFT_MAGENTA);
    tft.pushImage(SCREEN_WIDTH - 55, 5, 50, 50, ComMaterial);

    tft.fillRoundRect(10, 25, SCREEN_WIDTH - 20, 160, 6, BACKGROUND_MENU);
    tft.drawRoundRect(10, 25, SCREEN_WIDTH - 20, 160, 6, TFT_NAVY);

    tft.setTextColor(TFT_NAVY);
    tft.setFreeFont(&Segoe_UI12pt7b);
    tft.setCursor(20, 55);
    tft.print("Materiais");

    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI8pt7b);

    if (tarefa != nullptr && !tarefa->GetMateriais().empty()) {
        const auto& mats = tarefa->GetMateriais();
        int yOffset = 85;
        for (size_t i = 0; i < mats.size() && i < 4; i++) {
            tft.setCursor(20, yOffset);
            String item = "- " + String(mats[i].quantidade) + "x " + mats[i].nome.c_str();
            tft.println(item);
            yOffset += 24;
        }
    } else {
        tft.setCursor(20, 85);
        tft.println("- Nenhum material necessario.");
    }

    tft.setTextColor(TFT_DARKGREY);
    tft.setFreeFont(&Segoe_UI8pt7b);
    tft.drawString("[ Confirmar ] para Voltar", 70, 205);
}

void CarregadorDeTela::CarregarMenuNotificacoes(TFT_eSPI &tft){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(0, 0, WIFI_WIDTH, WIFI_HEIGHT, image_data_WiFi_Com, TFT_MAGENTA);
    tft.pushImage(SCREEN_WIDTH - 55, 5, 50, 50, ComNotificacao);

    tft.fillRoundRect(10, 25, SCREEN_WIDTH - 20, 160, 6, BACKGROUND_MENU);
    tft.drawRoundRect(10, 25, SCREEN_WIDTH - 20, 160, 6, TFT_DARKCYAN);

    tft.setTextColor(TFT_DARKCYAN);
    tft.setFreeFont(&Segoe_UI12pt7b);
    tft.setCursor(20, 55);
    tft.print("Notificacoes");

    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI8pt7b);
    tft.setCursor(20, 85);
    tft.println("- Nenhuma notificacao pendente.");
    tft.setCursor(20, 110);
    tft.println("- Sistema operacional atualizado.");

    tft.setTextColor(TFT_DARKGREY);
    tft.setFreeFont(&Segoe_UI8pt7b);
    tft.drawString("[ Confirmar ] para Voltar", 70, 205);
}

void CarregadorDeTela::CarregarMenuNovaTarefa(TFT_eSPI &tft, const Tarefa* tarefa){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(0, 0, WIFI_WIDTH, WIFI_HEIGHT, image_data_WiFi_Com, TFT_MAGENTA);

    tft.fillRoundRect(10, 25, SCREEN_WIDTH - 20, 160, 6, BACKGROUND_MENU);
    tft.drawRoundRect(10, 25, SCREEN_WIDTH - 20, 160, 6, TFT_DARKGREEN);

    tft.setTextColor(TFT_DARKGREEN);
    tft.setFreeFont(&Segoe_UI12pt7b);
    tft.setCursor(20, 55);
    tft.print("Nova Tarefa Recebida!");

    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI8pt7b);

    if (tarefa != nullptr) {
        tft.setCursor(20, 85);
        tft.print("ID: ");
        tft.println(tarefa->GetId());
        tft.setCursor(20, 110);
        tft.println(tarefa->GetDescricao().c_str());
    } else {
        tft.setCursor(20, 95);
        tft.println("Uma nova tarefa foi atribuida.");
    }

    tft.setTextColor(TFT_DARKGREY);
    tft.setFreeFont(&Segoe_UI8pt7b);
    tft.drawString("[ Confirmar ] para Abrir", 75, 205);
}
