/**
 * @file set.c Implementierung einer Bibliothek fuer Mengenoperationen.
 *
 * @author kandre kloodt, alexander loeffler
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "set.h"

void set_insert (Set * s, Element e){
    unsigned long firstIndex = e/SET_ARRAY_ENTRY_TYPE_BITS;
    unsigned long secondIndex = e-firstIndex*SET_ARRAY_ENTRY_TYPE_BITS;
    
    SET_ARRAY_ENTRY_TYPE insertPos = 1 << secondIndex;

    assert (s != NULL);
    /** assert ? **/
    (*s)[firstIndex] |= insertPos;

    assert ((*s)[firstIndex] & insertPos);
    
}

void set_remove (Set * s, Element e){
    unsigned long firstIndex = e/SET_ARRAY_ENTRY_TYPE_BITS;
    unsigned long secondIndex = e-firstIndex*SET_ARRAY_ENTRY_TYPE_BITS;

    SET_ARRAY_ENTRY_TYPE deletePos = 1 << secondIndex;

    assert (s != NULL); 

    (*s)[firstIndex] &= ~deletePos;

    assert (!((*s)[firstIndex] & deletePos));

}

void set_remove_all_elems (Set * s){

    unsigned long i = 0;

    assert(s!=NULL);

    for(; i <= SET_MAX_ELEM; i++){
        set_remove(s, i);
    }

    /**todo assert**/

}

int set_is_empty (Set s){

    unsigned int i = 0;

    for(; i < SET_ARRAY_SIZE; i++){
        if(s[i] != 0){
            return 0;
        }
    }
    return 1;
}

unsigned long set_cardinality (Set s){
    unsigned long i = 0;
    unsigned long containCount = 0;
    for (; i<=SET_MAX_ELEM; i++){
        if(set_contains(s,i)){
            containCount++;
        }
    }
    return containCount;
}

Element set_max (Set s){
    
    unsigned long elements = 0;
    unsigned long biggestEle = 0;

    assert(!set_is_empty(s));

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(s,elements) && elements > biggestEle){
            biggestEle = elements;
        }
    }
    return biggestEle;
}



Element set_min (Set s){
    
    unsigned long elements = 0;
    unsigned long smallestEle = SET_MAX_ELEM;

    assert(!set_is_empty(s)); /** hier überlegen todo**/

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(s,elements) && elements < smallestEle){
            smallestEle = elements;
        }
    }
    return smallestEle;
}

int set_contains (Set s, Element e){
    unsigned long firstIndex = e/SET_ARRAY_ENTRY_TYPE_BITS;
    unsigned long secondIndex =  e-firstIndex*SET_ARRAY_ENTRY_TYPE_BITS;
    
    SET_ARRAY_ENTRY_TYPE searchPos = 1 << secondIndex;

    return (s[firstIndex] & searchPos);

}

int set_is_subset (Set a, Set b){

    unsigned long elements = 0;

    if(set_is_empty(a)){
        return 1;
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(a,elements) && !set_contains(b,elements)){
            return 0;
        }
    }
    return 1;
}

int set_equals (Set a, Set b){

    unsigned long elements = 0;

    if(set_is_empty(a) && set_is_empty(b)){
        return 1;
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(!set_contains(a,elements) != !set_contains(b,elements)){
            return 0;
        }
    }
    return 1;

}

void set_union (Set * res, Set a, Set b){
    
    unsigned long elements = 0;

    assert(res != NULL);
    assert(set_is_empty(*res));



    if(set_is_empty(a) && set_is_empty(b)){
        return; 
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(a,elements) || set_contains(b,elements)){
            set_insert(res, elements);
        }
    }
    return;
}

void set_intersection (Set * res, Set a, Set b){
    
    unsigned long elements = 0;

    assert(res != NULL);
    assert(set_is_empty(*res));

    if(set_is_empty(a) && set_is_empty(b)){
        return; 
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(a,elements) && set_contains(b,elements)){
            set_insert(res, elements);
        }
    }
    return;
}

void set_difference (Set * res, Set a, Set b){

    unsigned long elements = 0;

    assert(res != NULL);
    assert(set_is_empty(*res));

    if(set_is_empty(a) && set_is_empty(b)){
        return; 
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(a,elements) && !set_contains(b,elements)){
            set_insert(res, elements);
        }
    }
    return;
}

void set_symmetric_difference (Set * res, Set a, Set b){

    unsigned long elements = 0;

    assert(res != NULL);
    assert(set_is_empty(*res));

    if(set_is_empty(a) && set_is_empty(b)){
        return; 
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(!set_contains(a,elements) != !set_contains(b,elements)){
            set_insert(res, elements);
        }
    }
    return;
}

void set_copy (Set * res, Set s){

    unsigned long elements = 0;

    assert(res != NULL);

    if(!set_is_empty(*res)){
        set_remove_all_elems(res);
    }

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(s,elements)){
            set_insert(res, elements);
        }
    }

    return;
}

/**
 * Summiert alle Elemente der Ausgangsmenge. Sollte die 
 * Ausgamgsmenge die leere Menge sein, ist der return 0. 
 *
 * @param[in] s Die Ausgangsmenge.
 * 
 * @return Die Summe aller Elemente
 *
 */

Element set_sum (Set s){
    assert (s != NULL);

{
    Element element = 0;
    SET_ARRAY_ENTRY_TYPE sum = 0;
    if(set_is_empty(s)){
        return 0;
    }

    for(; element<=SET_MAX_ELEM; element++){
        if(set_contains(s,element)){
            sum += element;
        }
    }

    return sum;
}
}

void set_partition (Set * res1, Set * res2, Set s){

    assert (res1 != NULL);
    assert (res2 != NULL);
    assert (set_is_empty((*res1)));
    assert (set_is_empty((*res2)));
{

    if(set_sum(s) > ULONG_MAX){
        exit(1);
    }

    set_copy(res1,s);

    if(set_is_empty(s)){
        
    } else {
        do{
            set_insert(res2,set_max((*res1)));
            set_remove(res1,set_max((*res1)));
        } while (set_sum((*res1)) >= set_sum((*res2)));
        set_insert(res1,set_min((*res2)));
        set_remove(res2,set_min((*res2)));
    }

}
}

void set_print (FILE * stream, Set s){

    unsigned long elements = 0;

    fprintf(stream, "{");
    
    if(!set_is_empty(s)){

    unsigned long setMax = set_max(s);

    for(; elements<=SET_MAX_ELEM; elements++){
        if(set_contains(s,elements)){
            if(elements < setMax ){
                fprintf(stream, "%lu,", elements);

            }
            else{
                fprintf(stream, "%lu", elements);
            }
        }
    }
    }
    fprintf(stream, "}");


    return;
}


