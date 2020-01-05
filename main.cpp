// Llibreries de Hardware
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Llibreries de WEB
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <HTTPClient.h> // conectar a APIs

// Llibreria d'autoconnexió WiFi (client + servidor)
#include <AutoConnect.h>

// JSON
#include <ArduinoJson.h>

// Llibreria temps offline (https://github.com/hickey/arduino/tree/master/Libraries/DateTime/DateTime)
#include <Datetime.h>

//OTA 
#include <HTTPUpdate.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include "HTTPUpdateServer.h"

//Llibreries aplicació
#include "main.hpp"
#include "libidiomes.hpp"
#include "libdisplayclassArd.hpp"
#include "libhardware.hpp"
#include "defPaginesWeb.hpp"

using namespace std;

//TODO Netejar includes. Ja estan al main.hpp?


//////////////////////////////////////////////////
//                                              //
// S   E   T   U   P                            //
//                                              //
//////////////////////////////////////////////////


void setup() {

    Serial.begin(115200);
    Serial.println("Arranca Setup");

    // Inicialitzem Paràmetres

    param.paraulaText = false;
    param.sonParaula = false;
    param.idioma = 0;
    param.pinOut = 2;
    param.ajustHora = 0;
    param.textStyleWEB ="height: 300px; width: 300px; background-color: #1111; border-collapse: collapse; border-style: hidden; margin-left: auto; margin-right: auto;";

    // Inicialitzem OBJECTES APLICACIÓ

    matriu.resetTimmer();
    
    // Inicialitzem Autoconnect (WEB)

    Config.title = "Menú WiFi - Rellotge";
    Config.apid = "rellotge_IOT";
    Config.psk  = "12345678"; //12345678
    Config.autoReconnect = true; // Autoreconectar a la WIFI anterior
    Portal.config(Config);

    PageConf.on(entradaPageConf, AC_EXIT_AHEAD);
    PageElemW.on(entradaPageConfW, AC_EXIT_AHEAD);
    
    PageReboot.on(entradaPageReboot, AC_EXIT_LATER);

    PageAPI.on(entradaPageAPI, AC_EXIT_AHEAD);
    PageAPIW.on(entradaPageAPIW, AC_EXIT_AHEAD);

    PageApiSants.on(entradaPageApiSants, AC_EXIT_LATER);

    PageAPIfb.on(entradaPageAPIfb, AC_EXIT_AHEAD);
    PageAPIfbW.on(entradaPageAPIfbW, AC_EXIT_AHEAD);

    PageHora.on(entradaPageHora, AC_EXIT_AHEAD);
    PageHoraW.on(entradaPageHoraW, AC_EXIT_AHEAD);
    
    Portal.join({update, PageReboot, PageAPI, PageAPIW, PageApiSants, PageAPIfb, PageAPIfbW, PageConf, PageElemW, PageHora, PageHoraW});    // Registrem les pagines al Autoconnect + OTA (update)
    
    Server.on("/", rootPage);

    if (Portal.begin()) {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
    }

    // OTA (part of Autoconnect)

    httpUpdater.setup(&Server, USERNAME, PASSWORD);

    if (Portal.begin()) {
        if (MDNS.begin(host)) {
            MDNS.addService("http", "tcp", HTTP_PORT);
            Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
        }
        else
        Serial.println("Error setting up MDNS responder");
    }

    // Inicialitzem Servidor NTP
    
    timeClient.begin();
    timeClient.setTimeOffset(3600);
    timeClient.setUpdateInterval(1000*60*10); // cada 10 minuts

    // Inicialitzem Datetime (si no tenim NTP)

    DateTime.sync(0);

    // actualitzem hora

    actualitzaHora();

    if (dataDeNTP) Serial.println("Conectat: " + timeClient.getFormattedTime());

    // Inicialitzem HARDWARE

    pixels.begin(); // LEDs
    pixels.show();
    pixels.setPin(param.pinOut);

    pinMode(0,INPUT);
    pinMode(4,INPUT);
    touchInputA.iniciaFiltreTouch(0, 30, 100, true);
    touchInputB.iniciaFiltreTouch(4, 30, 100, true);


    // Gestió del Delay

    lastMillis = millis();

    Serial.println("Final Setup");
}


//////////////////////////////////////////////////
//                                              //
// L   O   O   P                                //
//                                              //
//////////////////////////////////////////////////

