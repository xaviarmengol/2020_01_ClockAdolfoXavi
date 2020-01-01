// Clase: libdisplayclassArd.cpp
// Llibreria de gestió de la matriu de punts del rellotge
// (c) Xavi Armengol - 12/2019

#include "libidiomes.hpp"
#include "libdisplayclassArd.hpp"
#include "VectorArd.h"
#include <Adafruit_NeoPixel.h>

using namespace std;


//////////////////////////////////////////////////
//                                              //
// METODES BÀSICS DE MATRIU                     //
//                                              //
//////////////////////////////////////////////////

// Constructor: Inicialitzar la matriu a false. Inicialitzar timer a 0.

Matriu::Matriu(){

    setMatriuOff();
    _timmer = 0;
    _pinOut = 2;
}

// GET de la matriu: Sortida via referencia

void Matriu::getMatriu(bool (&matriu_sortida)[DIM_FIL][DIM_COL]){
    for (int i=0; i<_nFil;i++){
        for (int j=0; j<_nCol;j++){
            matriu_sortida[i][j] = _matriu[i][j];
        }
    }
}

// SET de un punt de la Matriu. 

void Matriu::setMatriuPunt(int fil, int col, bool valor){
    if (fil<_nFil && fil>=0 && col<_nCol && col>=0)
        _matriu[fil][col] = valor;
}

// GET de un punt de la Matriu. Retorna el valor del punt.

bool Matriu::getMatriuPunt(int fil, int col){
    if (fil<_nFil && fil>=0 && col<_nCol && col>=0)
        return _matriu[fil][col];
    else
        return 0;
}

// Dibuixa un cuadrat a la matriu a un valor

void Matriu::cuadratAMatriu(int filIni, int colIni, int filEnd, int colEnd, bool valor, int offSetFil, int offSetCol){

    for (int i=filIni; i<=filEnd; i++){
        for (int j=colIni; j<=colEnd; j++){
            setMatriuPunt(i+offSetFil, j+offSetCol, valor);
        }
    }
}

// Posar la matriu a false

void Matriu::setMatriuOff() {

    cuadratAMatriu(0, 0, _nFil, _nCol, false);

}

// Guardar estat matriu a un una matriu temporal

void Matriu::guardaEstatMatriu() {
    for (int i=0; i<_nFil; i++){
        for (int j=0; j<_nCol; j++){
            _old_matriu[i][j] = _matriu[i][j];
        }
    }
}


//////////////////////////////////////////////////
//                                              //
// METODES DE GESTIO D'IMATGES 8x8 (uint64)     //
//                                              //
//////////////////////////////////////////////////


// GET imatge (format uint64) corresponent a un caràcter

uint64_t Matriu::getImatgeDeCar(char car){

    int index=0;
    while (index<LLETRES_LEN){

        if (LLETRES_STR[index]==car) break;
        index++;
    }

    return LLETRES[index];
}

// GET imatge (format uint64) corresponent a un nombre

uint64_t Matriu::getImatgeDeNum(int valor){

    return getImatgeDeCar(char(48+valor));
}

//////////////////////////////////////////////////
//                                              //
// METODES DE GESTIO DEL TIMMER                 //
//                                              //
//////////////////////////////////////////////////

// TODO: Clarificar perquè es necessari

// SET / GET Timmer

void Matriu::setTimmer(int valor){
    _timmer = valor;
}

int Matriu::getTimmer(){
    return _timmer;
}

void Matriu::resetTimmer(){
    setTimmer(0);
    _voltes = 0;
}

// Incrementar timmer i compta les voltes

int Matriu::incTimmerTemps(int maxim){
    int valorTimmer = (getTimmer()+1) % maxim;
    setTimmer(valorTimmer);

    if (valorTimmer == 0) _voltes++;

    return _voltes;
}

int Matriu::getVoltes(){
    return _voltes;
}


//////////////////////////////////////////////////
//                                              //
// IMPRESIO DE LA MATRIU                        //
//                                              //
//////////////////////////////////////////////////

// Imprimeix a port SERIE - DEBUG

void Matriu::imprimexMatriu(){

    String matriuText = "";

    for (int i=0; i<_nFil; i++){
        for (int j=0; j<_nCol; j++){
            //Serial.print(getMatriuPunt(i,j));
            //Serial.print(" ");
            matriuText = matriuText + String(getMatriuPunt(i,j)) + " ";
        }
        //Serial.println("");
        matriuText = matriuText + "\n";
    }

    Serial.println(matriuText);

}

// Imprimeix punts de la matriu de forma seqüencial - TEST DELS LEDS

