/*
  DateTime.cpp - Arduino Date and Time library
  Copyright (c) Michael Margolis.  All right reserved.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

// Tenia bugs i els he resolt usant les llibreries de C (time.h)
// https://www.epochconverter.com/

extern "C" {
  // AVR LibC Includes
}
//#include <string.h> // for memset
#include "Datetime.h"
#include <arduino.h>
#include <time.h>


//extern unsigned long _time;

#define LEAP_YEAR(_year) ((_year%4)==0)
static  byte monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

// private methods

void DateTimeClass::setTime(unsigned long time)
{
  // set the system time to the given time value (as seconds since Jan 1 1970)
  this->sysTime = time;  
	this->prevMillis = millis();
}


//******************************************************************************
//* DateTime Public Methods
//******************************************************************************

DateTimeClass::DateTimeClass()
{
   this->status = dtStatusNotSet;
}

unsigned long DateTimeClass::now()
{
  while( millis() - prevMillis >= 1000){
    this->sysTime++;
    this->prevMillis += 1000;
  }
  return sysTime;
}

void DateTimeClass::sync(unsigned long time) 
{
   setTime(time); 
   //status.isSynced = true;   // this will be set back to false if the clock resets 
   //status.isSet = true; // if this is true and isSynced is false then clock was reset using EEPROM -- TODO
   this->status = dtStatusSync;
}

void DateTimeClass::syncIncreaseHour(){

  sync(now()+SECS_PER_HOUR);

}

void DateTimeClass::syncIncreaseMinute(){

  sync(now()+SECS_PER_MIN);
  
}

boolean DateTimeClass::available()
{  
// refresh time components if clock is set (even if not synced), just return false if not set
   if(this->status != dtStatusNotSet) { 
      this->now(); // refresh sysTime   
      this->localTime(&this->sysTime,&Second,&Minute,&Hour,&Day,&DayofWeek,&Month,&Year)  ;     
	  return true;
   }
   else
      return false;
}

void DateTimeClass::localTime(unsigned long *timep,byte *psec,byte *pmin,byte *phour,byte *pday,byte *pwday,byte *pmonth,byte *pyear) {
// convert the given unsigned long to time components
// this is a more compact version of the C library localtime function

// TODO: utilitzar la funció de C: https://www.epochconverter.com/programming/c

  unsigned long epoch=*timep;
  byte year;
  byte month, monthLength;
  unsigned long days;
  
  *psec=epoch%60;
  epoch/=60; // now it is minutes
  *pmin=epoch%60;
  epoch/=60; // now it is hours
  *phour=epoch%24;
  epoch/=24; // now it is days
  *pwday=(epoch+4)%7;
  
  year=70;  
  days=0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch) {
    year++;
  }
  *pyear=year; // *pyear is returned as years from 1900
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  epoch -= days; // now it is days in this year, starting at 0
  //*pdayofyear=epoch;  // days since jan 1 this year
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    if (month==1) { // february
      if (LEAP_YEAR(year)) {
        monthLength=29;
      } else {
        monthLength=28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (epoch>=monthLength) {
      epoch-=monthLength;
    } else {
        break;
    }
  }
  *pmonth=month;  // jan is month 0
  *pday=epoch+1;  // day of month
}


unsigned long DateTimeClass::makeTimeC(byte seg, byte min, byte hour, byte day, byte month, int year ){
    struct tm t;
    time_t t_of_day;

    t.tm_year = year-1900;  // Year - 1900
    t.tm_mon = month-1;           // Month, where 0 = jan
    t.tm_mday = day;          // Day of the month
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    return(t_of_day);
}


// make one instance for DateTime class the user 
DateTimeClass DateTime = DateTimeClass() ;