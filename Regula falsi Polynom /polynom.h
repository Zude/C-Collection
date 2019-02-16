#ifndef __POLYNOM_H__
#define __POLYNOM_H__

/**
 * @file polynom.h Schnittstelle eines Moduls mit Polynomfunktionen.
 *
 * @author Alexander Loeffler, Andre Kloodt
 */

#include <stdio.h>

#include "errors.h"
#include "state.h"

/**
 * Wert, ab dem das Regula-Falsi-Verfahren zwei Werte als gleich betrachten soll.
 */
#ifndef POLYNOM_EPSILON
#define POLYNOM_EPSILON 1.0e-10
#endif

#define INVALID_POLYNOM NULL


/* TODO Datenstruktur definieren */
struct PolynomStruct {double *koeff; int max;};

/** Typdefinition fuer ein Polynom als Zeiger auf einen Strukturwert */
typedef struct PolynomStruct *Polynom;


/** Funktionszeiger fuer eine Ausgabefunktion */
typedef void (*Printer)(FILE *, Polynom, double, double, State, unsigned int);


/**
 * Gibt den von diesem Polynom belegten Speicher wieder frei.
 *
 * @return INVALID_POLYNOM
 */
Polynom polynom_clear(Polynom p);

/**
 * Wandelt eine Zeichenkette, die eine nicht-leere Folge von Termen der Form ax^b enthaelt,
 * in ein Polynom um.
 * Die Zeichenkette darf Whitespace vor jedem Koeffizienten a und vor jedem
 * Exponenten b enthalten (wobei das jeweilige Vorzeichen zum Koeffizienten
 * gezaehlt wird, d.h. zwischen Vorzeichen und Zahl ist kein Whitespace zulaessig)
 * und muss das Polynom als Summe von Gliedern der Form ax^b darstellen.
 * a ist der Koeffizient aus dem Bereich der reelen Zahlen
 * b ist der ganzzahlige positive Exponent (>= 0)
 *
 * @param[in] str Zeichenkette, die umgewandelt werden soll
 * @param[out] error ERR_SYNTAX_ERROR wenn die Zeichenkette leer ist oder anderweitig
 * nicht der Syntax entspricht, sonst ERR_NULL
 *
 * @pre str ist nicht der Nullzeiger
 * @pre error ist nicht der Nullzeiger
 *
 * @return INVALID_POLYNOM, wenn die Zeichenkette leer ist oder anderweitig
 * nicht der Syntax entspricht, sonst das erzeugte Polynom
 */
Polynom polynom_parse (char * str, Errorcode * error);

/**
 * Berechnung einer Nullstelle des Polynoms p nach dem
 * Regular-Falsi-Verfahren.
 *
 * @param[in] p Polynom, von dem eine Nullstelle berechnet werden soll.
 * @param[in] ak Start-Wert ak fuer den Iterationsschritt
 * @param[in] bk Start-Wert bk fuer den Iterationsschritt
 * @param[in] maxSteps maximale Anzahl von Iterationsschritten; 
 *            bei maxSteps = 0 wird die Iteration nicht ausgefuehrt
 *            und der Fehler ERR_MAX_STEPS_REACHED zurueckgeliefert
 * @param[out] root berechnete Nullstelle
 * @param[in] print Zeiger auf eine Ausgabefunktion
 * @param[in] stream Stream, in den bei der Ausgabe geschrieben werden soll
 * @pre p != NULL
 * @pre maxSteps >= 0
 * @pre root != NULL
 *
 * @return Fehlercode
 *         ERR_NULL - Berechnung war erfolgreich.
 *         ERR_MAX_STEPS_REACHED - Die maximale Anzahl an Iterationsschritten wurde erreicht.
 *         ERR_DIVISION_BY_ZERO - Waehrend der Iteration wuerde durch 0 geteilt werden.
 */
Errorcode polynom_computeRoot (Polynom p, double ak, double bk, unsigned int maxSteps, 
                               double * root, Printer print, FILE * stream);

/**
 * Berechnet den Funktionswert des Polynoms p an der Stelle x.
 *
 * @param[in] p Polynom, dessen Funktionswert berechnet werden soll
 * @param[in] x Stelle, an der der Funktionswert berechnet werden soll
 *
 * @pre p != INVALID_POLYNOM
 *
 * @return Funktionswert des Polynoms p an der Stelle x
 */
double polynom_evaluate (Polynom p, double x);

/**
 * Gibt das Polynom p (in vereinfachter Form, siehe Aufgabenstellung) aus.
 * Die Ausgaben erfolgt mit einer Genauigkeit von drei Nachkommastellen.
 *
 * @param[in] stream Dateistream in den geschrieben werden soll
 * @param[in] p Polynom, das ausgegeben werden soll
 *
 * @pre stream != NULL
 * @pre p != INVALID_POLYNOM
 */
void polynom_print (FILE * stream, Polynom p);

#endif
