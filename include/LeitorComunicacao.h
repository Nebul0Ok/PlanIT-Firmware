#pragma once

#include "GerenciadorTarefa.h"

class LeitorComunicacao{
    public:
    LeitorComunicacao();
    LeitorComunicacao(GerenciadorTarefa& gerenciador);

    void inicializar(GerenciadorTarefa& gerenciador);
    void setGerenciador(GerenciadorTarefa& gerenciador);

    void lerBuffer();
    void lerSerial();

    private:
    GerenciadorTarefa* gerenciador = nullptr;
    char buffer[1000];
    int posicao = 0;
};