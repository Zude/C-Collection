#ifndef __STATE_H__
#define __STATE_H__

/**
 * @file state.h Schnittstelle eines Moduls zur Verwaltung des Zustandes einer 
 * Berechnung.
 *
 * Die Zustaende (State) und die Signatur der bereits vorimplementierten Funktion
 * duefern nicht veraendert werden.  Weitere sinnvoll in dieses Modul passende 
 * Funktionen duerfen bei Bedarf ergaenzt werden.
 *
 * @author Helga Karafiat, Malte Heins
 */

/** 
 * Zustand der Berechnung 
 */
typedef enum State {
  /** Verfahren laeuft - Linke Grenze wird verschoben */
  STATE_RUNNING_FROM_THE_LEFT = 0,
  /** Verfahren laeuft - Rechte Grenze wird verschoben */
  STATE_RUNNING_FROM_THE_RIGHT,
  /** Letzter Schritt der Berechnung */
  STATE_COMPLETED_LAST_STEP,
  /** Endergebnis */  
  STATE_COMPLETED_RESULT,
  /** Abgebrochen wegen Iterationsbeschraenkung */
  STATE_MAX_STEPS_REACHED,
  /** Abgebrochen wegen Nulldivision */
  STATE_DIV_BY_ZERO
} State;

/**
 * Uebersetzt den Status in eine textuelle Kurzform.
 *
 * Diese Funktion ist vollstaendig vorgegeben und darf nicht veraendert werden.
 *
 * @param[in] state Status.
 * @return Zeichenkette mit der Kurzform.
 */
char* state_getState (State state);

#endif
