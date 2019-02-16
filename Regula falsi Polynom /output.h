#ifndef __OUTPUT_H__
#define __OUTPUT_H__
/**
 * @file output.h Schnittstelle eines Moduls welches Zeichenketten (Strings) 
 * verarbeiten kann. Z.B. Ausgabe auf stdout, Speicherung in einer Liste, 
 * Ausgabe ueber Netzwerk etc..
 *
 * Aktuell ist nur die Ausgabe auf stdout implementiert.
 *
 * Die Verantwortung fuer die Speicherfreigabe der jeweils uebergebenen
 * Zeichenkette geht mit Aufruf an das Modul ueber.
 *
 * Diese Datei darf nicht veraendert werden!
 *
 * @author Helga Karafiat, Malte Heins
 */


/**
 * Gibt die uebergebene Zeichenkette auf stdout aus.
 *
 * Diese Funktion ist vollstaendig vorgegeben und darf nicht veraendert werden.
 * 
 * @param Zeiger auf den Beginn der Zeichenkette
 * @pre s != NULL
 */
void output_print(char * s);


#endif
