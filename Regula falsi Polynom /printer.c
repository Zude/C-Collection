/**
 * @file printer.c Implementierung eines Moduls mit Ausgabefunktionen.
 *
 * @author Andre Kloodt, Alexander Loeffler
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "printer.h"
#include "output.h"

extern int string_refs;

void printer_generateLog (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps) {

double aTemp = a;
double bTemp = b;

if(fabs(a) <__DBL_EPSILON__){
    aTemp = 1;
}
if(fabs(b) <__DBL_EPSILON__){
    bTemp = 1;
}
if(state == STATE_COMPLETED_RESULT){
    char *tempString = malloc(sizeof(state_getState(state)) + floor(log10(fabs(aTemp)))+1 + 9);
    /** Die 8 ist eine Kombination aus (1)Vorzeichen + (1)Leerzeichen + (6)Nachkommastellen + (1)Terminalsymbol */
    
    if(tempString == NULL){
        exit(ERR_OUT_OF_MEMORY);
    }

    tempString[0] = '\0';
    string_refs++;
    
    strcat(tempString, state_getState(state));
    sprintf(tempString + strlen(state_getState(state)) ," %.6f",a);
    
    output_print(tempString);
} else if (state == STATE_DIV_BY_ZERO){
    char *tempString = malloc(sizeof(state_getState(state)));
    /** Legt den Speicher nur fuer den Status fest */

    if(tempString == NULL){
        exit(ERR_OUT_OF_MEMORY);
    }

    tempString[0] = '\0';
    string_refs++;

    strcat(tempString, state_getState(state));
    output_print(tempString);
} else {
    char *tempString = malloc(sizeof(state_getState(state)) + floor(log10(fabs(aTemp)))+1 + floor(log10(fabs(bTemp)))+1 + 17);
    /** Die 17 ist eine Kombination aus (2)Vorzeichen + (2)Leerzeichen + (12)Nachkommastellen + (1)Terminierugssymbol */

    
    if(tempString == NULL){
        exit(ERR_OUT_OF_MEMORY);
    }

    tempString[0] = '\0';
    string_refs++;

    /** Zusammensetzten des Gesamtstrings */
    strcat(tempString, state_getState(state));
    sprintf(tempString + strlen(state_getState(state)) ," %.6f %.6f",a,b);

    output_print(tempString);
}
}





void printer_printHTML (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps) {

double tempA = polynom_evaluate(polynom,a);
double tempB = polynom_evaluate(polynom,b);

/** Wird einmal am Start ausgefuehrt */
if((state == STATE_RUNNING_FROM_THE_LEFT || state == STATE_RUNNING_FROM_THE_RIGHT) && steps == 0){
    fprintf(stream,"<html>\n<head><title>Regula Falsi</title></head>\n<body>\n");
    polynom_print(stream,polynom);
    fprintf(stream,"<table>\n<tr><td>%.6f</td> <td>%.6f</td> <td style='background-color:LightGray;'>%.6f</td> <td style='background-color:LightGray;'>%.6f</td></tr>\n",a,tempA,b,tempB);
} else if(state == STATE_COMPLETED_LAST_STEP && steps == 0){
    fprintf(stream,"<html>\n<head><title>Regula Falsi</title></head>\n<body>\n");
    polynom_print(stream,polynom);
    fprintf(stream,"<table>\n<tr><td>%.6f</td> ",a);
    fprintf(stream,"<td>%.6f</td> ",tempA);
    fprintf(stream,"<td>%.6f</td> ",b);
    fprintf(stream,"<td>%.6f</td></tr> \n",tempB);
} else {

switch (state) {
    case STATE_COMPLETED_RESULT: /** Das Ergebnis wird ausgegeben */
        fprintf(stream,"<tr style='color:green;'><td colspan=4> %.6f </td></tr>\n",a);
        fprintf(stream,"</table>\n</body>\n</html>\n");
        break;
    case STATE_MAX_STEPS_REACHED: /** Die maximale Anzahl an Schritten ist erreicht */
        fprintf(stream,"<tr style='color:red;'><td>%.6f</td> <td>%.6f</td> <td>%.6f</td> <td>%.6f</td></tr>\n",a,tempA,b,tempB);
        fprintf(stream,"</table>\n</body>\n</html>\n");
        break;
    case STATE_DIV_BY_ZERO: /** Div durch 0 nicht moeglich */
        fprintf(stream,"<tr style='color:red;'><td>%.6f</td> <td>%.6f</td> <td>%.6f</td> <td>%.6f</td></tr>\n",a,tempA,b,tempB);
        fprintf(stream,"</table>\n</body>\n</html>\n");
        break;
    case STATE_COMPLETED_LAST_STEP: /** Der letzte Schritt wird ausgefuehrt */
        fprintf(stream,"<tr><td>%.6f</td> ",a);
        fprintf(stream,"<td>%.6f</td> ",tempA);
        fprintf(stream,"<td>%.6f</td> ",b);
        fprintf(stream,"<td>%.6f</td></tr> \n",tempB);
        break;
    case STATE_RUNNING_FROM_THE_RIGHT: /** Obergrenze wird bearbeitet */
        fprintf(stream,"<tr><td>%.6f</td> ",a);
        fprintf(stream,"<td>%.6f</td> ",tempA);
        fprintf(stream,"<td style='background-color:LightGray;'>%.6f</td> ",b);
        fprintf(stream,"<td style='background-color:LightGray;'>%.6f</td></tr> \n",tempB); 
        break;
    case STATE_RUNNING_FROM_THE_LEFT: /** Untergrenze wird bearbeitet */
        fprintf(stream,"<tr><td style='background-color:LightGray;'>%.6f</td> ",a);
        fprintf(stream,"<td style='background-color:LightGray;'>%.6f</td> ",tempA); 
        fprintf(stream,"<td>%.6f</td> ",b);
        fprintf(stream,"<td>%.6f</td></tr> \n",tempB);
        break;
    default:
        break;
}
}
}

