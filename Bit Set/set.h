#ifndef __SET_H__
#define __SET_H__

/**
 * @file set.h Schnittstelle einer Bibliothek fuer Mengenoperationen.
 *
 * Mengen werden intern durch Arrays dargestellt, wobei jedes Bit des Arrays
 * einem moeglichen Element der Menge entspricht. Das Element ist in der Menge
 * enthalten, wenn das entsprechende Bit gesetzt ist. Das Element mit
 * dem Wert 0 befindet sich an der niedrigstwertigen Stelle des ersten
 * Array-Elements, das Element mit dem hoechsten Wert befindet sich an
 * der hoechstwertigen Stelle im letzten Array-Element. Der kleinste Wert, der
 * in einer Menge gespeichert werden kann, ist der Wert 0. Das groesste Element,
 * das in einer Menge gespeichert werden kann, hat den Wert SET_MAX_ELEM.
 * Mengenoperationen sind als Bitoperationen implementiert.
 *
 * @author andre kloodt, alexander loeffler
 */
#include <limits.h>

#ifndef SET_MAX_ELEM
/** groesstes Element, das die Menge aufnehmen kann */
#define SET_MAX_ELEM 511
#endif

#ifndef SET_ARRAY_ENTRY_TYPE
/** Typ eines Array-Eintrags (Basistyp). */
#define SET_ARRAY_ENTRY_TYPE unsigned char
#endif

/** Anzahl der Bits, die der (Basistyp) umfasst. */
/** @todo Definition muss ergaenzt werden */
#define SET_ARRAY_ENTRY_TYPE_BITS (sizeof(unsigned char) * CHAR_BIT)

/** Groesse des Arrays. */
/** @todo Definition muss ergaenzt werden */
#define SET_ARRAY_SIZE ((SET_MAX_ELEM / SET_ARRAY_ENTRY_TYPE_BITS) + 1)

/** Eine Menge, hier als Array vom Typ SET_ARRAY_ENTRY realisiert. */
typedef SET_ARRAY_ENTRY_TYPE Set [SET_ARRAY_SIZE];

/** Datentyp der Mengenelemente */
typedef unsigned long Element;

/** Leere Menge zur Initialisierung von Variablen. */
#define EMPTY_SET {0}

/**
 * Fuegt ein Element e in die Menge s ein.
 *
 * @param[in,out] s Zeiger auf die Menge.
 * @param[in] e das einzufuegende Element.
 *
 * @pre s ist nicht der Nullzeiger.
 * @post die Menge, auf die s zeigt, enthaelt den Wert e.
 */
void set_insert (Set * s, Element e);

/**
 * Loescht ein Element e aus der Menge s. 
 *
 * @param[in,out] s Zeiger auf die Menge.
 * @param[in] e das zu loeschende Element.
 *
 * @pre s ist nicht der Nullzeiger.
 * @post der Wert e ist nicht in der Menge, auf die s zeigt, enthalten.
 */
void set_remove (Set * s, Element e);

/**
 * Loescht alle Elemente aus der Menge s.
 *
 * @param[in,out] s Zeiger auf die Menge.
 *
 * @pre s ist nicht der Nullzeiger.
 * @post s ist Zeiger auf leere Menge.
 */
void set_remove_all_elems (Set * s);

/**
 * Prueft, ob die Menge s leer ist.
 *
 * @param[in] s die Menge.
 *
 * @return 1, falls Menge leer ist; sonst 0.
 */
int set_is_empty (Set s);

/**
 * Liefert die Kardinalitaet der Menge s (Anzahl der Elemente).
 *
 * @param[in] s die Menge.
 *
 * @return Kardinalitaet einer Menge.
 */
unsigned long set_cardinality (Set s);

/**
 * Liefert das groesste Element der Menge s. 
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
 * @param[in] s die Menge.
 *
 * @return kleinstes Element der Menge.
 *
 * @pre s ist nicht leer.
 */
Element set_min (Set s);

/**
 * Prueft, ob das Element e in der Menge s enthalten ist.
 *
 * @param[in] s die Menge.
 * @param[in] e das zu pruefende Element.
 *
 * @return 1, falls Element enthalten ist; sonst 0.
 */
int set_contains (Set s, Element e);

