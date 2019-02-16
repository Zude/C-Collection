/**
 * @file output.c Implementierung eines Moduls welches Zeichenketten (Strings) 
 * verarbeiten kann. 
 *
 * Diese Datei darf nicht veraendert werden!
 *
 * @author Helga Karafiat, Malte Heins
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Referenzaehler fuer die gespeicherten Zeichenketten */
int string_refs = 0;

void 
output_print(char * s) {
  assert(s != NULL);

  fputs(s, stdout);
  fputs("\n", stdout);
  free(s);
  string_refs--;
}