void loop() {

    // Llegim HARDWARE

    touchA = touchInputA.llegeix();
    touchB = touchInputB.llegeix();

    millisEsperant = millis()-lastMillis;

    // Decidim els canvis d'estat

    canvisModes();

    // Actualitzem la hora;

    actualitzaHora();
    
    puntsExteriors = minut % 5;

    // TODO: Iluminar punts exteriors via LEDs 

    if (millisEsperant>millisDelay) {

        // Delay per defecte. Si es vol modificar, s'ha d'explicitar a cada volta del LOOP.
        millisDelay = 100;

        matriu.setMatriuOff();

        // Preparem la matriu segons el mode

        estatModes();

        // Gestió hardware

        matriu.imprimexMatriuLed(pixels, param.pinOut, pixels.Color(127, 127, 127), pixels.Color(0, 0, 0));
        
        // PER DEGBUGAR;
        imprimeixEstat(2000);
        //matriu.imprimexMatriu();

        // Delay
        lastMillis = millis();
    }
    

    // Gestió del Autoconnect (WEB)

    Portal.handleClient();

    fixaPeriodeExecucioLoop(); // per mantenir un temps de scan limitat

    
}


//////////////////////////////////////////////////
//                                              //
// Funcions auxiliars MAIN                      //
//                                              //
//////////////////////////////////////////////////

// Gestió del temps màxim/mínim d'execució

void fixaPeriodeExecucioLoop(){

    tempsDeCicle = millis() - lastMillisCicleLoop;

    if (tempsDeCicle<=cicleLoop){
        //Serial.print("Cicle Max =  " + String(cicleLoop));
        //Serial.println("Cicle Real = " + String(tempsDeCicle));
        delay(cicleLoop-tempsDeCicle);
    } else {
        Serial.print("WATCH DOG ---> Cicle Max =  " + String(cicleLoop));
        Serial.println("Cicle Real = " + String(tempsDeCicle));
    }

    lastMillisCicleLoop = millis();
}

// DEBUG del estat dels modes

void imprimeixEstat(int millis){

    String strHoraOrigen;

    Serial.println("..............");
    Serial.print("Mode :");
    Serial.println(modeOperacio);
    Serial.print("HM->");
    Serial.print(hora24);
    Serial.print(":");
    Serial.print(minut);
    Serial.print(":");
    Serial.println(segon);
    Serial.print("DMA->");
    Serial.print(dia);
    Serial.print("-");
    Serial.print(mes);
    Serial.print("-");
    Serial.println(any);
    Serial.println(String(textMode.c_str()));
    Serial.println(DateTime.now());
    Serial.println(timeClient.getEpochTime());
    Serial.print("A->");
    Serial.print(touchA);
    Serial.print(">>");
    Serial.println(touchInputA.getValorRaw());
    Serial.print("B->");
    Serial.print(touchB);
    Serial.print(">>");
    Serial.println(touchInputB.getValorRaw());
    Serial.println(strHoraOrigen);

    millisDelay = millis;

}

string int_to_string(int num){
    return(String(num).c_str());
}


// Actualització de la HORA

void actualitzaHora(){

    bool actualitzada;
    unsigned long timeEpochSeconds;
    byte segB, minB, horaB, diaSB, diaB, mesB, anyB;


    // Intenta actualitzar la data de NTP


    if (dataDeNTP) {
        intentsNTP = 0;
        while(true) {
            actualitzada = timeClient.update();
            if (!actualitzada) {
                timeClient.forceUpdate();
                Serial.print("t");
                intentsNTP++;
                if (intentsNTP>maxIntentsNTP) {
                    Serial.println("Error conexió NTP");
                    //dataDeNTP = false;
                    errorConexioNTP = true;
                    break;
                }
            } else {
                errorConexioNTP = false;
                break;
            }
        }
    }

    // Si està en mode de NTP

    if (dataDeNTP) {

        hora24 = timeClient.getHours() + param.ajustHora;
        hora = hora24 % 12;
        minut = timeClient.getMinutes();
        segon = timeClient.getSeconds();

        // formated date: 2018-04-30T16:00:13Z
        // formattedDate = timeClient.getFormattedDate();

        timeEpochSeconds = timeClient.getEpochTime();

        // Utilitzem la funció de la llibreria DateTime per extreure dia, mes i any dels segons que han 
        // passat desde 1970
        DateTime.localTime(&timeEpochSeconds, &segB, &minB, &horaB, &diaB, &diaSB, &mesB, &anyB);
       
        dia = int(diaB);
        mes = int(mesB) + 1; // El mes comença a 0
        any = int(anyB) + 1900; // Ens anys comencen a 1900
        
        //DateTime.sync(DateTime.makeTime(segon, minut, hora24, dia, mes, any));
        DateTime.sync(timeClient.getEpochTime());


    } else {

        // Si no està en mode NTP

        hora24 = int(DateTime.Hour) + param.ajustHora;
        hora = hora24 % 12;
        minut = int(DateTime.Minute);
        segon = int(DateTime.Second);
        dia = int(DateTime.Day);
        mes = int(DateTime.Month) + 1;
        any = int(DateTime.Year) + 1900;
    }

    // Mante la data offline encara que hi hagi NTP
    DateTime.available();

}

