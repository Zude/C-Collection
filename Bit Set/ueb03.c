/**
 * @file Hauptprogramm
 *
 * @author Andre Kloodt, Alexander Loeffler 
 */

#include <assert.h>
#include <stdio.h>

#include "error.h"
#include "set.h"

/**
 * Gibt die Hilfe in die uebergebene Datei beziehungsweise den uebergebenen Stream 
 * (stderr / stdout) aus.
 *
 * @param stream Datei bzw Stream, in den geschrieben werden soll 
 * @pre stream != NULL
 */              
void 
printUsage(FILE * stream) {

  assert (stream != NULL);

  fprintf(stream, "%s\n", "Usage:");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb03 -h");
  fprintf(stream, "%s\n", "  Show this help and exit.");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb03 ELEM_OP SET E");
  fprintf(stream, "%s\n", "  where ELEM_OP is one of:");
  fprintf(stream, "%s\n", "  e - check whether SET contains E or not");
  fprintf(stream, "%s\n", "  r - remove E from SET");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb03 UN_OP SET");
  fprintf(stream, "%s\n", "  where UN_OP is one of:");
  fprintf(stream, "%s\n", "  o - print SET");
  fprintf(stream, "%s\n", "  R - remove all elements from SET");
  fprintf(stream, "%s\n", "  E - Is SET empty?");
  fprintf(stream, "%s\n", "  c - copy SET");
  fprintf(stream, "%s\n", "  C - cardinality of SET");
  fprintf(stream, "%s\n", "  m - minimum of SET");
  fprintf(stream, "%s\n", "  M - maximum of SET");
  fprintf(stream, "%s\n", "  p - partition SET");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb03 BIN_OP SET1 SET2");
  fprintf(stream, "%s\n", "  where BIN_OP is one of:");
  fprintf(stream, "%s\n", "  s - SET2 subset of SET1?");
  fprintf(stream, "%s\n", "  = - SET1 equals SET2?");
  fprintf(stream, "%s\n", "  u - union between SET1 and SET2");
  fprintf(stream, "%s\n", "  i - intersection between SET1 and SET2");
  fprintf(stream, "%s\n", "  d - difference between SET1 and SET2 (SET1 \\ SET2)");
  fprintf(stream, "%s\n", "  D - symmetric difference of SET1 and SET2");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "SET, SET1, SET2 - one set of elements (e.g. {1,2,3})");
  fprintf(stream, "%s\n", "E   - one element (e.g. 1)");
  fprintf(stream, "%s\n", "All elements must be positive integer numbers (incl. 0).");
}

/**
 * Liest eine Menge aus der uebergebenen Zeichenkette ein.
 * 
 * Eine Menge besteht aus einer oeffnenden ({) und einer schliessenden (}) Klammer,
 * zwischen denen beliebig viele nicht-negative Ganzzahlen stehen. Zwei Ganzzahlen
 * sind dabei jeweils durch ein Komma (,) getrennt. Vor, nach, und zwischen
 * saemtlichen Zeichen (ausser zwischen den Ziffern einer Zahl) darf beliebig
 * viel Whitespace gemaess der C-Funktion isspace stehen. Zahlen werden gemaess
 * sscanf mit "%lu" gelesen.
 * 
 * Zahlen, die groesser als als SET_MAX_ELEM sind, fuehren zu dem Fehler 
 * ERR_INVALID_SET_ELEMENT beim Einlesen. Saemtliche anderen auftretenden Fehler 
 * beim Einlesen fuehren zu dem Rueckgabewert ERR_INVALID_SET_SYNTAX.
 * 
 * Beispiele:
 * "{1,2,3}"
 * " { 1 , 2 , 3 } "
 * 
 * @param string zu verarbeitende Zeichenkette
 * @param result Ergebnismenge
 * 
 * @pre result != NULL
 * @pre result zeigt auf eine leere Menge.
 * 
 * @return Fehlercode (ERR_NULL, wenn kein Fehler aufgetreten ist,
 *         ansonsten ERR_INVALID_SET_SYNTAX oder ERR_INVALID_SET_ELEMENT)
 */
Errorcode
readSet (char * string, Set * result) {
  char buffer = '\0';
  int marker = 0;
  unsigned long numbers = 0;

  assert(result != NULL);
  /** todo assert**/
  
  if(sscanf(string, " %c%n", &buffer, &marker) == 1 && buffer == '{'){
    string += marker;

    if(sscanf(string, " %c%n", &buffer, &marker)==1 && buffer == '}'){
          string += marker;

    }
    else{
    /** TODO  {zahl} checken **/
    while(sscanf(string, " %lu %c%n", &numbers, &buffer, &marker)==2 && buffer == ','){
      if(numbers <= SET_MAX_ELEM){
        string += marker;
        set_insert(result, numbers);
      }
      else{
        return ERR_INVALID_SET_ELEMENT;
      }
      
    }
    /** todo: syntax nach } noch pruefen**/
    if(buffer == '}'){
      if(numbers <= SET_MAX_ELEM){
        string += marker;
        set_insert(result, numbers);
      }
    }
    else{
      return ERR_INVALID_SET_SYNTAX;
    }
  }
}
else{
  return ERR_INVALID_SET_SYNTAX;
}

if(sscanf(string," %c", &buffer)==EOF){
  return ERR_NULL;
}
else{
  return ERR_INVALID_SET_SYNTAX;
}
return ERR_NULL;
}

