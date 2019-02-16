#ifndef __PRINTER_H__
#define __PRINTER_H__
/**
 * @file printer.h Schnittstelle eines Moduls mit Ausgabefunktionen.
 *
 * Diese Datei darf nicht veraendert werden!
 *
 * @author Helga Karafiat, Malte Heins
 */

#include <stdio.h>

#include "polynom.h"


/**
 * Gibt die uebergebenen Werte als (dynamisch allokierte) Zeichenkette zur Weiterverarbeitung 
 * an das output-Modul weiter. Hierbei wird ein minimales Ausgabeformat verwendet:
 *
 * Zuerst wird ein Praefix, welches den Zustand der Berechnung beschreibt, gemaess der 
 * Ausgabe von state_getState, in die Zeichenkette geschrieben gefolgt von einem einzelnen
 * Leerzeichen. 
 *
 * Ist der State STATE_COMPLETED so folgt nur noch die Ausgabe des Endergebnisses (welches
 * auf beiden Parameterstellen a und b uebergeben wird).
 * Bei allen anderen States folgen die Zahlen a und b voneinander getrennt durch ein 
 * einzelnes Leerzeichen.
 *
 * Die Ausgaben der Zwischen- und Endergebnisse erfolgen stets mit einer
 * Genauigkeit von sechs Nachkommastellen.
 *
 * Zur Ausgabe darf lediglich die Funktion output_print verwendet werden. Direkte
 * Aufrufe von fprintf, fputs etc. sind somit verboten. Die Funktion output_print
 * darf hierbei pro Aufruf der Funktion generateLog nur ein einziges Mal aufgerufen 
 * werden. 
 *
 * @param[in] stream wird ignoriert
 * @param[in] polynom wird ignoriert
 * @param[in] a Der auszugebene Wert a. 
 * @param[in] b Der auszugebene Wert b.
 * @param[in] state Der Status. Steuert zudem, ob nur ein Wert (Ergebnis) oder beide ausgegeben werden.
 * @param[in] steps wird ignoriert
 */
void printer_generateLog (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps);


/**
 * Gibt den uebergebenen Wert als Teil einer HTML-Seite aus. Bei einem
 * STATE_RUNNING_FROM_THE_LEFT bzw. STATE_RUNNING_FROM_THE_RIGHT und steps = 0
 * wird zunaechst der Anfang der HTML-Seite, das Polynom und der Anfang einer
 * Tabelle ausgegeben. Der Wert wird in einer Tabellenzeile mit vier Zellen
 * ausgegeben, welche die Werte und ihre korrespondierenden Funktionswerte
 * enthalten. Bei einem STATE_COMPLETED, STATE_MAX_STEPS_REACHED oder 
 * STATE_DIV_BY_ZERO wird anschliessend das Tabellenende und das schliessende
 * HTML-Tag ausgegeben.
 *
 * Die Ausgaben der Zwischen- und Endergebnisse erfolgen stets mit einer
 * Genauigkeit von sechs Nachkommastellen.
 *
 * @param[in] stream Der zu nutzene Ausgabekanal.
 * @param[in] polynom Das Polynom.
 * @param[in] a Der auszugebene Wert a.
 * @param[in] b Der auszugebene Wert b.
 * @param[in] state Der Status. Steuert, ob der Wert normal ggfs mit Hinterlegung 
 *                  (Zwischenergebnis), gruen (erfolgreiches Endergebnis) oder 
 *                  rot (maximale Iterationsschritte erreicht oder Division durch Null)
 *                  ausgegeben werden soll.
 * @param[in] steps Anzahl der bisherigen Iterationsschritte.
 */
void printer_printHTML (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps);

/**
 * Gibt die uebergebenen Werte im menschenlesbaren Format aus.
 *
 * @param[in] stream Der zu nutzene Ausgabekanal.
 * @param[in] polynom Das Polynom.
 * @param[in] a Der auszugebene Wert a.
 * @param[in] b Der auszugebene Wert b.
 * @param[in] state Der Status. Steuert das Format der Ausgabe.
 * @param[in] steps Anzahl der bisherigen Iterationsschritte.
 */
void printer_printText (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps);

/**
 * Tut nichts.
 *
 * @param[in] stream wird ignoriert
 * @param[in] polynom wird ignoriert
 * @param[in] a wird ignoriert
 * @param[in] b wird ignoriert
 * @param[in] state wird ignoriert
 * @param[in] steps wird ignoriert
 */
void printer_printNothing (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps);

#endif
