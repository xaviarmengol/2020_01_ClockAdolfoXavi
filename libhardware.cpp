#include "Arduino.h"
#include "libhardware.hpp"


// Clase Filtre Touch ///////////////


FiltreTouch::FiltreTouch(){

}

void FiltreTouch::iniciaFiltreTouch(int iPin, unsigned long minMillis, int llindar) {
    _inputPin = iPin;
    _minMillis = minMillis;
    _llindar = llindar;
}

void FiltreTouch::llegeix(){

    _valorRaw = touchRead(_inputPin);
    _valor = touchToBool(_valorRaw, _llindar);

    //if (_valor) _valorAcum = _valorAcum+1.0; // agreguem el valor per simular un condensador

    if (_valor != _ultimValor){ // en cas de canvi, reseteja comptador
        _ultimaConmutacioMillis = millis();
    }

    _deltaMillis = millis() - _ultimaConmutacioMillis;

    if (_deltaMillis > _minMillis){
       // _estat = ((_valor/(float)_deltaMillis)>0.5)? 1 : 0; // comparem si ha estat a 1 més de la meitat del temps
       _estat = _valor;
    }

    // Flanc pujada només
    if ((_ultimEstat !=_estat) && (_estat == true)) _estatFlanc = true;
    else _estatFlanc = false;

    _ultimValor = _valor;
    _ultimEstat = _estat;
    
}

int FiltreTouch::getValorRaw(){
    return (_valorRaw);
}


bool FiltreTouch::touchToBool(uint16_t entradaTouch, uint16_t llindar){

    return(entradaTouch <= llindar);
}

bool FiltreTouch::estat(){
    return(_estat);
}

bool FiltreTouch::apretat(){
    if (_estatFlanc) {
        Serial.print("Apretat : ");
        Serial.println(getValorRaw());
    }
    return(_estatFlanc);
}

