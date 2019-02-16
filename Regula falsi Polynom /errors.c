/**
 * @file errors.c Implementierung eines Moduls zur Fehlerbehandlung.
 *
 * Diese Datei darf nicht veraendert werden!
 *
 * @author Helga Karafiat, Malte Heins
 */

#include <stdio.h>

#include "errors.h"

Errorcode
printError (Errorcode error)
{
  switch (error)
    {
    /* Kein Fehler */
    case ERR_NULL:
      break;
    case ERR_WRONG_ARG_CNT:
      fprintf (stderr, "Error: wrong arg count.\n");
      break;
    case ERR_SYNTAX_ERROR:
      fprintf (stderr, "Error: wrong polynom syntax.\n");
      break;
    case ERR_WRONG_ARG:
      fprintf (stderr, "Error: missing or wrong argument.\n");
      break;
    case ERR_NODOUBLE:
      fprintf (stderr, "Error: not a double.\n");
      break;
    case ERR_NOUINT:
      fprintf (stderr, "Error: not an unsigned int.\n");
      break;
    case ERR_INVALID_STARTVALUES:
      fprintf (stderr, "Error: Invalid start values.\n");
      break;
    case ERR_MAX_STEPS_REACHED:
      fprintf (stderr, "Error: Max iteration steps reached.\n");
      break;
    case ERR_DIVISION_BY_ZERO:
      fprintf (stderr, "Error: Division by zero.\n");
      break;
    case ERR_MEMORY_LEAK:
      fprintf (stderr, "Error! Memory Leak :( !\n");
      break;
    case ERR_OUT_OF_MEMORY:
      fprintf (stderr, "PANIC! Out of memory!\n");
      break;
    default:
      fprintf (stderr, "Error: Should not see me.\n");
      break;
  }
  return error;
}

