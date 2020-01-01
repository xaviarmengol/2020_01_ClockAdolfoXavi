#include "libidiomes.hpp"
#include "VectorArd.h"
//#include <vector>


using namespace std;


VectorArd calculaParaules(int hora, int minut, bool sonParaules, int idioma){

    switch(idioma) {
      case 0 :
         return calculaParaulesCat(hora, minut, sonParaules);
         break;
      case 1 :
          return calculaParaulesCas(hora, minut, sonParaules);
          break;
   }

}

VectorArd calculaParaulesCat(int hora, int minut, bool sonParaules){

    int article;
    int article_min;
    VectorArd paraules;

    int avant = 5; //7 per centrar-ho

    int quart = ((minut + avant) % 60) / 15;
    int cincs = (((minut + avant) % 60) % 15) / 5;

    int paraula_art_quarts[] = {-1,2,3,5};
    int paraula_quarts[] = {-1,7,8,8};
    int paraula_minuts[] = {9, -1, 10};
    int paraula_minuts_cinc[] = {11, -1, 11};
    int paraula_hores[] = {20,14,16,17,19,27,23,18,21,22,24,15};
    int paraula_altres[] = {1,4,-1,13,12,-1,0};
    int paraula_minuts_h[] = {25,-1,26};
    int paraula_minuts_h_cinc[] = {27,-1,27};


    if (quart==0 && cincs<2 && minut>45){
        hora++;
    }

    hora = hora % 12;
    int hora_seguent = (hora + 1) % 12;

    if (sonParaules) paraules.push_back(paraula_altres[6]);

    if (quart == 0){


        if (hora == 1) {
            article = paraula_altres[0];
        } else {
            article = paraula_altres[1];
        }


        if (cincs == 1) {
            paraules.push_back(article);
            paraules.push_back(paraula_hores[hora]);
            paraules.push_back(paraula_altres[2]);

        } else {

            if (hora == 5) {
                paraules.push_back(article);
                paraules.push_back(paraula_minuts_cinc[0]);
                paraules.push_back(paraula_minuts_h[cincs]);
                paraules.push_back(paraula_minuts_h_cinc[cincs]);

            } else {
                paraules.push_back(article);
                paraules.push_back(paraula_hores[hora]);
                paraules.push_back(paraula_minuts_h[cincs]);
                paraules.push_back(paraula_minuts_h_cinc[cincs]);
            }
        }

    } else {

        if (hora_seguent == 1 || hora_seguent == 11) {
            article_min = paraula_altres[3];
        } else {
            article_min = paraula_altres[4];
        }

        paraules.push_back(paraula_art_quarts[quart]);
        paraules.push_back(paraula_quarts[quart]);
        paraules.push_back(paraula_minuts[cincs]);
        paraules.push_back(paraula_minuts_cinc[cincs]);
        paraules.push_back(article_min);
        paraules.push_back(paraula_hores[hora_seguent]);

    }

    return paraules;
}

VectorArd calculaParaulesCas(int hora, int minut, bool sonParaules){

    VectorArd paraules;

    int avant = 0; //2 per centrar-ho

    int min_avanzado = (minut + avant) % 60;
    int rang_minuts = min_avanzado / 5;
    bool ultima_media_hora = rang_minuts > 6;

    int paraules_hores[]  = {15, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int paraules_minuts[] = {-1, 21, 19, 23, 18, 20, 22, 20, 18, 23, 19, 21};
    int paraules_altres[] = {0, 1, 2, 3};
    int paraules_y_menos[] = {16, 17};

    hora = ultima_media_hora ? hora +1 : hora;
    hora = hora % 12;


    if (hora == 1) {
        if (sonParaules) paraules.push_back(paraules_altres[0]);
        paraules.push_back(paraules_altres[2]);
    } else {
        if (sonParaules) paraules.push_back(paraules_altres[1]);
        paraules.push_back(paraules_altres[3]);
    }


    paraules.push_back(paraules_hores[hora]);


    if (rang_minuts != 0) {
        paraules.push_back(paraules_y_menos[ultima_media_hora ? 1 : 0]);
        paraules.push_back(paraules_minuts[rang_minuts]);
    }

    return paraules;
}