/**
 * Hauptprogramm, ruft Kommandozeilenverarbeitung, Operationen und
 * Fehlerbehandlung auf.
 *
 * @param[in] argc Anzahl der Kommanzeilenargumente.
 * @param[in] argv Array mit Kommanzeilenargumenten.
 *
 * @return Fehlercode.
 */
int
main (int argc, char * argv[]) {
  Errorcode error = ERR_NULL;
  Set s = EMPTY_SET;
  char op = '\0';
  
  if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == '\0') {
    printUsage(stdout);
    
    return ERR_NULL;
  }
  
  if (argc < 3) {
    error = ERR_WRONG_ARG_COUNT;
  }
  
  if (!error) {
    /* Operator einlesen */
    char dummy = '\0';
    if (sscanf(argv[1], "%c%c", &op, &dummy) != 1) {
      error = ERR_INVALID_ARG;
    }
  }
  
  if (!error) {
    /* (erste) Menge einlesen */
    error = readSet(argv[2], &s);
  }
  
  if (!error) {
    switch (op) {
      /* Argumente: ELEM_OP SET E */
      case 'e':
      case 'r': { 
        Element e = 0;
        char dummy = '\0';
        
        if (argc != 4) {
          error = ERR_WRONG_ARG_COUNT;
        }
        
        /* Element einlesen */
        if (!error && sscanf(argv[3], "%lu%c", &e, &dummy) != 1) {
          error = ERR_INVALID_ELEMENT;
        }
        
        if (!error) {
          switch (op) {
            case 'e':
              printf("%s%s\n", set_contains(s, e) ? "" : "no ", "element of set");
              break;
            case 'r':
              set_remove(&s, e);
              set_print(stdout, s);
              fprintf(stdout, "\n");
              break;
          }
        }
        break;
      }
      /* Argumente: UN_OP SET */
      case 'o':
      case 'R':
      case 'E':
      case 'C':
      case 'M':
      case 'm':
      case 'c':
      case 'p': {
        if (argc != 3) {
          error = ERR_WRONG_ARG_COUNT;
        }
        
        if (!error) {
          switch (op) {
            case 'o':
              set_print(stdout, s);
              fprintf(stdout, "\n");
              break;
            case 'R':
              set_remove_all_elems(&s);
              set_print(stdout, s);
              fprintf(stdout, "\n");
              break;
            case 'E':
              fprintf(stdout, "%s%s\n", set_is_empty(s) ? "" : "not ", "empty");
              break;
            case 'C':
              fprintf(stdout, "%lu\n", set_cardinality(s));
              break;
            case 'M':
              fprintf(stdout, "%lu\n", set_max(s));
              break;
            case 'm':
              fprintf(stdout, "%lu\n", set_min(s));
              break;
            case 'c': {
              Set result = EMPTY_SET;
              set_copy(&result, s);
              set_print(stdout, result);
              fprintf(stdout, "\n");
              break;
            }
            case 'p': {
              Set res1 = EMPTY_SET, res2 = EMPTY_SET;
              set_partition(&res1, &res2, s);
              fprintf(stdout, "{");
              set_print(stdout, res1);
              fprintf(stdout, ",");
              set_print(stdout, res2);
              fprintf(stdout, "}\n");
              break;
            }
          }
        }
        break;
      }
      /* Argumente: BIN_OP SET1 SET2 */
      case 's':
      case '=':
      case 'u':
      case 'i':
      case 'd':
      case 'D': {
        Set s2 = EMPTY_SET;
        Set result = EMPTY_SET;
        
        if (argc != 4) {
          error = ERR_WRONG_ARG_COUNT;
        }
        
        if (!error) {
          /* zweite Menge einlesen */
          error = readSet(argv[3], &s2);
        }
        
        if (!error) {
          switch (op) {
            case 's':
              fprintf(stdout, "%s%s\n", set_is_subset(s, s2) ? "" : "no ", "subset");
              break;
            case '=':
              fprintf(stdout, "%s%s\n", set_is_subset(s, s2) ? "" : "not ", "equal");
              break;
            case 'u':
              set_union(&result, s, s2);
              set_print(stdout, result);
              fprintf(stdout, "\n");
              break;
            case 'i':
              set_intersection(&result, s, s2);
              set_print(stdout, result);
              fprintf(stdout, "\n");
              break;
            case 'd':
              set_difference(&result, s, s2);
              set_print(stdout, result);
              fprintf(stdout, "\n");
              break;
            case 'D':
              set_symmetric_difference(&result, s, s2);
              set_print(stdout, result);
              fprintf(stdout, "\n");
              break;
          }
        }
        break;
      }
      default:
        error = ERR_INVALID_ARG;
    }
  }
  
  if (error) {
    printError (stderr, error);
    printUsage (stderr);
  }
  
  return error;
}
