/**
 * @file Hauptprogramm
 *
 * @author kar, mhe
 */

#include <assert.h>
#include <stdio.h>

#include "error.h"
#include "set.h"

extern int setlist_refs;

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
  fprintf(stream, "%s\n", "ueb04 -h");
  fprintf(stream, "%s\n", "  Show this help and exit.");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb04 ELEM_OP SET E [-m]");
  fprintf(stream, "%s\n", "  where ELEM_OP is one of:");
  fprintf(stream, "%s\n", "  e - check whether SET contains E or not");
  fprintf(stream, "%s\n", "  r - remove E from SET");
  fprintf(stream, "%s\n", "  I - insert E into SET"); /* NEU */ 
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb04 UN_OP SET [-m]");
  fprintf(stream, "%s\n", "  where UN_OP is one of:");
  fprintf(stream, "%s\n", "  o - print SET");
  fprintf(stream, "%s\n", "  R - remove all elements from SET");
  fprintf(stream, "%s\n", "  c - copy SET"); 
  fprintf(stream, "%s\n", "  C - cardinality of SET"); 
  fprintf(stream, "%s\n", "  m - minimum of SET");
  fprintf(stream, "%s\n", "  M - maximum of SET");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "ueb04 BIN_OP SET1 SET2 [-m]");
  fprintf(stream, "%s\n", "  where BIN_OP is one of:");
  fprintf(stream, "%s\n", "  s - SET1 subset of SET2?");
  fprintf(stream, "%s\n", "  = - SET1 equals SET2?");
  fprintf(stream, "%s\n", "  u - union between SET1 and SET2");
  fprintf(stream, "%s\n", "  i - intersection between SET1 and SET2");
  fprintf(stream, "%s\n", "  d - difference between SET1 and SET2 (SET1 \\ SET2)");
  fprintf(stream, "%s\n", "  D - symmetric difference of SET1 and SET2");
  fprintf(stream, "%s\n", "");
  fprintf(stream, "%s\n", "SET, SET1, SET2 - one set of elements (e.g. {1,2,3})");
  fprintf(stream, "%s\n", "E - one element (e.g. 1)");
  fprintf(stream, "%s\n", "All elements must be integers"); /* Angepasst */
  fprintf(stream, "%s\n", "-m - activates memory management (optional)"); /* NEU */
}

/**
 * Liest eine Menge aus der uebergebenen Zeichenkette ein.
 * 
 * Eine Menge besteht aus einer oeffnenden ({) und einer schliessenden (}) Klammer,
 * zwischen denen beliebig viele Ganzzahlen stehen. Zwei Ganzzahlen sind dabei jeweils 
 * durch ein Komma (,) getrennt. Vor, nach, und zwischen saemtlichen Zeichen (ausser 
 * zwischen den Ziffern einer Zahl) darf beliebig viel Whitespace gemaess der C-Funktion
 * isspace stehen. Zahlen werden gemaess sscanf mit "%ld" gelesen.
 * 
 * Saemtliche auftretenden Fehler beim Einlesen fuehren zu dem Rueckgabewert 
 * ERR_INVALID_SET_SYNTAX.
 * 
 * Beispiele:
 * "{1,2,3}"
 * " { 1 , 2 , 3 } "
 * 
 * @param[in]     string zu verarbeitende Zeichenkette
 * @param[in,out] error Zeiger auf den Fehlercode
 * 
 * @pre string != NULL
 * @pre error != NULL
 * @pre *error == ERR_NULL
 * 
 * @return Fehlercode (ERR_NULL, wenn kein Fehler aufgetreten ist,
 *         ansonsten ERR_INVALID_SET_SYNTAX)
 */
Set
readSet (char * string, Errorcode * error) {
  assert (string != NULL);
  assert (error != NULL); 
  assert (*error == ERR_NULL); 

  {
    Set
        result = EMPTY_SET
      ;
    char 
        delimiter = '\0'
      , d = '\0'
      ;
    int 
        num = 0
      ;
  
    /* { ueberlesen */
    if (sscanf(string, " %c%n", &delimiter, &num) == 1 && delimiter == '{') {
      string += num;
      delimiter = ',';
    } else {
      *error = ERR_INVALID_SET_SYNTAX;
    }
  
    /* } ueberlesen */
    if (! (*error) && sscanf(string, " %c%n", &d, &num) == 1 && d == '}') {
      string += num;
      delimiter = '}';
    }

    while (! (*error) && delimiter == ',') {
      Element e = 0;
    
      /* Element einlesen */
      if (sscanf(string, "%ld%n", &e, &num) != 1) {
        *error = ERR_INVALID_SET_SYNTAX;
      }
      string += num;
   
      if (! (*error)) {
        
        result = set_insert (result, e);
      
        /* Trennzeichen (, oder }) ueberlesen */
        if (sscanf(string, " %c%n", &delimiter, &num) != 1) {
          *error = ERR_INVALID_SET_SYNTAX;
        }
        string += num;
      }
    }
  
    if (! (*error) && (delimiter != '}' || sscanf(string, " %c", &d) == 1)) {
      *error = ERR_INVALID_SET_SYNTAX;
    }
  
    return result;
  }
}

/**
 * Gibt das uebergebene Set als Menge auf stdout und als Repraesentation der
 * internen Liste auf stderr.
 *
 * @param[in] s das Set
 */
