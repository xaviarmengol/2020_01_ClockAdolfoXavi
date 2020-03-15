#ifndef LIBDISPLAYCLASS_HPP_INCLUDED
#define LIBDISPLAYCLASS_HPP_INCLUDED

#include "VectorArd.h"

//#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_SSD1306.h>

#define DIM_FIL 10
#define DIM_COL 11

using namespace std;

//typedef vector<double> typeRow;

class Matriu {

    bool _matriu[DIM_FIL][DIM_COL];
    bool _old_matriu[DIM_FIL][DIM_COL];
    int _nFil = DIM_FIL;
    int _nCol = DIM_COL;
    int _timmer;
    int _voltes;
    int _pinOut;
    //MapArd _mapCar;


    public:

        Matriu();

        void imprimexMatriu();
        void imprimexMatriuLed(Adafruit_NeoMatrix &matrix, int pinOut, uint32_t colorOn, uint32_t colorOff);
        void imprimexMatriuOLED96(Adafruit_SSD1306 &display);

        void setMatriuOff();
        void cuadratAMatriu(int filIni, int colIni, int filEnd, int colEnd, bool valor, int offSetFil=0, int offSetCol=0);
        void guardaEstatMatriu();

        void getMatriu(bool (&matriu_sortida)[DIM_FIL][DIM_COL]);
        void setMatriuPunt(int fil, int col, bool valor);
        bool getMatriuPunt(int fil, int col);

        //void setMapCar(MapArd mapCar);
        uint64_t getImatgeDeCar(char car);
        uint64_t getImatgeDeNum(int valor);

        void setTimmer(int timmer);
        int getTimmer();
        void resetTimmer();
        int incTimmerTemps(int maxim);
        int getVoltes();

        void imatgeAMatriu(uint64_t imatge, int offsetCol=0, int iniCol=0, int finCol=7);
        void dosNombresAMatriu(int num1, int num2);

        int rollerHora(int hora, int minut);
        int rollerText(string textHora);
        int flashText(string text);
        int testTotsPuntsMatriu();

        void escriuPuntsDeParaules(VectorArd paraules, int idioma);
        string textHora(int idioma);
        char getLletraIdioma (int fila, int col, int idioma);
        //MapArd crearMapImatges(const uint64_t imatges[], int numImatges, String llistaCaracters);

        void menuClock(string text, int valor);
        void menuClock(string text, string textValor);
        void menuClock(string text, char carValor);


        void segmentAMatriu(int segment, int offSetFil=0, int offSetCol=0);
        void sevenSegAMatriu(int num, int offSetFil, int offSetCol);
        void dosNombresSevenSegAMatriu(int num1, int num2);

};




const uint64_t LLETRES[] = {
  0x6666667e66663c00,
  0x3e66663e66663e00,
  0x3c66060606663c00,
  0x3e66666666663e00,
  0x7e06063e06067e00,
  0x0606063e06067e00,
  0x3c66760606663c00,
  0x6666667e66666600,
  0x3c18181818183c00,
  0x1c36363030307800,
  0x66361e0e1e366600,
  0x7e06060606060600,
  0xc6c6c6d6feeec600,
  0xc6c6e6f6decec600,
  0x3c66666666663c00,
  0x06063e6666663e00,
  0x603c766666663c00,
  0x66361e3e66663e00,
  0x3c66603c06663c00,
  0x18181818185a7e00,
  0x7c66666666666600,
  0x183c666666666600,
  0xc6eefed6c6c6c600,
  0xc6c66c386cc6c600,
  0x1818183c66666600,
  0x7e060c1830607e00,
  0x0000000000000000,
  0x7c667c603c000000,
  0x3e66663e06060600,
  0x3c6606663c000000,
  0x7c66667c60606000,
  0x3c067e663c000000,
  0x0c0c3e0c0c6c3800,
  0x3c607c66667c0000,
  0x6666663e06060600,
  0x3c18181800180000,
  0x1c36363030003000,
  0x66361e3666060600,
  0x1818181818181800,
  0xd6d6feeec6000000,
  0x6666667e3e000000,
  0x3c6666663c000000,
  0x06063e66663e0000,
  0xf0b03c36363c0000,
  0x060666663e000000,
  0x3e403c027c000000,
  0x1818187e18180000,
  0x7c66666666000000,
  0x183c666600000000,
  0x7cd6d6d6c6000000,
  0x663c183c66000000,
  0x3c607c6666000000,
  0x3c0c18303c000000,
  0x3c66666666663c00,
  0x0010107c10100000,
  0x0000003c00000000,
  0x006c38fe386c0000,
  0x00060c1830600000,
  0x60660c1830660600,
  0x00003c003c000000,
  0x000000365c000000,
  0x0000008244281000,
  0x6030180c18306000,
  0x060c1830180c0600,
  0x6030181818306000,
  0x060c1818180c0600,
  0x7818181818187800,
  0x1e18181818181e00,
  0x7018180c18187000,
  0x0e18183018180e00,
  0x0606000000000000,
  0x0018180018180000,
  0x0c18180018180000,
  0x060c0c0c00000000,
  0x180018183c3c1800,
  0x1800183860663c00,
  0x003c421a3a221c00,
  0xfc66a6143c663c00,
  0x103c403804781000,
  0x6c6cfe6cfe6c6c00,
  0x0000000018181800,
  0x3c4299858599423c,
  0x1c2222222222221c, // 0
  0x1c08080808080c08,
  0x3e0408102020221c,
  0x1c2220201820221c,
  0x20203e2224283020,
  0x1c2220201e02023e,
  0x1c2222221e02221c,
  0x040404081020203e,
  0x1c2222221c22221c,
  0x1c22203c2222221c
};
const int LLETRES_LEN = sizeof(LLETRES)/8;

