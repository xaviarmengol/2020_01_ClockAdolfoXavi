#ifndef DEFPAGINESWEB_HPP_INCLUDED
#define DEFPAGINESWEB_HPP_INCLUDED

#include <AutoConnect.h>
#include <main.hpp>

//////////////////////////////////////////////////
//                                              //
// Definicions del Autoconnect (WEB)            //
//                                              //
//////////////////////////////////////////////////

String entradaPageConf(AutoConnectAux& aux, PageArgument& args);
String entradaPageConfW(AutoConnectAux& aux, PageArgument& args);
String entradaPageAPI(AutoConnectAux& aux, PageArgument& args);
String entradaPageReset(AutoConnectAux& aux, PageArgument& args);
String entradaPageHora(AutoConnectAux& aux, PageArgument& args);
String entradaPageHoraW(AutoConnectAux& aux, PageArgument& args);

//////////////////////////////////////////////////
//                                              //
// Pàgina configuració rellotge                 //
//                                              //
//////////////////////////////////////////////////

ACText(header, "<h2>Opcions Rellotge</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption, "Opcions de configuració del rellotge", "font-family:serif;color:#4682b4;");
ACInput(modeop, "", "Mode Operació", "^[0-9]$");
ACSelect(idioma, { String(IDIOMASTRLIST[0].c_str()), String(IDIOMASTRLIST[1].c_str())}, "Seleccioni Idioma", 1);
ACElement(newline, "<hr>");
ACSelect(checkNTP_conf, { "Si", "No"}, "NTP activat");
ACInput(maxIntentsNTP_conf, "", "Max. Intents NTP", "");
ACInput(ajustHora_conf, "", "Ajust hora estiu", "");
ACSelect(paraulaText_conf, { "Si", "No"}, "Hora en text activat");
ACSelect(sonParaula_conf, { "Si", "No"}, "Iniciar hora amb Son");
ACInput(pinNeo_conf, "", "Pin de sortida dels leds", "");
ACInput(textStyle_conf, "", "Estil rellotge WEB", "");

ACSubmit(guardar, "Guardar", "/configuracio_w");
ACSubmit(reset, "Reset", "/reset");
ACSubmit(home_conf, "Home","/");
ACElement(adjust_width, "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"size\",e.length*.8)}};</script>");


AutoConnectAux PageConf ("/configuracio", "Configuració Rellotge", true, {
    header,
    caption,
    modeop,
    idioma,
    newline,
    checkNTP_conf,
    maxIntentsNTP_conf,
    ajustHora_conf,
    paraulaText_conf,
    sonParaula_conf,
    pinNeo_conf,
    textStyle_conf,
    newline,
    newline,
    guardar,
    reset,
    home_conf,
    adjust_width
});


//////////////////////////////////////////////////
//                                              //
// Pàgina configuració rellotge  - CONF         //
//                                              //
//////////////////////////////////////////////////

ACText(header_conf_w, "<h2>Configuració actualitzada</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_conf_w, "Configuració actualitzada", "font-family:serif;color:#4682b4;");
ACSubmit(hora_conf_w, "Tornar Configuració Rellotge", "/configuracio");
ACSubmit(home_conf_w, "Tornar Home", "/");

AutoConnectAux PageElemW ("/configuracio_w", "Opcions Rellotge", false, {
    header_conf_w,
    caption_conf_w,
    hora_conf_w,
    home_conf_w
});

//////////////////////////////////////////////////
//                                              //
// Pàgina Reset ESP Autoconnect                 //
//                                              //
//////////////////////////////////////////////////

ACText(header_reset, "<h2>Reset Rellotge</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_reset, "Està segur de realitzar un reset al rellotge?", "font-family:serif;color:#4682b4;");
ACSubmit(conf_reset, "Confirmar reset", "/reset");
ACSubmit(home_reset, "Tornar Home", "/");

AutoConnectAux PageReset ("/reset", "Reset Rellotge", false, {
    header_reset,
    caption_reset,
    conf_reset,
    home_reset
});


//////////////////////////////////////////////////
//                                              //
// Pàgina test APIs                             //
//                                              //
//////////////////////////////////////////////////

ACText(header_api, "<h2>Test APIs</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_api, "Test de les APIs del rellotge", "font-family:serif;color:#4682b4;");
ACInput(textAPI_api, "", "Petició de la API", "");
ACText(resultat_api, "...", "font-family:serif;color:#4682b4;");
ACSubmit(conf_api, "Llançar la petició de la API", "/ap");
ACSubmit(home_api, "Tornar Home", "/");

AutoConnectAux PageAPI ("/ap", "Access Point", true, {
    header_api,
    caption_api,
    textAPI_api,
    resultat_api,
    conf_api,
    home_api
});


//////////////////////////////////////////////////
//                                              //
// Pàgina Configuració Temps OFFLINE            //
//                                              //
//////////////////////////////////////////////////

ACText(header_hora, "<h2>Configuració Hora - Data</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_hora, "Menú de configuració de la hora i la data", "font-family:serif;color:#4682b4;");

ACInput(hora_hora, "", "Hora", "^0*(?:[0-9][0-9]?|24)$");
ACInput(minut_hora, "", "Minut", "");
ACInput(segon_hora, "", "Segon", "");
ACInput(any_hora, "", "Any", "");
ACInput(mes_hora, "", "Mes", "");
ACInput(dia_hora, "", "Dia", "");

ACSubmit(conf_hora, "Confirmar actualització hora", "/hora_w");
ACSubmit(home_hora, "Tornar Home", "/");

AutoConnectAux PageHora ("/hora", "Configuració Hora i Data", true, {
    header_hora,
    caption_hora,

    hora_hora, 
    minut_hora,
    segon_hora,
    newline, 
    dia_hora,  
    mes_hora,
    any_hora,
    newline,    
    conf_hora,
    home_hora
});


//////////////////////////////////////////////////
//                                              //
// Pàgina Configuració Temps OFFLINE - CONF     //
//                                              //
//////////////////////////////////////////////////

ACText(header_hora_w, "<h2>Hora i data configurada</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_hora_w, "Hora i data configurada", "font-family:serif;color:#4682b4;");
ACSubmit(hora_hora_w, "Tornar Configuració Hora", "/hora");
ACSubmit(home_hora_w, "Tornar Home", "/");

AutoConnectAux PageHoraW ("/hora_w", "Configuració Hora i Data", false, {
    header_hora_w,
    caption_hora_w,
    hora_hora_w,
    home_hora_w
});


#endif
