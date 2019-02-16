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
    case ERR_FILE_READ:
      fprintf(stream, format, "Could not read file.");
      break;
    case ERR_FILE_WRITE:
      fprintf(stream, format, "Could not write file.");
      break;
    case ERR_INVALID_ARG:
      fprintf(stream, format, "Invalid programm call.");
      break;
    case ERR_IMAGE_SIZE:
      fprintf(stream, format, "Invalid image size.");
      break;
    case ERR_PGM_INVALID_HEAD:
      fprintf(stream, format, "Invalid PGM Header.");
      break;
    case ERR_PGM_INVALID_DATA:
      fprintf(stream, format, "Invalid PGM Image Data.");
      break;
    case ERR_UNKNOWN:
    default:
      fprintf(stream, format, "An unknown error occurred.");
      break;
  }
}


 
