/** 
 * @file error.c 
 *
 * Ausgabe der entsprechenden Fehlermeldungen.
 *
 * @author kar, mhe
 */

#include <stdio.h>
#include "error.h"

void printError(FILE * stream, Errorcode error) {
  char * format = "Error: %s\n";
  switch(error) {
    case ERR_NULL: 
      fprintf(stream, format, "Should not see me! No error occurred.");
      break;
    case ERR_WRONG_ARG_COUNT:
      fprintf (stderr, "Error: Wrong argument count.\n");
      break;
    case ERR_INVALID_ARG:
      fprintf (stderr, "Error: Invalid argument.\n");
      break;
    case ERR_INVALID_SET_SYNTAX:
      fprintf (stderr, "Error: Invalid set syntax.\n");
      break;
    case ERR_INVALID_ELEMENT:
      fprintf (stderr, "Error: Wrong element syntax.\n");
      break;
    case ERR_MEMORY_LEAK:
      fprintf (stderr, "Error! Memory Leak :( !\n");
      break; 
    case ERR_OUT_OF_MEMORY:
      fprintf (stderr, "PANIC! Out of memory!\n");
      break; 
    case ERR_UNKNOWN:
    default:
      fprintf(stream, format, "An unknown error occurred.");
      break;
  }
}