int Matriu::testTotsPuntsMatriu() {

    int iMax = getTimmer() / _nCol;
    int jMax = getTimmer() % _nCol;

    for (int i=0;i<iMax;i++) {
        for (int j=0;j<_nCol;j++)
            setMatriuPunt(i, j, true);
    }

    for (int j=0;j<=jMax;j++) {
            setMatriuPunt(iMax, j, true);
    }

    return incTimmerTemps(_nCol*_nFil);
}

// Imprimeix una imatge (uint64)
// Imatge uint 64 es 8x8 i s'ha d'imprimir en una matriu nFil / nCol
// offsetCol -> posició de la matriu destí on es comença a imprimir
// indexI -> columna inicial de la imatge que s'imprimeix
// indexJ -> columna final de la imatge que s'imprimeix
// Les files son fixes. 

// TODO: Parametritzar els offsets de les files

void Matriu::imatgeAMatriu(uint64_t imatge, int offsetCol, int iniCol, int finCol) {

    int valor;
    int offsetFila = 1;
    int indexI;
    int indexJ;

    for (int i = 0; i < 8; i++) {
        uint8_t  row = (imatge >> i * 8) & 0xFF;
        for (int j = iniCol; j < finCol+1; j++) {
            valor = ( (row>>j) & 1);
            indexI = i+offsetFila;
            indexJ = j+offsetCol-iniCol;

            if (valor) setMatriuPunt(indexI, indexJ, true);
            else if (!valor) setMatriuPunt(indexI, indexJ, false);
        }
      }
}

// Posar dos nombres a una matriu. Sense moviment.

void Matriu::dosNombresAMatriu(int num1, int num2){
    imatgeAMatriu(getImatgeDeNum(num1), 0, 1);
    imatgeAMatriu(getImatgeDeNum(num2), 6, 1, 5);
}

// Rolling de la hora i el minut

int Matriu::rollerHora(int hora, int minut){

    int inici = _nCol-getTimmer()-2;
    int espai = 6;

    imatgeAMatriu(getImatgeDeNum(hora/10), inici+espai*1, 1);
    imatgeAMatriu(getImatgeDeNum(hora%10), inici+espai*2, 1);
    imatgeAMatriu(getImatgeDeCar(':'), inici+espai*3, 1);
    imatgeAMatriu(getImatgeDeNum(minut/10), inici+espai*4, 1);
    imatgeAMatriu(getImatgeDeNum(minut%10), inici+espai*5, 1);

    return incTimmerTemps(_nCol*5);
}

// Rolling d'un text

int Matriu::rollerText(string text){

    int espai = 8;

    int inici = _nCol-getTimmer()-2;

    int i=0;
    for (char& car : text) {
        imatgeAMatriu(getImatgeDeCar(car), inici+espai*i, 1);
        i++;
    }

    return incTimmerTemps((text.length()+2)*espai);
}

// Mostra imatges de forma seqüencial

int Matriu::flashText(string text){

    imatgeAMatriu(getImatgeDeCar(text[getTimmer()]), 1, 0);

    return incTimmerTemps(text.length()+1);
}


//////////////////////////////////////////////////
//                                              //
// IMPRESIO DE LES PARAULES (segons Idioma)     //
//                                              //
//////////////////////////////////////////////////

// TODO: Aquesta funció està massa relacionada amb l'idioma. S'ha de separar

void Matriu::escriuPuntsDeParaules(VectorArd paraules, int idioma){

    int fil;
    int colIni;
    int llarg;
    int paraula;
    int totalParaules;

    int totalParaulesCat = sizeof(posicioParaulesCat) / (sizeof(int)*3);
    int totalParaulesCas = sizeof(posicioParaulesCas) / (sizeof(int)*3);

    if (idioma==0) totalParaules = totalParaulesCat;
    else totalParaules = totalParaulesCas;

    for (int i=0; i<paraules.size(); i++){

        paraula = paraules[i];

        if (paraula>=0 && paraula <totalParaules) {

            if (idioma==0) {
                fil =  posicioParaulesCat[paraula][0];
                colIni =  posicioParaulesCat[paraula][1];
                llarg =   posicioParaulesCat[paraula][2];

            } else {
                fil =  posicioParaulesCas[paraula][0];
                colIni =  posicioParaulesCas[paraula][1];
                llarg =   posicioParaulesCas[paraula][2];
            }

            for (int col=colIni; col<colIni+llarg; col++) {
                setMatriuPunt(fil, col, true);
            }

        }

    }

}

// Converteix la matriu de punts en text

