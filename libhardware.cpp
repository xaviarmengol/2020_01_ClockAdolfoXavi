#include "Arduino.h"
#include "libhardware.hpp"


// Clase Filtre Touch ///////////////


FiltreTouch::FiltreTouch(){

}

void FiltreTouch::iniciaFiltreTouch(int iPin, int llindar, int minMillis, bool flanc) {
    _inputPin = iPin;
    _minMillis = minMillis;
    _llindar = llindar;
    _flanc = flanc;
}

bool FiltreTouch::llegeix(){

    _valorRaw = touchRead(_inputPin);
    _valor = touchToBool(_valorRaw, _llindar);

    if (_valor != _ultimValor){
        _ultimaConmutacioMillis = millis();
    }

    if ((millis() - _ultimaConmutacioMillis) > _minMillis){
        _estat = _valor;
    }

    // Flanc pujada només
    if ((_ultimEstat !=_estat) && (_estat == true)) _estatFlanc = true;
    else _estatFlanc = false;

    _ultimValor = _valor;
    _ultimEstat = _estat;
    
    if (_flanc) return (_estatFlanc);
    else return (_estat);
}

int FiltreTouch::getValorRaw(){
    return (_valorRaw);
}


bool FiltreTouch::touchToBool(uint16_t entradaTouch, uint16_t llindar){

    return(entradaTouch <= llindar);
}