//////////////////////////////////////////////////
//                                              //
// Funcions auxiliars: Gestió Màquina estats    //
//                                              //
//////////////////////////////////////////////////

//TODO: Substituir per una llibreria de gestió d'estats finits

// Gestió del CANVI de modes

void canvisModes() {

    // si està fora del menu, i toca la tecla B
    if (touchB && modeOperacio<100) {

        modeOperacio = (modeOperacio + 1) % TOTALMODES;
        matriu.resetTimmer();

    } else if (touchA) { // entrada al menu

        matriu.resetTimmer();

        // Si esta en mode normal, ves a menu
        if (modeOperacio<100) {
            newMode = modeOperacio;
            modeOperacio = 100;
        } 
        // Si està al menu, avança al següent menu
        else if (modeOperacio<(100+NUMMENU-1)) modeOperacio++;

        // Si arriba al final del menu, fes l'acció corresponent
        else if (modeOperacio==(100+NUMMENU-1)) {
            modeOperacio = newMode;
            if (newMode>=50) newMode=0;
            matriu.resetTimmer();
        }

    } else if (touchB && modeOperacio>=100){ // tecla modificació de valor

        if (modeOperacio==100) DateTime.syncIncreaseHour();
        else if (modeOperacio==101) DateTime.syncIncreaseMinute();
        else if (modeOperacio==102) {
            param.idioma = (param.idioma+1)%TOTALIDIOMES;
        }
        else if (modeOperacio==103) param.paraulaText = !param.paraulaText;
        else if (modeOperacio==104) newMode = (newMode +1) % TOTALMODES;
    }
}

// Gestió de l'estat dels modes

void estatModes() {

    switch(modeOperacio) {

        // ESTATS NORMALS: 0-49

        case 0 : {

            paraules = calculaParaules(hora, minut, param.sonParaula, param.idioma);
            matriu.escriuPuntsDeParaules(paraules, param.idioma);
            textHora = matriu.textHora(param.idioma);
            textMode = textHora;
            
            if ((textHora!= textHoraAntic) && !arrancFred) {
                modeOperacio = 50;
            }

            textHoraAntic = textHora;
            arrancFred = false;

            millisDelay=500; // En mode normal, actualitzem més lent

            break;
        }
        case 1 : {
            matriu.dosNombresAMatriu(segon/10, segon%10);
            textMode = int_to_string(segon);
            Serial.println(String(textMode.c_str()) + " " + String(segon) );
            break;
        }
        case 2 : {
            volta = matriu.rollerHora(hora, minut);
            if (volta == 3) modeOperacio=0;
            textMode = "Rolling : " + int_to_string(segon) + ":" + int_to_string(minut);
            break;
        }
        case 3 : {
            matriu.rollerText(textHora);
            textMode = "Rolling : " + textHora;
            break;
        }
        case 4 : {
            string textAux = string(WiFi.localIP().toString().c_str());
            matriu.rollerText(textAux);
            textMode = "Rolling : " + textAux;
            break;
        }
        case 5 : {
            matriu.dosNombresSevenSegAMatriu(hora, minut);
            millisDelay = 500; 
            textMode = "Hora : Minuts";
            break;
        }
        case 6 : {
            matriu.testTotsPuntsMatriu();
            millisDelay = 10; 
            textMode = "Debug Leds";
            break;
        }

        // ESTATS ESPECIALS: 50-99

        // Estat de canvi d'hora
        case 50 : { 

            keyDiaMes = String(String(dia) + "-" + String(mes+1)); // mes = 0..11

            // TODO *****************************
            //if (mapOfDiesVIP.count(keyDiaMes)==1){
            //    colorLeds = colorAni;
            //    volta = matriu.rollerText(mapOfDiesVIP.at(keyDiaMes), 100);

            //} else
            

            if (param.paraulaText) {
                volta = matriu.rollerText(textHora);
            }


            if (volta == 1 || !param.paraulaText) modeOperacio=0;
            break;
        }

        // Estat d'arranc
        case 51 : {
            string text = "A&X";
            volta = matriu.flashText(text);
            if (volta == 1) modeOperacio=0;
            break;
        }

        // ESTATS DE MENU

        case 100: {
            matriu.menuClock("Hora", hora24);
            textMode = "Menu : Canvi hora : " + int_to_string(hora24);
            break;
        }
        case 101: {
            matriu.menuClock("Minut", minut);
            textMode = "Menu : Canvi minut : " + int_to_string(minut);
            break;
        }
        case 102: {
            matriu.menuClock("Idioma", IDIOMASTRLIST[param.idioma%TOTALIDIOMES]);
            textMode = "Menu : Canvi idioma: " + int_to_string(param.idioma);
            break;
        }
        case 103: {
            matriu.menuClock("Text Hora", int(param.paraulaText));
            textMode = "Menu : Activar/Desactivar Text Hora : " + int_to_string(param.paraulaText);
            break;
        }
        case 104: {
            matriu.menuClock("Mode", MODESTRLIST[newMode%TOTALMODES]);
            textMode = "Menu : Definir Mode : " + int_to_string(newMode); // No cal perquè es pot definir directament
            break;
        }

    }

}




