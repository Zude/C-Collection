#ifndef __ERROR_H__
#define __ERROR_H__
/**
 * @file error.h
 *
 * Defintion der Fehlercodes / Fehlerfaelle.
 *
 * Diese Datei darf nicht veraendert werden. 
 * 
 * @author kar, mhe
 */

#include <stdio.h>

/**
 * Aufzaehlung aller moeglichen Fehlerfaelle.
 */
enum Errorcode {
  /* Alles gut - kein Fehler */
  ERR_NULL = 0,
  /* Falsche Parameteranzahl */
  ERR_WRONG_ARG_COUNT,
  /* Fehler beim Verarbeiten der Kommandozeilen-Argumente */
  ERR_INVALID_ARG,
  /* Fehler beim Einlesen einer Menge, die Syntax der Menge stimmt nicht */
  ERR_INVALID_SET_SYNTAX,
  /* Fehler beim Einlesen eines Elements */
  ERR_INVALID_ELEMENT,
  /* Speicherleck **/
  ERR_MEMORY_LEAK,
  /* Fehler beim Allozieren von Speicher */
  ERR_OUT_OF_MEMORY,
  /* Unbekannter Fehler */
  ERR_UNKNOWN
};
/** Typdefinition. */
typedef enum Errorcode Errorcode;


/**
 * Schreibt die entsprechende Fehlermeldung in der uebergebenen Ausgabestrom.
 *
 * @param stream Ausgabestrom.
 * @param err Fehlercode
 */
void printError(FILE * stream, Errorcode err);

#endif
