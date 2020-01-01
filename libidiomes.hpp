#ifndef LIBIDIOMES_HPP_INCLUDED
#define LIBIDIOMES_HPP_INCLUDED

#include "VectorArd.h"
#include <Arduino.h>

//#include <vector>

using namespace std;

const char LLETRESCAT[10][11] = {
        {'E', 'S', 'O', 'N', 'R', 'L', 'A', 'M', 'U', 'N', 'A'},
        {'D', 'O', 'S', 'L', 'E', 'S', 'N', 'T', 'R', 'E', 'S'},
        {'C', 'I', 'N', 'C', 'Q', 'U', 'A', 'R', 'T', 'S', 'U'},
        {'M', 'E', 'N', 'Y', 'S', 'I', 'E', 'C', 'I', 'N', 'C'},
        {'D', 'E', 'D', 'R', 'U', 'N', 'A', 'O', 'N', 'Z', 'E'},
        {'D', 'U', 'E', 'S', 'T', 'R', 'E', 'S', 'E', 'T', 'D'},
        {'Q', 'U', 'A', 'T', 'R', 'E', 'D', 'O', 'T', 'Z', 'E'},
        {'V', 'U', 'I', 'T', 'N', 'O', 'U', 'O', 'N', 'Z', 'E'},
        {'S', 'I', 'S', 'A', 'M', 'D', 'E', 'U', 'N', 'Y', 'S'},
        {'M', 'E', 'N', 'Y', 'S', 'I', 'A', 'C', 'I', 'N', 'C'}
        };


const char LLETRESCAS[10][11] = {
        {'E', 'S', 'O', 'N', 'E', 'L', 'A', 'S', 'U', 'N', 'A'},
        {'D', 'O', 'S', 'I', 'T', 'R', 'E', 'S', 'O', 'A', 'M'},
        {'C', 'U', 'A', 'T', 'R', 'O', 'C', 'I', 'N', 'C', 'O'},
        {'S', 'E', 'I', 'S', 'A', 'S', 'I', 'E', 'T', 'E', 'N'},
        {'O', 'C', 'H', 'O', 'N', 'U', 'E', 'V', 'E', 'P', 'M'},
        {'L', 'A', 'D', 'I', 'E', 'Z', 'S', 'O', 'N', 'C', 'E'},
        {'D', 'O', 'C', 'E', 'L', 'Y', 'M', 'E', 'N', 'O', 'S'},
        {'O', 'V', 'E', 'I', 'N', 'T', 'E', 'D', 'I', 'E', 'Z'},
        {'V', 'E', 'I', 'N', 'T', 'I', 'C', 'I', 'N', 'C', 'O'},
        {'M', 'E', 'D', 'I', 'A', 'C', 'U', 'A', 'R', 'T', 'O'}
        };


VectorArd calculaParaulesCat(int hora, int minut, bool sonParaules);


VectorArd calculaParaulesCas(int hora, int minut, bool sonParaula);


VectorArd calculaParaules(int hora, int minut, bool sonParaula, int idioma);



#endif // LIBIDIOMES_HPP_INCLUDED