string Matriu::textHora(int idioma){

    char textAdd;
    string text = "";
    bool anteriorText = true;


    for (int i=0; i<_nFil; i++){
        for (int j=0; j<_nCol; j++) {

            if (getMatriuPunt(i,j)){
                    if (!anteriorText || (j==0 && i!=0)){
                        text = text + " ";
                    }

                textAdd = getLletraIdioma(i, j, idioma);

                text.push_back(textAdd);
                anteriorText = true;
            } else
                anteriorText = false;

        }
    }
    return text;

}

// Converteix la matriu de punts en text

char Matriu::getLletraIdioma (int fila, int col, int idioma){

    char textAdd;

    if (idioma==0) textAdd = LLETRESCAT[fila][col];
    else textAdd = LLETRESCAS[fila][col];

    return textAdd;
}


//////////////////////////////////////////////////
//                                              //
// HARDWARE: SORTIDA DE LA MATRIU A LEDS        //
//                                              //
//////////////////////////////////////////////////

// Gestiona la tira de leds. Cada led es gestiona via una adreça seqüencial amb la llibreria Adafruit Neopixel

void Matriu::imprimexMatriuLed(Adafruit_NeoPixel &pixels, int pinOut, uint32_t colorOn, uint32_t colorOff){

    bool actualitzaSempre = false; // Només actualitza quant hi ha un canvi d'estat
    int numLed;

    // Modifiquem el pin de sortida si es necessari
    if (pinOut != _pinOut) {
        _pinOut = pinOut;
        pixels.setPin(_pinOut);
    }

    for (int i=0; i<_nFil; i++){
        for (int j=0; j<_nCol; j++){
            if ((_old_matriu[i][j] !=_matriu[i][j]) || actualitzaSempre) {

                numLed = _nFil*i + j; // Direcció de la tira de leds
                if (getMatriuPunt(i,j)) 
                    pixels.setPixelColor(numLed, colorOn);
                else 
                    pixels.setPixelColor(numLed, colorOff);

            }
        }
    }

    pixels.show(); /// Veure si s'ha d'activar a cada pixel o no TODO!!!

    guardaEstatMatriu();
}

//////////////////////////////////////////////////
//                                              //
// GESTIO DE MENU D'OPCIONS                     //
//                                              //
//////////////////////////////////////////////////

// Presenta el text del menu amb un roller, i després presenta el valor de la opció.
// El valor es presenta segons el tipus de dada.

void Matriu::menuClock(string text, int valor){

    if (getVoltes()==0) {
        rollerText(text);
    } else {
        dosNombresAMatriu(valor/10, valor%10);
    }
}


void Matriu::menuClock(string text, string textValor){

    if (getVoltes()==0) {
        rollerText(text);
    } else {
        rollerText(textValor);
    }
}

void Matriu::menuClock(string text, char carValor){

    if (getVoltes()==0) {
        rollerText(text);
    } else {
        imatgeAMatriu(getImatgeDeCar(carValor));
    }
}


//////////////////////////////////////////////////
//                                              //
// GESTIO DE SIMULACIO DE SET SEGMENTS          //
//                                              //
//////////////////////////////////////////////////

// Dibuixa segment, de SET SEGMENTS, a matriu
// TODO: Crear clase propia

void Matriu::segmentAMatriu(int segment, int offSetFil, int offSetCol){

    if (segment==0){
        cuadratAMatriu(0,0,0,3, true, offSetFil, offSetCol);
    } else if (segment == 1){
        cuadratAMatriu(0,3,2,3, true, offSetFil, offSetCol);
    } else if (segment == 2){
        cuadratAMatriu(2,3,4,3, true, offSetFil, offSetCol);
    } else if (segment == 3){
        cuadratAMatriu(4,0,4,3, true, offSetFil, offSetCol);
    } else if (segment == 4){
        cuadratAMatriu(2,0,4,0, true, offSetFil, offSetCol);
    } else if (segment == 5){
        cuadratAMatriu(0,0,2,0, true, offSetFil, offSetCol);
    } else if (segment == 6){
        cuadratAMatriu(2,0,2,3, true, offSetFil, offSetCol);
    }
}

// Dibuixa numero a matriu. Es basa en un numero de 5x4

void Matriu::sevenSegAMatriu(int num, int offSetFil, int offSetCol){

    for (int i=0; i<7; i++){
        if (SEVENSEG[num][i]) segmentAMatriu(i, offSetFil, offSetCol);
    }

}

// Dibuixa:
// N1 s1 N2
// N3 s2 N4

void Matriu::dosNombresSevenSegAMatriu(int num1, int num2){

    sevenSegAMatriu(num1/10, 0, 0);
    sevenSegAMatriu(num1%10, 0, 5);
    sevenSegAMatriu(num2/10, 5, 0);
    sevenSegAMatriu(num2%10, 5, 5);
}