//////////////////////////////////////////////////
//                                              //
// AutoConnect: Pàgines i transicións           //
//                                              //
//////////////////////////////////////////////////

// Definició de la pàgina principal

void rootPage() {

    //String valorStr;

    String capcelera =
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<script type=\"text/javascript\">"
    "setTimeout(\"location.reload()\", {{tempsReload}});"
    "</script>"
    "</head>"
    "<body>"
    "<p style=\"text-align: center;font-size:5vw;font-family:Arial;\"><span style=\"text-align: center; color: #0000ff;\"><span style=\"color: #ff0000;\"><strong> Rellotge Adolfo &amp; Xavi </strong></span></span></p>"
    "<p style=\"text-align: center;font-family:Arial;\">{{hora}}</p>"
    "<p style=\"text-align: center;font-family:Arial;\">{{mode}}</p>"
    "<p style=\"text-align: center;font-family:Arial;\">AVUI: {{eventDia}}</p>"
    "<p style=\"padding-top:15px;text-align:center\">" AUTOCONNECT_LINK(COG_24) "</p>";

    String peu =
    "</tbody>"
    "</table>"
    "<p style=\"text-align: center;font-family:Arial;\"><a href=\"/configuracio\" title=\"config\"> Configuraci&oacute; </a></p>";

    String taula = 
    "<table border=\"0\" style=\"{{styleWEB}}\">"
    "<tbody>";

    for (int i = 0; i < 10; i++) {
        taula = taula + "<tr>";
        for (int j = 0; j < 11; j++) {
            taula = taula + "<td style=\"text-align: center; font-size:3.5vw; font-family:Arial;\">";

            //valorStr.clear();
            String valorStr = String(matriu.getLletraIdioma(i,j,param.idioma));
            //valorStr = valorStr + "  ";

            if (matriu.getMatriuPunt(i,j)) {
                taula = taula + "<strong><span style=\"color: #0000ff;\">" + valorStr + "</span></strong>";
            } else {
                taula = taula + valorStr;
            }

            taula = taula + "</td>";
        }
        taula = taula + "</tr>";
    }
    taula = taula + "</table>";
    
    capcelera.replace("{{hora}}", String(hora24) + " : " + String(minut));
    capcelera.replace("{{mode}}", String(textMode.c_str()));
    capcelera.replace("{{tempsReload}}", String(reloadPaginaPrinc));
    capcelera.replace("{{eventDia}}", apiICScalendar());

    taula.replace("{{styleWEB}}", param.textStyleWEB);
    
    Server.send(200, "text/html", capcelera+taula+peu);
}


// Funció de transció quan es crida la pàgina de configuració