const String LLETRES_STR = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyzÓ+-*/%=~^<>()[]{}.:;,!?@&$#'©0123456789";


const int posicioParaulesCat[][3] = {
    {0, 1, 3}, // SON
    {0, 5, 2}, // LA
    {0, 8, 2}, // UN
    {1, 0, 3}, // DOS
    {1, 3, 3}, // LES
    {1, 7, 4}, // TRES
    {2, 0, 4}, // CINC
    {2, 4, 5}, // QUART
    {2, 4, 6}, // QUARTS
    {3, 0, 5}, // MENYS
    {3, 5, 1}, // I
    {3, 7, 4}, // CINC
    {4, 0, 2}, // DE
    {4, 2, 1}, // D'
    {4, 4, 3}, // UNA
    {4, 7, 4}, // ONZE
    {5, 0, 4}, // DUES
    {5, 4, 4}, // TRES
    {5, 7, 3}, // SET
    {6, 0, 6}, // QUATRE
    {6, 6, 5}, // DOTZE
    {7, 0, 4}, // VUIT
    {7, 4, 3}, // NOU
    {8, 0, 3}, // SIS
    {8, 5, 3}, // DEU
    {9, 0, 5}, // MENYS
    {9, 5, 1}, // I
    {9, 7, 4} // CINC
};

const int posicioParaulesCas[][3] = {
    {0, 0, 2}, // 0 ES
    {0, 1, 3}, // 1 SON
    {0, 5, 2}, // 2 LA
    {0, 5, 3}, // 3 LAS
    {0, 8, 3}, // 4 UNA
    {1, 0, 3}, // 5 DOS
    {1, 4, 4}, // 6 TRES
    {2, 0, 6}, // 7 CUATRO
    {2, 6, 5}, // 8 CINCO
    {3, 0, 4}, // 9 SEIS
    {3, 5, 5}, // 10 SIETE
    {4, 0, 4}, // 11 OCHO
    {4, 4, 5}, // 12 NUEVE
    {5, 2, 4}, // 13 DIEZ
    {5, 7, 4}, // 14 ONCE
    {6, 0, 4}, // 15 DOCE
    {6, 5, 1}, // 16 Y
    {6, 6, 5}, // 17 MENOS
    {7, 1, 6}, // 18 VEINTE
    {7, 7, 4}, // 19 DIEZ
    {8, 0, 11}, // 20 VEINTICINCO
    {8, 6, 5}, // 21 CINCO (de Y MENOS)
    {9, 0, 5}, // 22 MEDIA
    {9, 5, 6} // 23 CUARTO
};

const bool SEVENSEG[10][7] = {
    {1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1},
    {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1},
    {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1},
    {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1}
};


#endif // LIBDISPLAYCLASS_HPP_INCLUDED
