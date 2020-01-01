#ifndef LIBHARDWARE_HPP_INCLUDED
#define LIBHARDWARE_HPP_INCLUDED

#include "Arduino.h"


void imprimeixEstat(int millis);

class FiltreTouch
{
private:
    int _inputPin;
    int _llindar;
    int _minMillis;
    bool _flanc;

    bool _estat=false;
    bool _ultimEstat=false;
    bool _estatFlanc;

    int _valorRaw;
    bool _valor;
    bool _ultimValor=false;

    int _ultimaConmutacioMillis;

    bool touchToBool(uint16_t entradaTouch, uint16_t llindar);

public:
    FiltreTouch();
    void iniciaFiltreTouch(int iPin, int llindar, int minMillis, bool flanc);
    int getValorRaw();
    bool llegeix();
};

#endif