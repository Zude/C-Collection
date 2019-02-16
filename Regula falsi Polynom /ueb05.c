/**
 * @file Hauptprogramm
 *
 * Diese Datei darf nicht veraendert werden!
 *
 * @author Helga Karafiat, Malte Heins
 */

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "errors.h"
#include "polynom.h"
#include "printer.h"

extern int polynom_refs;
extern int string_refs;

/**
 * Gibt die Hilfe aus.
 * @param[in] stream Zeiger auf den Ausgabestrom.
 * @pre stream != NULL
 */
static void printUsage (FILE *stream);

/**
 * Liest ein Argument des Typs double ein.
 *
 * @param[in] argc Programmcounter
 * @param[in] argv Programmargumente
 * @param[in] index Stelle an der der Wert eingelesen werden soll
 * @param[out] arg eingelesener Wert
 *
 * @return Fehlercode siehe errors.h
 */
static Errorcode readDoubleArg (int argc, char **argv, unsigned int index, double *arg);

/**
 * Liest ein Argument des Typs unsigned int ein.
 *
 * @param[in] argc Programmcounter
 * @param[in] argv Programmargumente
 * @param[in] index Stelle an der der Wert eingelesen werden soll
 * @param[out] arg eingelesener Wert
 *
 * @return Fehlercode siehe errors.h
 */
static Errorcode readUIntArg (int argc, char **argv, unsigned int index, unsigned int *arg);


static void
printUsage (FILE *stream) {

  assert (stream != NULL);

  fprintf (stream, "Usage: ueb05 POLYNOM A0 B0 STEPS DISPLAY [M]\n");
  fprintf (stream, "             ->  compute a root of the POLYNOM\n");
  fprintf (stream, "                 A0 / B0: starting values\n");
  fprintf (stream, "                 STEPS: maximum number of iterations\n");
  fprintf (stream, "                 DISPLAY: one of\n");
  fprintf (stream, "                    p: display the simplified polynom\n");
  fprintf (stream, "                    r: print the calculated root\n");
  fprintf (stream, "                    h: output calculation as html\n");
  fprintf (stream, "                    o: output calculation in human-readable format\n");
  fprintf (stream, "                    l: output calculation as a log\n");
  fprintf (stream, "                 M: optional, when given activates memory management\n");
  fprintf (stream, "   or: ueb05 -h ->  print usage\n");
  fprintf (stream, "\n");
  fprintf (stream, "The POLYNOM is a sum of terms of the form ax^b where a is the coefficient\n");
  fprintf (stream, "and b is the exponent. Sequences of whitespace as accepted by the C standard\n");
  fprintf (stream, "function sscanf() are allowed in front of each a (where the sign belongs to\n");
  fprintf (stream, "the a, i.e. no whitespace is allowed between sign and number) and each b;\n");
  fprintf (stream, "apart from that no whitespace is permitted.\n");
  fprintf (stream, "e.g.: \"4.5x^2+2.0x^1+7.12x^2-9.0098x^0\"\n");
  fprintf (stream, "e.g.: \" 4.0x^ 2 +2.0x^ 1\"\n");
}

static Errorcode
readDoubleArg (int argc, char **argv, unsigned int index, double *arg) {
  char
      check = '\0'
    ;

  if ( (int) index >= argc ) {
    return ERR_WRONG_ARG;

  } else if ( sscanf (argv[index], "%lf%c", arg, &check ) != 1) {
    return ERR_NODOUBLE;

  } else {
    return ERR_NULL;
  }
}

static Errorcode
readUIntArg (int argc, char **argv, unsigned int index, unsigned int *arg) {
  char
      check = '\0'
    ;

  if ( (int) index >= argc ) {
    return ERR_WRONG_ARG;

  } else if (sscanf (argv[index], "%u%c", arg, &check) != 1) {
    return ERR_NOUINT;

  } else {
    return ERR_NULL;
  }
}

/**
 * Hauptprogramm.
 *
 * @param[in] argc Programmcounter
 * @param[in] argv Programmargumente
 *
 * @return Fehlercode siehe errors.h
 */
int
main (int argc, char * argv []) {

  Errorcode
      error = ERR_NULL
    ;

  unsigned int
      maxSteps = 0
    ;

  char
      option  = '\0'
    , display = '\0'
    , check   = '\0'
    ;

  double
      a_0  = 0.0
    , b_0  = 0.0
    , root = 0.0
    ;

  int
      memory_management = 0
    ;


  Polynom
      polynom = INVALID_POLYNOM;
    ;

  if ( argc == 6  || argc == 7 ) {

    /* Memory Management? */
    if ( argc == 7 ) {
      if ( sscanf(argv[6], "%c%c", &option, &check) == 1 && option == 'M' ) {
        memory_management = 1;
      } else {
          error = ERR_WRONG_ARG;
      }
    }

    /* Startwerte A0 einlesen */
    if ( ! error ) {
      error = readDoubleArg (argc, argv, 2, &a_0);
    }
    
    /* Startwerte B0 einlesen */
    if ( ! error ) {
      error = readDoubleArg (argc, argv, 3, &b_0);
    }

    if ( ! error ) {
      /* Polynom parsen */
      polynom = polynom_parse (argv[1], &error);
    }

    if ( ! error ) {
      /* MaxSteps einlesen */
      error = readUIntArg (argc, argv, 4, &maxSteps);
    }
    
    if ( ! error ) {
      double f_a0 = polynom_evaluate (polynom, a_0);
      double f_b0 = polynom_evaluate (polynom, b_0);
      /* Vorzeichen muessen sich unterscheiden (hier ausreichend genau) */
      if (f_a0 * f_b0 >= 0) {
        error = ERR_INVALID_STARTVALUES;
      }
    }

    if ( ! error ) {
      /* Ausgabe */
      if ( sscanf(argv[5], "%c%c", &display, &check) == 1) {
        switch ( display ) {
          case 'p':
            polynom_print( stdout, polynom );
            break;
          case 'r':
            error = polynom_computeRoot(polynom, a_0, b_0, maxSteps, &root, printer_printNothing, stdout);
            if ( ! error ) printf("Root %f\n", root);
            break;
          case 'h':
            error = polynom_computeRoot(polynom, a_0, b_0, maxSteps, &root, printer_printHTML, stdout);
            break;
          case 'o':
            error = polynom_computeRoot(polynom, a_0, b_0, maxSteps, &root, printer_printText, stdout);
            break;
          case 'l':
            error = polynom_computeRoot(polynom, a_0, b_0, maxSteps, &root, printer_generateLog, stdout);
            break;            
          default:
            error = ERR_WRONG_ARG;
            break;
        }
      }
    }

  } else if ( argc == 2 && sscanf(argv[1], "-%c%c", &option, &check) == 1 && option == 'h' ) {
     printUsage( stdout );
  } else {
    error = ERR_WRONG_ARG_CNT;
  }

  /* Speicherfreigabe */
  if ( polynom != NULL ) polynom_clear(polynom);

  /* Speichermanagement Ausgaben */
  if ( memory_management ) {
    if ( polynom_refs != 0 || string_refs != 0) {
      error = ERR_MEMORY_LEAK;
    }
    fprintf( stderr, "PolynomRefs: %d\n", polynom_refs );
    fprintf( stderr, "StringRefs: %d\n", string_refs );    
  }

  /* Fehlerausgabe */
  if ( error ) {
      printError (error);
      fprintf (stderr, "\n");
      printUsage (stderr);
  }

  return error;
}
