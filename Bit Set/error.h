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
  /* Zu grosse Summe in der Funktion set_partition */
  ERR_PARTITION_OVERFLOW,
  /* Falsche Parameteranzahl */
  ERR_WRONG_ARG_COUNT,
  /* Fehler beim Verarbeiten der Kommandozeilen-Argumente */
  ERR_INVALID_ARG,
  /* Fehler beim Einlesen einer Menge, das Element kann nicht dargestellt werden */
  ERR_INVALID_SET_ELEMENT,
  /* Fehler beim Einlesen einer Menge, die Syntax der Menge stimmt nicht */
  ERR_INVALID_SET_SYNTAX,
  /* Fehler beim Einlesen eines Elements */
  ERR_INVALID_ELEMENT,
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