String entradaPageConf(AutoConnectAux& aux, PageArgument& args) {

    string idiomaSelector;
    String activacioNTP;
    String sonParaula;
    String paraulaText;

    Serial.println("Carrega paràmetres");

    AutoConnectInput& inputText = aux.getElement<AutoConnectInput>("modeop");
    inputText.value = String(modeOperacio);

    AutoConnectSelect& input1 = aux.getElement<AutoConnectSelect>("idioma");
    input1.selected = param.idioma + 1;

    AutoConnectSelect& checkNTP = aux.getElement<AutoConnectSelect>("checkNTP_conf");
    checkNTP.selected = dataDeNTP ? 1 : 2; // Si, No

    AutoConnectInput& inputMaxInt = aux.getElement<AutoConnectInput>("maxIntentsNTP_conf");
    inputMaxInt.value = String(maxIntentsNTP);

    AutoConnectInput& inputAjustHora = aux.getElement<AutoConnectInput>("ajustHora_conf");
    inputAjustHora.value = String(param.ajustHora);

    AutoConnectSelect& checkParaulaText = aux.getElement<AutoConnectSelect>("paraulaText_conf");
    checkParaulaText.selected = param.paraulaText ? 1 : 2; // Si, No

    AutoConnectSelect& checkSonParaula = aux.getElement<AutoConnectSelect>("sonParaula_conf");
    checkSonParaula.selected = param.sonParaula ? 1 : 2; // Si, No

    AutoConnectInput& inputPinOut = aux.getElement<AutoConnectInput>("pinNeo_conf");
    inputPinOut.value = String(param.pinOut);

    AutoConnectInput& inputStyleWeb = aux.getElement<AutoConnectInput>("textStyle_conf");
    inputStyleWeb.value = String(param.textStyleWEB);
        
    return String();
}


// Funció de transció quan es crida la pàgina de configuració (Escriure)

String entradaPageConfW(AutoConnectAux& aux, PageArgument& args) {

    string idiomaSelector;
    String activacioNTP;
    String sonParaula;
    String paraulaText;


    Serial.println("Escriu paràmetres");
    
    // modeOperacio
    modeOperacio = Server.arg("modeop").toInt();

    // idioma
    idiomaSelector = string(Server.arg("idioma").c_str());
    if (idiomaSelector == IDIOMASTRLIST[0]) param.idioma = 0;
    else if (idiomaSelector == IDIOMASTRLIST[1]) param.idioma =  1;

    // activació NTP
    activacioNTP = Server.arg("checkNTP_conf");
    if (activacioNTP == "Si") dataDeNTP = true;
    else if (activacioNTP == "No") dataDeNTP = false;

    // Max Intents NTP
    maxIntentsNTP = Server.arg("maxIntentsNTP_conf").toInt();

    // Ajust Hora (canvi hora)
    param.ajustHora = Server.arg("ajustHora_conf").toInt();


    // activació Paraula Text cada n minuts
    paraulaText = Server.arg("paraulaText_conf");
    if (paraulaText == "Si") param.paraulaText = true;
    else if (paraulaText == "No") param.paraulaText = false;

    // activació opció Son de les paraules
    sonParaula = Server.arg("sonParaula_conf");
    if (sonParaula == "Si") param.sonParaula = true;
    else if (sonParaula == "No") param.sonParaula = false;

    // PIN Neopixel
    param.pinOut = Server.arg("pinNeo_conf").toInt();

    // Text Style WEB 
    param.textStyleWEB = Server.arg("textStyle_conf");

    return String();
}


// Entrada pagina hora


String entradaPageHora(AutoConnectAux& aux, PageArgument& args) {

    unsigned long timeSegons;
    byte segonR, minutR, horaR, diaR, diaSR, mesR, anyR;

    Serial.println("Carrega paràmetres HORA - FORA");

    timeSegons = DateTime.now();
    DateTime.localTime (&timeSegons, &segonR, &minutR, &horaR, &diaR, &diaSR, &mesR, &anyR);

    AutoConnectInput& inputSeg = aux.getElement<AutoConnectInput>("segon_hora");
    inputSeg.value = String(segonR);

    AutoConnectInput& inputMin = aux.getElement<AutoConnectInput>("minut_hora");
    inputMin.value = String(minutR);

    AutoConnectInput& inputHora = aux.getElement<AutoConnectInput>("hora_hora");
    inputHora.value = String(horaR);

    AutoConnectInput& inputDia = aux.getElement<AutoConnectInput>("dia_hora");
    inputDia.value = String(diaR);

    AutoConnectInput& inputMes = aux.getElement<AutoConnectInput>("mes_hora");
    inputMes.value = String(mesR + 1);

    AutoConnectInput& inputAny = aux.getElement<AutoConnectInput>("any_hora");
    inputAny.value = String(anyR + 1900);
      
    
    return String();
}

