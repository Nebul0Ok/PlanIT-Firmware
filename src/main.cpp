#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Logo.h>
#include <Cores.h>
#include <Segoe_UI20pt7b.h>

#include "CarregadorDeTela.h"
#include "MenuController.h"

TFT_eSPI tft;
CarregadorDeTela Tela;
MenuController Menu;

#define BtnEsquerdo 16
#define BtnDireito 21
#define BtnConfirmar 17

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
  pinMode(BtnDireito, INPUT_PULLUP);
  pinMode(BtnConfirmar, INPUT_PULLUP);

}

void loop() {
  Menu.VerificarBotaoApertado(tft);
}