/**
 * Prueft, ob die Menge a Untermenge der Menge b ist, also alle Elemente von a
 * auch in b enthalten sind.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a Untermenge von b ist; sonst 0.
 */
int set_is_subset (Set a, Set b);

/**
 * Prueft, ob die Menge a gleich der Menge b ist.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a gleich Menge b ist; sonst 0.
 */
int set_equals (Set a, Set b);

/**
 * Erzeugt die Vereinigungsmenge der beiden Mengen a und b. Dabei bilden
 * alle Elemente, die in a oder b enthalten sind, die neue Menge res.
 *
 * @param[in,out] res Zeiger auf die Vereinigungsmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res ist nicht der Nullzeiger.
 * @pre res zeigt auf eine leere Menge.
 */
void set_union (Set * res, Set a, Set b);

/**
 * Erzeugt die Schnittmenge der beiden Mengen a und b. Dabei bilden
 * die Elemente, die sowohl in a als auch in b enthalten sind, die 
 * neue Menge res.
 *
 * @param[in,out] res Zeiger auf die Schnittmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res ist nicht der Nullzeiger.
 * @pre res zeigt auf eine leere Menge.
 */
void set_intersection (Set * res, Set a, Set b);

/**
 * Erzeugt die Differenzmenge der beiden Mengen a und b. Dabei bilden
 * die Elemente aus a, die nicht in b enthalten sind, die neue
 * Menge res.
 *
 * @param[in,out] res Zeiger auf die Differenzmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res ist nicht der Nullzeiger.
 * @pre res zeigt auf eine leere Menge.
 */
void set_difference (Set * res, Set a, Set b);

/**
 * Erzeugt die symmetrische Differenzmenge der beiden Mengen a und b. Dabei
 * bilden alle Elemente aus a und b, die nicht in beiden Mengen enthalten sind,
 * die neue Menge res.
 *
 * @param[in,out] res Zeiger auf die symmetrische Differenzmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res ist nicht der Nullzeiger.
 * @pre res zeigt auf eine leere Menge.
 */
void set_symmetric_difference (Set * res, Set a, Set b);

/**
 * Erzeugt eine Kopie der Menge s. Die Menge res wird dabei vollstaendig 
 * ueberschrieben.
 * 
 * @param[in,out] res Zeiger auf die neue Menge mit der Kopie von s.
 * @param[in] s die Ausgangsmenge.
 *
 * @pre res ist nicht der Nullzeiger.
 * @post die Menge, auf die res zeigt, ist gleich s.
 */
void set_copy (Set * res, Set s);

/**
 * Erzeugt die zweielementige Partition der Menge a, bei der die Summe aller
 * Elemente in der ersten Menge (b) moeglichst gleich der Summe aller Elemente
 * in der zweiten Menge (c) ist. Die Summe der Elemente in der ersten Menge (b)
 * ist dabei immer groesser gleich der Summe der Elemente in der zweiten Menge
 * (c). Der Wert jedes Elements in der ersten Menge (b) ist zudem kleiner als
 * der Wert jedes Elements in der zweiten Menge (c). Wenn die zu partionierende
 * Menge leer ist, so werden zwei leere Mengen als Partitionen erzeugt.
 *
 * @param[in,out] res1 Zeiger auf die erste Menge der Partition.
 * @param[in,out] res2 Zeiger auf die zweite Menge der Partition.
 * @param[in] s die Ausgangsmenge.
 *
 * @pre res1 ist nicht der Nullzeiger.
 * @pre res2 ist nicht der Nullzeiger.
 * @pre res1 zeigt auf eine leere Menge.
 * @pre res2 zeigt auf eine leere Menge.
 * @post die Schnittmenge der Menge, auf die res1 zeigt, und der
 *                  Menge, auf die res2 zeigt, ist die leere Menge.
 * @post die Vereinigungsmenge der Menge, auf die res1 zeigt, und der
 *                  Menge, auf die res2 zeigt, ist die Menge s.
 */
void set_partition (Set * res1, Set * res2, Set s);

/**
 * Schreibt die Stringrepraesentation der Menge s in den uebergebenen Ausgabestrom.
 *
 * @pre stream ist nicht der Nullzeiger.
 * @param[in] stream Ausgabestrom.
 * @param[in] s die Menge.
 */
void set_print (FILE * stream, Set s);
#endif
