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
  /* Fehler beim Schreiben der Datei */
  ERR_FILE_WRITE,
  /* Fehler beim Lesen der Datei (Datei kann nicht gelesen werden) */
  ERR_FILE_READ,
  /* Fehler beim Verarbeiten der Kommandozeilen-Argumente */
  ERR_INVALID_ARG,
  /* Die Bildgroesse ist groesser als das Array */
  ERR_IMAGE_SIZE,
  /* Ungueltiger PGM Header beim Einlesen */
  ERR_PGM_INVALID_HEAD,
  /* Ungueltige PGM Bilddaten beim Einlesen */
  ERR_PGM_INVALID_DATA,
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
