#ifndef __ERRORS_H__
#define __ERRORS_H__

/**
 * @file errors.h Schnittstelle eines Moduls zur Fehlerbehandlung.
 *
 * Diese Datei darf nicht veraendert werden!
 *
 * @author Helga Karafiat, Malte Heins
 */

/**
 * Aufzaehlung von Fehlerfaellen.
 */
enum Errorcode
{
  /** kein Fehler */
  ERR_NULL = 0,
  /** Falsche Parameteranzahl */
  ERR_WRONG_ARG_CNT,
  /** Polynom entspricht nicht der Syntax */
  ERR_SYNTAX_ERROR,
  /** Fehlendes oder fehlerhaftes Argument */
  ERR_WRONG_ARG,
  /** Argument ist keine Fliesskommazahl */
  ERR_NODOUBLE,
  /** Argument ist keine vorzeichenlose Ganzzahl */
  ERR_NOUINT,
  /** Ungueltige Startwerte */ 
  ERR_INVALID_STARTVALUES,
  /** Maximale Schrittanzahl erreicht */
  ERR_MAX_STEPS_REACHED,
  /** Division durch Null */
  ERR_DIVISION_BY_ZERO,
  /** Speicherleck */
  ERR_MEMORY_LEAK,
  /** Fehler beim Allozieren von Speicher */
  ERR_OUT_OF_MEMORY
};

/**
 * Aufzaehlungstyp fuer Fehlerfaelle.
 */
typedef enum Errorcode Errorcode;

/**
 * Gibt die zum Fehlercode error gehoerende Fehlermeldung auf stderr aus.
 * @param[in] error Fehlercode.
 * @return Wert des Parameters error.
 */
Errorcode printError (Errorcode error);

#endif
