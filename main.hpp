#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

// Hardware
#include <Arduino.h>
//#include "JC_Button.h"
#include "libhardware.hpp"

#include <WebServer.h>
#include <AutoConnect.h>
#include <NTPClient.h>

#include "VectorArd.h"
#include "libdisplayclassArd.hpp"

#include "HTTPUpdateServer.h"
#define HOSTIDENTIFY  "esp32"
#define mDNSUpdate(c)  do {} while(0)

using namespace std;

//////////////////////////////////////////////////
//                                              //
// Funcions auxiliars del MAIN                  //
//                                              //
//////////////////////////////////////////////////

void rootPage();

void imprimeixEstat(int millis);
void canvisModes();
void estatModes();
void actualitzaHora();
void fixaPeriodeExecucioLoop();

String getApiResult(String peticio, String XAuth="res");
String apiSants (int dia, int mes);
String apiFootBallData();
String apiICScalendar();
String subStringEntreParaules(String text, String inicial, String final, int indexInicial=0);

//////////////////////////////////////////////////
//                                              //
// Estructura per guardar els paràmetres        //
//                                              //
//////////////////////////////////////////////////

struct param_t {
    bool paraulaText;
    bool sonParaula;
    int idioma;
    int pinOut;
    int ajustHora;
    String textStyleWEB;
};



//////////////////////////////////////////////////
//                                              //
// Definició OBJECTES aplicació                 //
//                                              //
//////////////////////////////////////////////////

VectorArd paraules;
Matriu matriu;
int modeOperacio = 51; //mode arranc fred
int newMode;
bool arrancFred = true;

const int NUMMENU = 5;

const int TOTALIDIOMES = 2;
const int TOTALMODES = 7;
const string MODESTRLIST[TOTALMODES] = {"STD", "SEG", "24H", "TXT", "LED", "H:M", "TEST"};
const string IDIOMASTRLIST[TOTALIDIOMES] = {"CAT", "ESP"};

string textHora="";
string textHoraAntic="";

string textMode="";

int volta = 0;

// Gestió APIs

//StaticJsonDocument<300> doc;

DynamicJsonDocument doc(2000);

//////////////////////////////////////////////////
//                                              //
// Gestió del temsps de cicle (Delay)           //
//                                              //
//////////////////////////////////////////////////

unsigned long millisDelay=100;
unsigned long lastMillis=0;
unsigned long millisEsperant;

unsigned long cicleLoop = 10;
unsigned long tempsDeCicle;
unsigned long lastMillisCicleLoop;

//////////////////////////////////////////////////
//                                              //
// Definició AutoConnect (WEB + Gestió WiFi)    //
//                                              //
//////////////////////////////////////////////////

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config;       // Enable autoReconnect supported on v0.9.4

int reloadPaginaPrinc = 5000;

// OTA
static const char* host = HOSTIDENTIFY "-webupdate";

#define HTTP_PORT 80
#define USERNAME "user"   //*< Replace the actual username you want */
#define PASSWORD "pass"   //*< Replace the actual password you want */

HTTPUpdateServer httpUpdater;
AutoConnectAux  update("/update", "Update");

String cridaAPI = "https://api.abalin.net/namedays?country=es&month=7&day=23";
String cridaAPIfb = "https://api.football-data.org/v2/teams/81/matches?dateFrom=2020-01-04&dateTo=2020-01-04&limit=1";


//////////////////////////////////////////////////
//                                              //
// Definició Servidor NTP i variables de Temps  //
//                                              //
//////////////////////////////////////////////////

int maxIntentsNTP = 15;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);//, "europe.pool.ntp.org");
int intentsNTP;

int hora24 = 0;
int hora;
int minut = 0;
int segon = 0;
int dia = 1;
int mes = 1;
int any = 1970;
String formattedDate;
String keyDiaMes;

bool dataDeNTP = true;
bool errorConexioNTP = false;


//////////////////////////////////////////////////
//                                              //
// Inicialització de HARDWARE                   //
//                                              //
//////////////////////////////////////////////////

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10*11, 2, NEO_GRB + NEO_KHZ800);

//Modificació sobre -> https://github.com/JChristensen/JC_Button
FiltreTouch touchInputA, touchInputB;
bool touchA, touchB;

//Button touchInputA(0, 100, false, false, true, 30);
//Button touchInputB(0, 100, false, false, true, 30);

int puntsExteriors = 0;


//////////////////////////////////////////////////
//                                              //
// Definició estructura paràmetres              //
//                                              //
//////////////////////////////////////////////////

param_t param;

struct DiaEspecial{
    string dia;
    string text;
};



#endif