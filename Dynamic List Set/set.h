#ifndef __SET_H__
#define __SET_H__

/**
 * @file set.h Schnittstelle einer Bibliothek fuer Mengenoperationen.
 *
 * Die Mengen werden als Intervall-Listen dargestellt, die stets (d.h. vor und nach der
 * Ausfuehrung einer Operation) die folgenden Invarianten erfuellen:
 * - Ein Intervall umfasst immer mindestens eine Zahl (Intervallende >= Intervallanfang)
 * - Intervalle sind stets aufsteigend sortiert, d.h. soweit ein Intervall ein 
 *   Vorgaengerintervall hat, muss sein Anfang nach dem Ende des Vorgaengerintervalls
 *   liegen. Intervalle duerfen sich hierbei also weder ueberschneiden noch direkt 
 *   aneinander angrenzen. (Intervallanfang > Ende Vorgaengerintervall +1)
 * 
 * Die Verwaltung des benoetigten Speichers findet komplett innerhalb dieses Moduls statt.
 *
 * @author alexander loeffler, andre kloodt
 */

/** Datentyp der Mengenelemente */
typedef long Element; 

/* TODO Typdefinition Set als Intervall-Liste */
typedef struct Intervall *Set;
struct Intervall { Element start; Element end; Set next; };

/** Leere Menge zur Initialisierung von Variablen. */
#define EMPTY_SET NULL

/** Makro fuer Pruefung ob das Set leer ist TODO */
#define SET_IS_EMPTY(SET) (SET==EMPTY_SET)


/**
 * Fuegt ein Element e in der Menge s ein. 
 *
 * Die Menge s wird hierbei veraendert.
 *
 * @param[in,out] s die Menge.
 * @param[in]     e das einzufuegende Element.
 *
 * @return die Menge s in der das Element e eingefuegt wurde.
 *
 * @post die Menge enthaelt das Element e.
 */
Set set_insert (Set s, Element e);

/**
 * Entfernt ein Element e aus der Menge s. 
 *
 * Die Menge s wird hierbei veraendert.
 *
 * @param[in,out] s die Menge.
 * @param[in] e     das zu loeschende Element.
 *
 * @return die Menge s aus der das Element e geloescht wurde.
 *
 * @post das Element e ist nicht in der Menge enthalten.
 */
Set set_remove (Set s, Element e);

/**
 * Entfernt alle Elemente aus der Menge s. 
 *
 * Die Menge s wird hierbei veraendert.
 *
 * @param[in,out] s die Menge.
 * 
 * @return die Menge aus der alle Elemente geloescht wurden.
 *
 * @post s ist die leere Menge.
 */
Set set_clear (Set s);

/**
 * Liefert die Kardinalitaet der Menge s (Anzahl der Elemente).
 *
 * Die Menge s bleibt hierbei unveraendert.
 *
 * @param[in] s die Menge.
 *
 * @return Kardinalitaet der Menge.
 */
unsigned long set_cardinality (Set s);


/**
 * Liefert das groesste Element der Menge s.
 *
 * Die Menge s bleibt hierbei unveraendert.
 * 
 * @param[in] s die Menge.
 *
 * @return groesstes Element der Menge.
 *
 * @pre s ist nicht leer.
 */
Element set_max (Set s);

/**
 * Liefert das kleinste Element der Menge s.
 *
 * Die Menge s bleibt hierbei unveraendert.
 *
 * @param[in] s die Menge.
 *
 * @return kleinstes Element der Menge.
 *
 * @pre s ist nicht leer.
 */
Element set_min (Set s);

/**
 * Prueft, ob ein Element in einer Menge enthalten ist.
 *
 * Die Menge s bleibt hierbei unveraendert.
 *
 * @param[in] s die Menge.
 * @param[in] e das zu pruefende Element.
 *
 * @return 1, falls Element enthalten ist, sonst 0.
 */
int set_contains (Set s, Element e);

/**
 * Prueft, ob die Menge a Untermenge der Menge b ist, also alle Elemente von a
 * auch in b enthalten sind.
 *
 * Die beiden Mengen a und b bleiben hierbei unveraendert.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a Untermenge von b ist, sonst 0.
 */
int set_is_subset (Set a, Set b);

/**
 * Prueft, ob die Menge a gleich der Menge b ist, also ob beide Mengen die selben 
 * Elemente enthalten.
 *
 * Die beiden Mengen a und b bleiben hierbei unveraendert.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a gleich Menge b ist, sonst 0.
 */
int set_equals (Set a, Set b);

/**
 * Erzeugt die Vereinigungsmenge der beiden Mengen a und b. Dabei bilden
 * alle Elemente, die in a oder b enthalten sind, eine neue Menge, die 
 * zurueckgegeben wird.
 *
 * Die beiden Mengen a und b bleiben hierbei unveraendert.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return die Vereinigungsmenge der Mengen a und b.
 */
Set set_union (Set a, Set b);

/**
 * Erzeugt die Schnittmenge zweier Mengen a und b. Dabei bilden
 * die Elemente, die sowohl in a als auch in b enthalten sind, die 
 * neue Menge, die zurueckgegeben wird.
 *
 * Die beiden Mengen a und b bleiben hierbei unveraendert.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return die Schnittmenge der Mengen a und b.
 */
Set set_intersection (Set a, Set b);

/**
 * Erzeugt die Differenzmenge der beiden Mengen a und b. Dabei bilden
 * die Elemente aus a, die nicht in b enthalten sind, die neue Menge, 
 * die zurueckgegeben wird.
 *
 * Die beiden Mengen a und b bleiben hierbei unveraendert.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return die Differenzmenge der Mengen a und b.
 */
Set set_difference (Set a, Set b);

/**
 * Erzeugt die symmetrische Differenzmenge der beiden Mengen a und b. Dabei
 * bilden alle Elemente aus a und b, die nicht in beiden Mengen enthalten sind,
 * die neue Menge, die zurueckgegeben wird.
 *
 * Die beiden Mengen a und b bleiben hierbei unveraendert.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return die symmetrische Differenzmenge der Mengen a und b.
 */
Set set_symmetric_difference (Set a, Set b);

/**
 * Erzeugt eine Kopie der Menge s.
 *
 * Die Menge s bleibt hierbei unveraendert.
 *
 * @param[in] s die Ausgangsmenge.
 *
 * @return die Kopie der Menge a.
 * 
 * @post die erzeugte Menge ist gleich s.
 */
Set set_copy (Set s);

/**
 * Schreibt die Stringrepraesentation der Menge s in den uebergebenen Ausgabestrom.
 *
 * @pre stream ist nicht der Nullzeiger.
 * @param[in] stream Ausgabestrom.
 * @param[in] s die Menge.
 */
void set_print (FILE * stream, Set s);

/**
 * Schreibt die Stringrepraesentation der Liste, die zur Implementierung der Menge s 
 * genutzt wird, in den uebergebenen Ausgabestrom.
 *
 * @pre stream ist nicht der Nullzeiger.
 * @param[in] stream Ausgabestrom.
 * @param[in] s die Menge.
 */
void set_print_list (FILE * stream, Set s);


#endif