void 
printSet(Set s) {
  
  set_print(stdout, s);
  fprintf(stdout, "\n");

  set_print_list(stderr, s); 
  fprintf(stderr, "\n");
}

/**
 * Hauptprogramm, ruft Kommandozeilenverarbeitung, Operationen und
 * Fehlerbehandlung auf.
 *
 * @param[in] argc Anzahl der Kommandozeilenargumente.
 * @param[in] argv Array mit Kommandozeilenargumenten.
 *
 * @return Fehlercode.
 */
int
main (int argc, char * argv[]) {

  Errorcode 
      error = ERR_NULL
    ;
  Set 
      s = EMPTY_SET
    ;
  char 
      op = '\0'
    , arg = '\0'  
    , dummy = '\0' 
    ;
  int 
      memory_management = 0
    ;
 
  if (argc == 2 && (sscanf(argv[1], "-%c%c", &arg, &dummy) == 1) && arg == 'h') {    
    printUsage(stdout);
    return ERR_NULL;
  }
  
  if (argc < 3) {
    error = ERR_WRONG_ARG_COUNT;
  }
  
  if (!error) {
    /* Operator einlesen */
    if (sscanf(argv[1], "%c%c", &op, &dummy) != 1) {
      error = ERR_INVALID_ARG;
    }
  }
  
  if (!error) {
    /* (erste) Menge einlesen */
    s = readSet(argv[2], &error);
  }
  
  if (!error) {
    switch (op) {
      /* Argumente: ELEM_OP SET E */
      case 'e':
      case 'r':
      case 'I': { 

        Element 
            e = 0
          ;
        
        if (argc < 4 || argc > 5) { 
          error = ERR_WRONG_ARG_COUNT;
        } else if (argc == 5 && ((sscanf(argv[4], "-%c%c", &arg, &dummy) == 1) && arg == 'm')) { 
          memory_management = 1;
        }
        
        /* Element einlesen */
        if (!error && sscanf(argv[3], "%ld%c", &e, &dummy) != 1) {
          error = ERR_INVALID_ELEMENT;
        }
        
        if (!error) {
          switch (op) {
            case 'e':
              printf("%s%s\n", set_contains(s, e) ? "" : "no ", "element of set");
              break;
            case 'r':
              s = set_remove(s, e);
              printSet(s);
              break;
            case 'I':
              s = set_insert(s, e);
              printSet(s);
              break;
          }

        }
        break;
      }
      /* Argumente: UN_OP SET */
      case 'o':
      case 'O':                
      case 'R':
      case 'C':
      case 'M': 
      case 'm': 
      case 'c': {

        if (argc < 3 || argc > 4) { 
          error = ERR_WRONG_ARG_COUNT;
        } else if (argc == 4 && ((sscanf(argv[3], "-%c%c", &arg, &dummy) == 1) && arg == 'm')) { 
          memory_management = 1;
        }

        if (!error) {
          switch (op) {
            case 'o':
              printSet(s);
              break;
            case 'R':
              s = set_clear(s);
              printSet(s);
              break;
            case 'C':
              fprintf(stdout, "%lu\n", set_cardinality(s));
              break;
            case 'M':
              if (! SET_IS_EMPTY(s)) {
                fprintf(stdout, "%ld\n", set_max(s));
              } else {
                fprintf(stdout, "no maximum for empty list");
              }
              break;
            case 'm':
              if (! SET_IS_EMPTY(s)) {
                fprintf(stdout, "%ld\n", set_min(s));
              } else {
                fprintf(stdout, "no minimum for empty list");
              }
              break;
            case 'c': {
              Set result = set_copy(s);
              printSet(s);              
              result = set_clear(result);
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

        Set 
            s2 = EMPTY_SET
          , result = EMPTY_SET
          ;
        
        if (argc < 4 || argc > 5) { 
          error = ERR_WRONG_ARG_COUNT;
        } else if (argc == 5 && ((sscanf(argv[4], "-%c%c", &arg, &dummy) == 1) && arg == 'm')) {
           memory_management = 1;
        }
        
        if (!error) {
          /* zweite Menge einlesen */
          s2 = readSet(argv[3], &error);
        }
        
        if (!error) {
          switch (op) {
            case 's':
              fprintf(stdout, "%s%s\n", set_is_subset(s, s2) ? "" : "no ", "subset");
              break;
            case '=':
              fprintf(stdout, "%s%s\n", set_equals(s, s2) ? "" : "not ", "equal");
              break;
            case 'u':
              result = set_union(s, s2);
              printSet(result);
              break;
            case 'i':
              result = set_intersection(s, s2);
              printSet(result);
              break;
            case 'd':
              result = set_difference(s, s2);
              printSet(result);
              break;
            case 'D':
              result = set_symmetric_difference(s, s2);
              printSet(result);
              break;
          }
        }
        s2 = set_clear(s2);
        result = set_clear(result);
      }
      break;          
      default:
        error = ERR_INVALID_ARG;
    }
  }

  s = set_clear(s);

  if (memory_management) {
    if (setlist_refs != 0) {
      error = ERR_MEMORY_LEAK;
    }
    fprintf(stderr, "SetListRefs: %d\n", setlist_refs);
  }

  
  if (error) {
    printError (stderr, error);
    printUsage (stderr);
  }
  
  return error;
}
