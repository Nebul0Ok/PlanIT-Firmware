#include "CarregadorDeTela.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Logo.h>
#include <Cores.h>
#include "WiFi_Com.h"
#include "WiFi_Sem.h"
#include <Segoe_UI20pt7b.h>
#include <Segoe_UI8pt7b.h>
#include "Botoes.h"


void CarregadorDeTela::CarregarSplashLogo(TFT_eSPI &tft){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    // tft.setSwapBytes(true);
    tft.pushImage(95,160-LOGO_HEIGHT, LOGO_WIDTH, LOGO_HEIGHT, image_data_Logo, TFT_MAGENTA);
    tft.setFreeFont(&Segoe_UI20pt7b);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("PlanIT",110,180);
}

void CarregadorDeTela::CarregarMenuSemTarefas(TFT_eSPI &tft){
    tft.fillScreen(BACKGROUND_SPLASH_SCREEN);
    tft.pushImage(0,0,WIFI_WIDTH,WIFI_HEIGHT, image_data_WiFi_Com, TFT_MAGENTA);

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

void CarregadorDeTela::CarregarMenuComTarefas(TFT_eSPI &tft){
  tft.fillScreen(TFT_DARKGREY);

  tft.pushImage(0,0,WIFI_WIDTH,WIFI_HEIGHT,image_data_WiFi_Com, TFT_MAGENTA);

  tft.fillRoundRect(14, 23, 228, 116, 4, 0xFF92);
  tft.drawRoundRect(14, 23, 228, 116, 4, 0xFF92);

  
  tft.pushImage(8, 157, 70, 70, BtnConcluir);
  tft.drawRect(7,156,72,72, TFT_WHITE);
  tft.drawRect(6,155,74,74, TFT_WHITE);
  tft.drawRect(5,154,76,76, TFT_WHITE);

  tft.pushImage(88, 157, 70, 70, BtnRetornar);

  tft.pushImage(167, 157, 70, 70, BtnPausar);

  tft.pushImage(246, 157, 70, 70, BtnAdiar);

  tft.pushImage(256, 89, 50, 50, ComMaterial);

  tft.pushImage(256, 89, 50, 50, SemMaterial);

  tft.pushImage(256, 23, 50, 50, SemNotificacao);

  tft.pushImage(256, 23, 50, 50, ComNotificacao);

    tft.setViewport(20,30, 222,100);

    tft.setTextColor(TFT_BLACK);
    tft.setFreeFont(&Segoe_UI8pt7b);
    tft.setCursor(15,15);
    tft.println("Teste de tarefas!");
    tft.resetViewport();

}