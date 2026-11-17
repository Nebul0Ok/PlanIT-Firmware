#pragma once

class LeitorComunicacao{
    public:
    void lerBuffer();
    void lerSerial();

    private:
    char buffer[1000];
    int posicao = 0;
};