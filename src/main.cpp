#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Logo.h>
#include <Cores.h>
#include <Segoe_UI20pt7b.h>

#include "CarregadorDeTela.h"



TFT_eSPI tft;
CarregadorDeTela Tela;


void setup() {
  Serial.begin(115200);
  Serial.println("--== ESP32 INICIADO ==--");

  tft.init();
  
  //Configurações iniciais
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.setTextWrap(true, true);

  // Tela.CarregarSplashLogo(tft);
  // delay(3000);

  // bool wifiConnection = true;

  // if(wifiConnection){
  //   Tela.CarregarMenuSemTarefas(tft);
  // }
  // else{
  //   Tela.CarregarMenuSemInternet(tft);
  // }

  Tela.CarregarMenuComTarefas(tft);

  Serial.println("Configurando Botão Esquerdo");
  pinMode(BtnEsquerdo, INPUT_PULLUP);

}

void loop() {
  if(digitalRead(BtnEsquerdo) == LOW){
    Serial.println("Botão esquerdo foi apertado");
    tft.drawRect(7,156,72,72, TFT_DARKGREY);
    tft.drawRect(6,155,74,74, TFT_DARKGREY);
    tft.drawRect(5,154,76,76, TFT_DARKGREY);

    tft.drawRect(87,156,72,72, TFT_WHITE);
    tft.drawRect(86,155,74,74, TFT_WHITE);
    tft.drawRect(85,154,76,76, TFT_WHITE);
  }
}
