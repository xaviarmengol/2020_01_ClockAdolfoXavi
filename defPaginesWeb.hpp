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
String entradaPageAPIW(AutoConnectAux& aux, PageArgument& args);
String entradaPageAPIfb(AutoConnectAux& aux, PageArgument& args);
String entradaPageAPIfbW(AutoConnectAux& aux, PageArgument& args);
String entradaPageApiSants(AutoConnectAux& aux, PageArgument& args);
String entradaPageReboot(AutoConnectAux& aux, PageArgument& args);
String entradaPageHora(AutoConnectAux& aux, PageArgument& args);
String entradaPageHoraW(AutoConnectAux& aux, PageArgument& args);

//////////////////////////////////////////////////
//                                              //
// Pàgina configuració rellotge                 //
//                                              //
//////////////////////////////////////////////////

ACText(header_conf, "<h2>Opcions Rellotge</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
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
ACSubmit(home, "Home","/");
ACElement(adjust_width, "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"size\",e.length*.8)}};</script>");


AutoConnectAux PageConf ("/configuracio", "Configuració Rellotge", true, {
    header_conf,
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
    home,
    adjust_width
});


//////////////////////////////////////////////////
//                                              //
// Pàgina configuració rellotge  - WRITE        //
//                                              //
//////////////////////////////////////////////////

ACText(caption_conf_w, "Configuració actualitzada", "font-family:serif;color:#4682b4;");
ACSubmit(hora_conf_w, "Tornar Configuració Rellotge", "/configuracio");

AutoConnectAux PageElemW ("/configuracio_w", "Opcions Rellotge", false, {
    header_conf,
    caption_conf_w,
    hora_conf_w,
    home
});

//////////////////////////////////////////////////
//                                              //
// Pàgina Reboot ESP Autoconnect                 //
//                                              //
//////////////////////////////////////////////////

ACText(header_reset, "<h2>Reset Rellotge</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_reset, "Està segur de realitzar un reset al rellotge?", "font-family:serif;color:#4682b4;");
ACSubmit(conf_reset, "Confirmar reset", "/reset");

AutoConnectAux PageReboot ("/reset", "Reset Rellotge", true, {
    header_reset,
    caption_reset,
    conf_reset,
    home
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
ACSubmit(conf_api, "Llançar la petició de la API", "/api");
ACSubmit(guardar_api, "Guardar nova petició", "/api_w");
ACElement(calendari_google, "<iframe src=\"https://calendar.google.com/calendar/embed?height=600&amp;wkst=1&amp;bgcolor=%23ffffff&amp;ctz=Europe%2FMadrid&amp;src=bTVxbmlncjhidTc1NDlqMmFpa2Y4bzg2OWdAZ3JvdXAuY2FsZW5kYXIuZ29vZ2xlLmNvbQ&amp;color=%23871111&amp;title=Calendari%20Rellotge%20Adolfo%26Xavi\" style=\"border:solid 1px #777\" width=\"800\" height=\"600\" frameborder=\"0\" scrolling=\"no\"></iframe>");

AutoConnectAux PageAPI ("/api", "Test APIs", true, {
    header_api,
    caption_api,
    textAPI_api,
    guardar_api,
    resultat_api,
    conf_api,
    home,
    calendari_google
});


//////////////////////////////////////////////////
//                                              //
// Pàgina test APIs - WRITE                     //
//                                              //
//////////////////////////////////////////////////

ACText(caption_api_w, "Apis modificades", "font-family:serif;color:#4682b4;");

AutoConnectAux PageAPIW ("/api_w", "Test APIs", false, {
    header_api,
    caption_api_w,
    conf_api,
    home
});



//////////////////////////////////////////////////
//                                              //
// Pàgina lectura API Sants                     //
//                                              //
//////////////////////////////////////////////////

ACText(header_api_sants, "<h2>Test APIs Sants</h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_api_sants, "Test de la API dels Sants", "font-family:serif;color:#4682b4;");
ACInput(textAPI_api_dia, "8", "Dia", "");
ACInput(textAPI_api_mes, "12", "Mes", "");
ACSubmit(conf_api_sants, "Llançar la petició de la API Sants", "/api_sants");

AutoConnectAux PageApiSants ("/api_sants", "Test APIs Sants", true, {
    header_api_sants,
    caption_api_sants,
    textAPI_api_dia,
    textAPI_api_mes,
    resultat_api,
    conf_api_sants,
    
    home
});


//////////////////////////////////////////////////
//                                              //
// Pàgina test APIs Football                    //
//                                              //
//////////////////////////////////////////////////

ACText(header_fb, "<h2>Test APIs Football </h2>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(caption_fb, "Test API Football", "font-family:serif;color:#4682b4;");
ACInput(textAPI_fb, "", "Petició de la API", "");
ACText(resultat_fb, "...", "font-family:serif;color:#4682b4;");
ACSubmit(conf_fb, "Llançar la petició de la API", "/api_fb");
ACSubmit(guardar_fb, "Guardar nova petició", "/api_fb_w");

AutoConnectAux PageAPIfb ("/api_fb", "Test API Football", true, {
    header_fb,
    caption_fb,
    textAPI_fb,
    guardar_fb,
    resultat_fb,
    conf_fb,
    home
});


//////////////////////////////////////////////////
//                                              //
// Pàgina test APIs Football - WRITE            //
//                                              //
//////////////////////////////////////////////////

ACText(caption_fb_w, "Petició modificada", "font-family:serif;color:#4682b4;");

AutoConnectAux PageAPIfbW ("/api_fb_w", "Test API Football", false, {
    header_fb,
    caption_fb_w,
    conf_fb,
    home
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
    home
});


//////////////////////////////////////////////////
//                                              //
// Pàgina Configuració Temps OFFLINE - WRITE    //
//                                              //
//////////////////////////////////////////////////

ACText(caption_hora_w, "Hora i data configurada", "font-family:serif;color:#4682b4;");
ACSubmit(hora_hora_w, "Tornar Configuració Hora", "/hora");
ACSubmit(home_hora_w, "Tornar Home", "/");

AutoConnectAux PageHoraW ("/hora_w", "Configuració Hora i Data", false, {
    header_hora,
    caption_hora_w,
    hora_hora_w,
    home_hora_w
});


#endif