void printer_printText (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps) {

/**  Wird nur einmal am Anfang ausgefuehrt */
if((state == STATE_RUNNING_FROM_THE_LEFT || state == STATE_RUNNING_FROM_THE_RIGHT) && steps == 0){
    fprintf(stream,"Die Regula Falsi Berechnung\n");
    fprintf(stream,"Das von Ihnen gegebene Polynom\n");
    polynom_print(stream,polynom);
    fprintf(stream,"Startwert fuer die Untergrenze: %.6f\n",a);
    fprintf(stream,"Startwert fuer die Obergrenze: %.6f\n",b);
} else {

switch (state) {
    case STATE_COMPLETED_RESULT: /** Das Ergebnis wird ausgegeben */
        fprintf(stream,"Dies ist das Ergebnis fuer die Nullstelle: %.6f\n",a);
        fprintf(stream,"Aktuelle Schrittzahl betraegt: %u",steps);
        break;
    case STATE_MAX_STEPS_REACHED: /** Die maximale Anzahl an Schritten ist erreicht */
        fprintf(stream,"%.6f %.6f\n",a,b);
        fprintf(stream,"Sie haben Ihre maximale Schrittzahl erreicht\n");
        break;
    case STATE_DIV_BY_ZERO: /** Div durch 0 nicht moeglich */
        fprintf(stream,"Eine Division durch Null ist nicht moeglich\n");
        break;
    case STATE_COMPLETED_LAST_STEP: /** Der letzte Schritt wird ausgefuehrt */
        fprintf(stream,"Dies sind die Werte vom letzten Schritt: %.6f %.6f\n",a,b);
        break;
    case STATE_RUNNING_FROM_THE_RIGHT: /** Obergrenze wird bearbeitet */
        fprintf(stream,"Zwischenwert der Untergrenze: %.6f ",a);
        fprintf(stream,"Veraenderter Zwischenwert der Obergrenze: %.6f\n",b); 
        fprintf(stream,"Aktuelle Schrittzahl betraegt: %u",steps);
        break;
    case STATE_RUNNING_FROM_THE_LEFT: /** Untergrenze wird bearbeitet */
        fprintf(stream,"Veraenderter Zwischenwert der Untergrenze: %.6f ",a); 
        fprintf(stream,"Zwischenwert der Obergrenze: %.6f\n",b);
        fprintf(stream,"Aktuelle Schrittzahl betraegt: %u",steps);
        break;
    default:
        break;
}
}
}


void printer_printNothing (FILE * stream, Polynom polynom, double a, double b, State state, unsigned int steps) {

}

