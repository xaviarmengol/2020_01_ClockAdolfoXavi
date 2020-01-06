#ifndef LIBHARDWARE_HPP_INCLUDED
#define LIBHARDWARE_HPP_INCLUDED

#include "Arduino.h"




class FiltreTouch
{
private:
    int _inputPin;
    int _llindar;
    unsigned long _minMillis;

    bool _estat=false;
    bool _ultimEstat=false;
    bool _estatFlanc;

    int _valorRaw;
    bool _valor;
    bool _ultimValor=false;
    float _valorAcum=0.0;

    unsigned long _ultimaConmutacioMillis;
    unsigned long _deltaMillis;

    bool touchToBool(uint16_t entradaTouch, uint16_t llindar);

public:
    FiltreTouch();
    void iniciaFiltreTouch(int iPin, unsigned long minMillis, int llindar);
    int getValorRaw();
    void llegeix();
    bool estat(); // estat del boto
    bool apretat(); // flanc pujada
};

#endif