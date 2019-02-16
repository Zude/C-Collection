/**
 * @file state.c Implementierung eines Moduls zur Verwaltung des Zustandes 
 * einer Berechnung.
 *
 * Die vorgebene Funktion in dieser Datei darf nicht veraendert werden, weitere sinnvoll
 * in dieses Modul passende Funktionen duerfen bei Bedarf ergaenzt werden.
 *
 * @author Helga Karafiat, Malte Heins
 */

#include "state.h"

char* 
state_getState (State state) {

  switch (state) {
    case STATE_RUNNING_FROM_THE_LEFT:
      return "RFTL";
      break;
    case STATE_RUNNING_FROM_THE_RIGHT:
      return "RFTR";
      break;
    case STATE_COMPLETED_LAST_STEP:
      return "CLS";
      break;      
    case STATE_COMPLETED_RESULT:
      return "CR";
      break;
    case STATE_MAX_STEPS_REACHED:
      return "!MSR";
      break;
    case STATE_DIV_BY_ZERO:
      return "!DBZ";
      break;
    default:
      return "!!UNKNWN";
      break;
  }

  
}