// Escriptura configuració hora

String entradaPageHoraW(AutoConnectAux& aux, PageArgument& args) {

    byte segonW, minutW, horaW, diaW, diaSW, mesW; 
    int anyW;
    unsigned long newEpoch;

    Serial.println("Escriu paràmetres HORA - Ella mateixa");
    
    segonW = byte(Server.arg("segon_hora").toInt());
    minutW = byte(Server.arg("minut_hora").toInt());
    horaW = byte(Server.arg("hora_hora").toInt());
    diaW = byte(Server.arg("dia_hora").toInt());


    mesW = byte(Server.arg("mes_hora").toInt());
    anyW = Server.arg("any_hora").toInt();

    Serial.println(segonW);
    Serial.println(minutW);
    Serial.println(horaW);
    Serial.println(diaW);
    Serial.println(mesW);
    Serial.println(anyW);

    newEpoch = DateTime.makeTimeC(segonW, minutW, horaW, diaW, mesW, anyW);

    Serial.println(newEpoch);

    DateTime.sync(newEpoch);

    //timeClient.setEpochTime(newEpoch);
    
    return String();
}


// Funció de transció quan es crida la pàgina

String entradaPageAPI(AutoConnectAux& aux, PageArgument& args) {
    String resultatAPI;

    AutoConnectInput& input = aux.getElement<AutoConnectInput>("textAPI_api");
    input.value = cridaAPI;
    resultatAPI = getApiResult(cridaAPI);
    //resultatAPI = apiSants(cridaAPI);

    AutoConnectText& resultat = aux.getElement<AutoConnectText>("resultat_api");
    resultat.value = resultatAPI;

    return String();
}

// Funció de transció quan es crida la pàgina - Escriure

String entradaPageAPIW(AutoConnectAux& aux, PageArgument& args) {

    // Guardem la nova petició
    cridaAPI = Server.arg("textAPI_api");

    return String();
}


// Funció de transció quan es crida la pàgina

String entradaPageApiSants(AutoConnectAux& aux, PageArgument& args) {
    String resultatAPI;

    resultatAPI = apiSants(Server.arg("textAPI_api_dia").toInt(), Server.arg("textAPI_api_mes").toInt());

    AutoConnectText& resultat = aux.getElement<AutoConnectText>("resultat_api");
    resultat.value = resultatAPI;

    return String();
}



// Funció de transció quan es crida la pàgina

String entradaPageAPIfb(AutoConnectAux& aux, PageArgument& args) {
    String resultatAPI;

    
    AutoConnectInput& input = aux.getElement<AutoConnectInput>("textAPI_fb");
    input.value = cridaAPIfb;
    resultatAPI = apiFootBallData();

    AutoConnectText& resultat = aux.getElement<AutoConnectText>("resultat_fb");
    resultat.value = resultatAPI;

    return String();
}

// Funció de transció quan es crida la pàgina - Escriure

String entradaPageAPIfbW(AutoConnectAux& aux, PageArgument& args) {

    // Guardem la nova petició

    // TODO: No funciona perque la petició es genera dins la funció de la API
    cridaAPIfb = Server.arg("textAPI_fb");

    return String();
}

// Funció de transció quan es crida la pàgina

String entradaPageReboot(AutoConnectAux& aux, PageArgument& args) {

    ESP.restart();

    return String();
}




//////////////////////////////////////////////////
//                                              //
// Gestió APIs                                  //
//                                              //
//////////////////////////////////////////////////


// Gestió APIs

String getApiResult(String peticio, String XAuth){

    // Per saber com deserialitzar!!!
    //https://arduinojson.org/v6/assistant/

    HTTPClient http;
    String payload;
    
    // incialitzem client APIs
    Serial.println(peticio);
    
    http.begin(peticio);

    if (XAuth !="res") {
        http.addHeader("X-Auth-Token", XAuth);
    }

    int httpCode = http.GET();  //Make the request

    if (httpCode > 0) { //Check for the returning code
        payload = http.getString();
        // TODO: Modificar a ---> http.getStream
        
        Serial.println(httpCode);
        //Serial.println(payload);
    } else {
        Serial.print("Error on HTTP request : ");
        Serial.println(httpCode);
    }

    http.end(); // Alliberem la conexió

    //return(payload);
    return(payload);
}


 // API SANTS ;D 
 // Retorna un String amb els Sants


String apiSants (int dia, int mes) {

    String peticio;
    String strRetornat;

    peticio = "https://api.abalin.net/namedays?country=es&month=" + String(mes) + "&day=" + String(dia);

    cridaAPI = peticio; // Per debugar. TODO -> Mes endavant no caldrà

    String jsonStr;
    jsonStr = getApiResult(peticio);
    jsonStr.replace('\\', '_'); // TODO: S'ha de completar amb caractes Unicode

    DeserializationError error = deserializeJson(doc, jsonStr);

    Serial.println("Deserialitzat");

    if (error) {
        Serial.println("ERROR: Lectura Json : ");
        Serial.println(jsonStr);
        strRetornat = jsonStr;
    } else {

        if (doc.containsKey("data")) {
            const char* resultat = doc["data"][0]["namedays"]["es"];
            strRetornat = String(resultat);
        } else{
            strRetornat = String("La API respon amb un error : " + String(jsonStr));
        }

    }

    return(strRetornat);
    

}


String apiFootBallData() {

    //https://www.football-data.org/documentation/quickstart/
    //https://www.jokecamp.com/blog/guide-to-football-and-soccer-data-and-apis/

    HTTPClient http;
    String payload;
    String peticio;
    String clauXToken;
    String strRetornat;
    
    // incialitzem client APIs

    // TODO: Modificar perque agafi el dia d'avui!!!
    //peticio = "https://api.football-data.org/v2/teams/81/matches?dateFrom=2019-12-21&dateTo=2020-01-02&limit=1";
    
    //cridaAPI = peticio;
    clauXToken = "xxx";

    String jsonStr;
    jsonStr = getApiResult(cridaAPIfb, clauXToken);
    jsonStr.replace('\\', '_'); // TODO: S'ha de completar amb caractes Unicode

    DeserializationError error = deserializeJson(doc, jsonStr);

    Serial.println("Deserialitzat");

    // Lectura d'un sol partit 

    if (error) {
        Serial.println("ERROR: Lectura Json : ");
        Serial.println(jsonStr);
        strRetornat = jsonStr;
    } else {

        if (doc.containsKey("matches")) {
            int golsCasa = doc["matches"][0]["score"]["fullTime"]["homeTeam"];
            int golsFora = doc["matches"][0]["score"]["fullTime"]["awayTeam"];
            const char* equipCasa = doc["matches"][0]["homeTeam"]["name"];
            const char* equipFora = doc["matches"][0]["awayTeam"]["name"];

            strRetornat = String(equipCasa) + " " + String(golsCasa) + " - " + String(equipFora) + " " + String(golsFora);
        } else{
            strRetornat = String("La API respon amb un error : " + String(jsonStr));
        }

    }
    //Serial.print("Resultat->");
    Serial.println(strRetornat);
    return(strRetornat);

}


String apiICScalendar() {

    char dades[8];
    String diaAvui;
    String peticio;
    String strRetornat;

    peticio = "https://calendar.google.com/calendar/ical/m5qnigr8bu7549j2aikf8o869g%40group.calendar.google.com/private-68d3004a5a28ae3a9257d08092d123d9/basic.ics";

    cridaAPI = peticio; // Debug. No caldrà més endavant

    // Calculem el string que identifica el dia
    sprintf(dades, "%04d%02d%02d", any, mes, dia);
    diaAvui = String("DTSTART:") + String(dades);

    //Serial.println(diaAvui);

    String icsCalendar = getApiResult(peticio);

    int indexAvui = icsCalendar.indexOf(diaAvui,0);
    //Serial.println(indexAvui);
    strRetornat = subStringEntreParaules(icsCalendar, "SUMMARY", "TRANSP", indexAvui); // aqui es on hi ha el titol del event
    return(strRetornat);

}

String subStringEntreParaules(String text, String inicial, String final, int indexInicial){
    
    int startDate = text.indexOf(inicial, indexInicial);
    int endDate = text.indexOf(final,startDate);
    String strResposta = text.substring(startDate + inicial.length(), endDate);

    Serial.print("Inicial:");
    Serial.println(startDate);
    Serial.print("Final:");
    Serial.println(endDate);
    Serial.print("Text:");
    Serial.println(strResposta);

    return(strResposta);
}



    




