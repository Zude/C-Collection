/**
 * @file set.c Implementierung einer Bibliothek fuer Mengenoperationen.
 *
 * @author alexander loeffler, andre kloodt
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "set.h"
#include "error.h"

/* Referenzaehler fuer die gespeicherten Intervalllistenelemente */
int setlist_refs = 0;

/**
 * Erzeugt ein neuen Knoten mit einem uebergebenen Intervall. 
 *
 * @param[in] start der Anfang des Intervalls.
 * @param[in] end das Ende des Intervalls.
 * @param[in] next Zeiger auf das naechste Element
 *
 * @return ein neuer Knoten mit dem Intervall von start bis end.
 */
Set
createIntervall(Element start, Element end, Set next) {
    Set s = malloc(sizeof(struct Intervall));
    if(s == NULL){
        exit(ERR_OUT_OF_MEMORY);
    }
    s->start = start;
    s->end = end; 
    s->next = next;
    setlist_refs++;
    
    return s;
}

/**
 * Erzeugt ein neuen Knoten mit einem Element. 
 *
 * @param[in] e Das neue Element.
 *
 * @return ein neuer Knoten mit dem Element e.
 */
Set
createSingleIntervall(long e) {
    Set s = malloc(sizeof(struct Intervall));
    if(s == NULL){
        exit(ERR_OUT_OF_MEMORY);
    }
    s->start = e;
    s->end = e;
    s->next = EMPTY_SET;
    setlist_refs++;
    return s;
}

/**
 * Fuegt ein neues Element an den uebergebenen Knoten an. 
 *
 * @param[in] curr Zeiger auf den aktuellen Knoten.
 * @param[in] e    Das neue Element.
 *
 * @return der Knoten curr mit dem neuen Element e.
 */
Set
addSingleIntervall(Set curr, long e){
    if(e == curr->end + 1) {
        curr->end = e;
    } else if(e == curr->start -1) {
        curr->start = e;
    } else{
        curr->next = createSingleIntervall(e);
        curr = curr->next;
    }
    return curr;
}

Set
set_insert (Set s, Element e) {
    Set curr = s;
    Set prev = NULL;
          
    /* Wenn leer, dann erzeuge neues Intervall */
    if (SET_IS_EMPTY(s)) {
        s = createIntervall(e,e,EMPTY_SET);
        assert(set_contains(s,e));
        return s;
    }

    /* Travasieren */
    while(curr != EMPTY_SET && e > curr->end){
        prev = curr;
        curr = curr->next;
    }

    /* Wenn wir ganz hinten sind, hinten einfuegen **/
    if (curr == NULL) {
        if (e == prev->end + 1) {
            prev->end = e;
        }
        else{
        prev->next = createIntervall(e,e,EMPTY_SET);
        }
        assert(set_contains(s,e));
        return s;
    }
    /* Wenn wir nicht vorne sind, fuege richtig ein */
    if(!SET_IS_EMPTY(prev)){
        if (e >= curr->start) {
            /* Element bereits drinnen */
            assert(set_contains(s,e));
            return s;
        }
        if (e == curr->start - 1) {
            curr->start = e;
        }
        if (e == prev->end + 1) {
            prev->end = e;
        }
        if (prev->end == curr->start) {
            prev->next = curr->next;
            prev->end = curr->end;
            free(curr);
            setlist_refs--;
            assert(set_contains(s,e));
            return s;
        }
        else if((e == prev->end  )|| (e == curr->start )){
            assert(set_contains(s,e));
            return s;
        }
    }
    /* Wenn wir vorne sind, fuege richtig ein */
    else{
        if(e >= curr->start){
            assert(set_contains(s,e));
            return s;
        }
        else if (e == curr->start - 1) {
            curr->start = e;
            assert(set_contains(s,e));
            return s;
        } 
        else {
            Set new = createIntervall(e,e,EMPTY_SET);
            new->next = s;
            assert(set_contains(new,e));
            return new; 
        } 
    }
    prev->next = createIntervall(e,e,EMPTY_SET);
    prev->next->next = curr;
    assert(set_contains(s,e));
    return s;   
}


Set
set_remove (Set s, long e) {
    Set curr = s;
    Set prev = EMPTY_SET;

    /**travasieren**/
    while(curr != EMPTY_SET && e > curr->end  ){
        prev = curr;
        curr = curr->next;
    }

    /* pruefe nachfolgend alle faelle */
    if (SET_IS_EMPTY(curr)) {
        assert(!set_contains(s,e));
        return s;
    }
    
    else if (e == curr->end && e != curr->start) {
        curr->end = curr->end-1;
        assert(!set_contains(s,e));
        return s;
    }
    else if (e == curr->start && e != curr->end) {
        curr->start = curr->start+1;
        assert(!set_contains(s,e));
        return s;
    }
    else if (e > curr->start && e < curr->end) {
        curr->next = createIntervall(e+1, curr->end, curr->next);
        curr->end = e-1;
        assert(!set_contains(s,e));
        return s;
    }
    else if (prev == EMPTY_SET){
        s = s->next;
        free(curr);
        setlist_refs--;  
        assert(!set_contains(s,e));
        return s;
    }
    else{
        prev->next = curr->next;
        free(curr);
        setlist_refs--; 
        assert(!set_contains(s,e));
        return s;
    }
}


Set
set_clear (Set s) {
    
    while(!SET_IS_EMPTY(s)){
        Set toFree = s;
        s = s->next;
        free(toFree);
        setlist_refs--;
    }
    assert(SET_IS_EMPTY(s));
    return s;
    
}

unsigned long
set_cardinality (Set s) {
    unsigned long counter = 0;
    Set curr = s;

    while(!SET_IS_EMPTY(curr)){
        counter = counter + (curr->end+1 - curr->start);
        curr = curr->next;
    }

    return counter;
}


Element 
set_max(Set s) {
    
    Set curr = s;

    assert(!SET_IS_EMPTY(s));

    while(!SET_IS_EMPTY(curr->next)){
        curr = curr->next;
    }
    
    return curr->end;
}

Element
set_min(Set s) {
    assert(!SET_IS_EMPTY(s));

    return s->start;
}


int
set_contains (Set s, long e) {

    if(SET_IS_EMPTY(s)){
        return 0;
    }
    while(!SET_IS_EMPTY(s)){
        if (e >= s->start && e <= s->end) {
            return 1;
        }
        s = s->next;
    }
    return 0;
}


int
set_is_subset (Set a, Set b) {

if (SET_IS_EMPTY(a)){
    return 1;
} else if (SET_IS_EMPTY(b)) {
    return 0;
}

/**iteration durch die liste**/
while(!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b)){
    if (a->start >= b->start && a->end <= b->end){
        a = a->next;
    } else if(a->start > b->end){
        b = b->next;
    } else {
        return 0;
    }
}

if(SET_IS_EMPTY(a)){
    return 1;
} else {
    return 0;
}
}


int
set_equals (Set a, Set b) {

    if(SET_IS_EMPTY(a) && SET_IS_EMPTY(b)){
        return 1;
    } else if((SET_IS_EMPTY(a) && !SET_IS_EMPTY(b)) || (!SET_IS_EMPTY(a) && SET_IS_EMPTY(b))){
        return 0;
    }
    /* ueberprueft ob der Start und das Ende des Intervalls uebereinstimmt */
    while(!SET_IS_EMPTY(a)){
        if((a->start != b->start) || (a->end != b->end)){
            return 0;
        }
        a = a->next;
        b = b->next;
    }

    if(SET_IS_EMPTY(a) && SET_IS_EMPTY(b)){
        return 1;
    }

    return 0;
}

 

Set
set_union (Set a, Set b) {
    int biggestNum = 0;
    Set new = EMPTY_SET;
    Set curr = new;

/**hauptalgorithmus**/
while(!SET_IS_EMPTY(a) || !SET_IS_EMPTY(b)){
    /**fuer den fall das a groesser als b ist**/
    if (SET_IS_EMPTY(b) || (!SET_IS_EMPTY(a) && a->start <= b->start)) {
        if(SET_IS_EMPTY(new)){
            new = createIntervall(a->start, a->end,NULL);
            curr = new;
            biggestNum = a->end;
            a = a->next; 
        }
        else if(a->start <= biggestNum && a->end > biggestNum){
            curr->end = a->end;

            biggestNum = a->end;
            a = a->next; 
        }
        else if(a->start <= biggestNum){
            a = a->next; 
        }

        else if(a->start == curr->end + 1){
            curr->end = a->end;
            a = a->next; 
        }
        else if(a->start > curr->end){
            curr->next = createIntervall(a->start, a->end,NULL);
            curr= curr->next;
            biggestNum = a->end;
            a = a->next; 
        }
        else {
            a = a->next; 
        }
    }
    /**fuer  den fall das b groesser ist**/
    else if(!SET_IS_EMPTY(b)){
        if(SET_IS_EMPTY(new)){
            new = createIntervall(b->start, b->end,NULL);
            curr = new;
            biggestNum = b->end;
            b= b->next; 
        }
        else if(b->start <= biggestNum && b->end > biggestNum){
            curr->end = b->end;

            biggestNum = b->end;
            b = b->next; 
        }
        else if(b->start <= biggestNum){
            b = b->next; 
        }
        else if(b->start == curr->end + 1){
            curr->end = b->end;
            b = b->next; 
        }
        else if(b->start > curr->end){
            curr->next = createIntervall(b->start, b->end,NULL);
            curr= curr->next;
            biggestNum = b->end;
            b = b->next; 
        }
        else {
            b = b->next; 
        }

    }
}
    return new;
}



Set
set_intersection (Set a, Set b) {

    Set new = EMPTY_SET;
    Set curr = new;
    long tempValue = 0;

if(SET_IS_EMPTY(a) || SET_IS_EMPTY(b)){
    return new;
}

tempValue = a->start;

while(!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b)){
    if (tempValue <= b->start) {
        if(a->end < b->start){
            a = a->next; 
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
            /** Hier wird nur das letzte Element hinzu gefuegt */
        } else if (a->end == b->start){
            if(SET_IS_EMPTY(new)){
                new = createIntervall(a->end, a->end,NULL);
                curr = new;
                a = a->next;
            } else {
                curr->next = createIntervall(a->end, a->end,NULL);
                curr= curr->next;
                a = a->next; 
            }
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
            /** Hier wird die das Intervall von b hinzugefuegt */
        } else if(a->end > b->end){
            if(SET_IS_EMPTY(new)){
                new = createIntervall(b->start, b->end,NULL);
                curr = new;
                tempValue = b->end+1;
                b = b->next;
            } else {
                curr->next = createIntervall(b->start, b->end,NULL);
                curr= curr->next;
                tempValue = b->end+1;
                b = b->next; 
            }
            /** Hier werden beide am ende weiter gesetzt */
        } else if(a->end == b->end) {
            if(SET_IS_EMPTY(new)){
                new = createIntervall(b->start, a->end,NULL);
                curr = new;
            } else {
                curr->next = createIntervall(b->start, a->end,NULL);
                curr= curr->next;
            }
            b = b->next;
            a = a->next;
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
            /** die difference hinzufuegen */
        } else {
            if(SET_IS_EMPTY(new)){
                new = createIntervall(b->start, a->end,NULL);
                curr = new;
                a = a->next;
            } else {
                curr->next = createIntervall(b->start, a->end,NULL);
                curr= curr->next;
                a = a->next; 
            }
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
        }
        /** Hier wird die Liste b bearbeitet */
    } else if(b->end < tempValue){
            b = b->next;
    } else if(b->end == tempValue){
        if(SET_IS_EMPTY(new)){
            new = createIntervall(b->end, b->end,NULL);
            curr = new;
            tempValue = b->end+1;
            b = b->next;
        } else {
            curr->next = createIntervall(b->end, b->end,NULL);
            curr= curr->next;
            tempValue = b->end+1;
            b = b->next;
        }
        /** von dem temporaeren Wert bis zum ende des Intervalls von b */
    } else if(b->end < a->end){
        if(SET_IS_EMPTY(new)){
            new = createIntervall(tempValue, b->end,NULL);
            curr = new;
            tempValue = b->end+1;
            b = b->next;
        } else {
            curr->next = createIntervall(tempValue, b->end,NULL);
            curr= curr->next;
            tempValue = b->end+1;
            b = b->next;
        }
    } else if(b->end == a->end) {
        if(SET_IS_EMPTY(new)){
            new = createIntervall(tempValue, b->end,NULL);
            curr = new;
        } else {
            curr->next = createIntervall(tempValue, b->end,NULL);
            curr= curr->next;
        }
        b = b->next;
        a = a->next;
        if(!SET_IS_EMPTY(a)){
            tempValue = a->start;
        }
        /** Falls das Ende von b groesser sein sollte */
    } else {
       if(SET_IS_EMPTY(new)){
            new = createIntervall(tempValue, a->end,NULL);
            curr = new;
        } else {
            curr->next = createIntervall(tempValue, a->end,NULL);
            curr= curr->next;
        }
        a = a->next;
        if(!SET_IS_EMPTY(a)){
            tempValue = a->start;
        } 
    }
}
return new;
}




Set
set_difference (Set a, Set b) {

    Set new = EMPTY_SET;
    Set curr = new;
    long tempValue = 0;

if(!SET_IS_EMPTY(a) && SET_IS_EMPTY(b)){
    while(!SET_IS_EMPTY(a)){
        if(SET_IS_EMPTY(new)){
                new = createIntervall(a->start, a->end,NULL);
                curr = new;
                a = a->next;
            } else {
                curr->next = createIntervall(a->start, a->end,NULL);
                curr= curr->next;
                a = a->next;
            }
    }
    return new;
}

/** Sollte keine von beiden Leer sein wird fortgesetzt */

if(!SET_IS_EMPTY(a)){
    tempValue = a->start;
}

while(!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b)){
    if (tempValue < b->start) {
        if(a->end < b->start){
            if(SET_IS_EMPTY(new)){
                new = createIntervall(tempValue, a->end,NULL);
                curr = new;
                a = a->next;
            } else {
                curr->next = createIntervall(tempValue, a->end,NULL);
                curr= curr->next;
                a = a->next;
            }
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
            /** Im Falle dass das Ende von a groesser ist */
        } else if (a->end > b->end){
            if(SET_IS_EMPTY(new)){
                new = createIntervall(tempValue, b->start-1,NULL);
                curr = new;
                tempValue = b->end + 1;
                b = b->next;
            } else {
                curr->next = createIntervall(tempValue, b->start-1,NULL);
                curr= curr->next;
                tempValue = b->end + 1;
                b = b->next; 
            }
            while(!SET_IS_EMPTY(b) && a->end > b->end){
                curr->next = createIntervall(tempValue, b->start-1,NULL);
                curr= curr->next;
                tempValue = b->end + 1;
                b = b->next; 
            }
            /** Hier werden beide Listen weiter gesetzt */
        } else {
            if(SET_IS_EMPTY(new)){
                new = createIntervall(tempValue, b->start-1,NULL);
                curr = new;
            } else {
                curr->next = createIntervall(tempValue, b->start-1,NULL);
                curr= curr->next;
            }
            a = a->next;
            b = b->next;   
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
        }
        /** Hier wird die Liste b bearbeitet */
    } else {
        if(a->end <= b->end){
            a = a->next;
            if(!SET_IS_EMPTY(a)){
                tempValue = a->start;
            }
        } else {
            tempValue = b->end+1;
            b = b->next;
        }
    }
}

/** Fuer den Fall das am Ende in a noch Elemente sind */
if(SET_IS_EMPTY(b) && !SET_IS_EMPTY(a)){
    if(a->start > tempValue){
        tempValue = a->start;
    }
    while(!SET_IS_EMPTY(a)){
        if(SET_IS_EMPTY(new)){
            new = createIntervall(tempValue, a->end,NULL);
            curr = new;
            a = a->next;
        } else {
            curr->next = createIntervall(tempValue, a->end,NULL);
            curr= curr->next;
            a = a->next; 
        }
        if(!SET_IS_EMPTY(a)){
            tempValue = a->start;
        }
    } 
}
return new;
}





Set
set_symmetric_difference (Set a, Set b) {
    Set new = EMPTY_SET;
    Set curr = new; 
    long last = 0;

    /**hier wird geschaut welche der listen kleiner ist um last  zu belegen**/
    if ((!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b) && a->start <= b->start)) {
        last = a->start-1;
    }
    else if ((!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b) && a->start > b->start)) {
        last = b->start-1;
    }

    /**die schleife ueber den algorithmus**/
    while(!SET_IS_EMPTY(a) || !SET_IS_EMPTY(b)){
        /**fuer den fall das b leer ist**/
        if(SET_IS_EMPTY(b)){
            if(a->start > last){
                    last = a->start-1;
                }
             if(SET_IS_EMPTY(new)){
                    new = createIntervall(last+1, a->end,NULL);
                    curr = new;
                    last = a->end;
                    a = a->next;
                }
                else{
                    if(last+1 == curr->end+1){
                        curr->end = a->end;
                        last = a->end;
                        a = a->next;
                    }
                    else{
                        curr->next = createIntervall(last+1, a->end,NULL);
                        curr= curr->next;
                        last = a->end;
                        a = a->next;
                    }
                    
                }  
        } 
        /**fuer den fall das a intervall komplett kleiner ist**/
        else if ((!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b) && a->start <= b->start)) {
            if(a->end < b->start){
                if(a->start > last){
                    last = a->start-1;
                }
                if(SET_IS_EMPTY(new)){
                    new = createIntervall(last+1, a->end,NULL);
                    curr = new;
                    last = a->end;
                    a = a->next;
                }
                else{
                    if(last+1 == curr->end+1){
                        curr->end = a->end;
                        last = a->end;
                        a = a->next;
                    }
                    else{
                        curr->next = createIntervall(last+1, a->end,NULL);
                        curr= curr->next;
                        last = a->end;
                        a = a->next;
                    }
                }
            }
            /**falls das ende von a groesser  ist als  b start**/
            else if(a->end >= b->start){

                if(a->start > last){
                    last = a->start-1;
                }
                if(a->start == b->start){
                        last = b->start;
                    }
                else if(SET_IS_EMPTY(new)){
                    new = createIntervall(last+1, b->start-1,NULL);
                    curr = new;
                    last = b->start-1;
                }
                else{
                    if(last+1 == curr->end+1){
                        curr->end = b->start-1;
                        last = b->start-1;
                    }
                    else{
                        curr->next = createIntervall(last+1, b->start-1,NULL);
                        last = b->start-1;
                        curr= curr->next;
                    }
                }
                /**wenn das ende  von a groesser ist als das ende  von b**/
                if(a->end >= b->end){
                    if(a->start == a->end){
                        last = b->end;
                        b = b->next;
                        a = a->next;
                    }else if(a->end == b->end){
                        last = b->end;
                        b = b->next;
                        a = a->next;
                    }
                    else{
                        last = b->end;
                        b = b->next;   
                    }      
                }
                else{
                   last = a->end;
                    a = a->next; 
                }
            }
        }

        /**a ist leer  b nicht**/
        else if(SET_IS_EMPTY(a)){
            if(b->start > last){
                    last = b->start-1;
                }
             if(SET_IS_EMPTY(new)){
                    new = createIntervall(last+1, b->end,NULL);
                    curr = new;
                    last = b->end;
                    b = b->next;
                }
                else{
                    if(last+1 == curr->end+1){
                        curr->end = b->end;
                        last = b->end;
                        b = b->next;
                    }
                    else{
                        curr->next = createIntervall(last+1, b->end,NULL);
                        curr= curr->next;
                        last = b->end;
                        b = b->next;
                    }
                    
                }
        } 
        /**b kleiner als a**/
        else if ((!SET_IS_EMPTY(a) && !SET_IS_EMPTY(b) && b->start <= a->start)) {
            if(b->end < a->start){
                if(b->start > last){
                    last = b->start-1;
                }
                if(SET_IS_EMPTY(new)){
                    new = createIntervall(last+1, b->end,NULL);
                    curr = new;
                    last = b->end;
                    b = b->next;
                }
                else{
                    if(last+1 == curr->end+1){
                        curr->end = b->end;
                        last = b->end;
                        b = b->next;
                    }
                    else{
                        curr->next = createIntervall(last+1, b->end,NULL);
                        curr= curr->next;
                        last = b->end;
                        b = b->next;
                    }
                }
            }
            /**das ende von b ist groesser als a start**/
            else if(b->end >= a->start){
                if(b->start > last){
                    last = b->start-1;
                }
                if(b->start == a->start){
                        last = b->start;
                    }
                else if(SET_IS_EMPTY(new)){
                    new = createIntervall(last+1, a->start-1,NULL);
                    curr = new;
                    last = a->start-1;
                }
                else{
                    if(last+1 == curr->end+1){
                        curr->end = a->start-1;
                        last = a->start-1;
                        
                    }
                    else{
                        curr->next = createIntervall(last+1, a->start-1,NULL);
                        last = a->start-1;
                        curr= curr->next;
                    }
                }
                /**das ende von bist  groesser  als  das ende von a**/
                if(b->end >= a->end){
                    if(b->start == b->end){
                        last = a->end;
                        b = b->next;
                        a = a->next;
                    }else if(a->end == b->end){
                        last = a->end;
                        b = b->next;
                        a = a->next;
                    }else{
                        last = a->end;
                        a = a->next;   
                    } 
                }
                else{
                   last = b->end;
                    b = b->next; 
                }
            }
        }
    }
    return new;
}




Set
set_copy (Set s) {
    Set new = EMPTY_SET;
    Set curr = s;
    Set newCurr = new;
    
    if (SET_IS_EMPTY(s)) {
        return EMPTY_SET;
    }
    

    while(!SET_IS_EMPTY(curr)){
        /**falls wir noch keine liste haben**/
        if(SET_IS_EMPTY(new)){
            new = createIntervall(curr->start, curr->end, EMPTY_SET);
            newCurr = new;
            curr = curr->next;
        }
        else{
        newCurr->next = createIntervall(curr->start, curr->end, EMPTY_SET);
        newCurr = newCurr->next;
        curr = curr->next;
        }
    }
    
    assert (set_equals(s,new));
    return new; 
}

void
set_print (FILE * stream, Set s) {

    Set curr = s;
    long i = 0;

    assert(stream != NULL);

    fprintf(stream, "{");

    while(!SET_IS_EMPTY(curr)){
        
        for(i = curr->start; i <= curr->end; i++)
        {   if(SET_IS_EMPTY(curr->next) && i == curr->end){
                fprintf(stream, "%ld", i);
            }            
            else{
                fprintf(stream, "%ld,", i);
            }
        }
        curr = curr->next;
    }
    fprintf(stream, "}");
}


void
set_print_list (FILE * stream, Set s) {

    Set curr = s;

    assert(stream != NULL);

    
    if (SET_IS_EMPTY(curr)) {
        fprintf(stream, "[]");
    }
    else{
        while(!SET_IS_EMPTY(curr)){
            fprintf(stream, "[%ld:%ld]", curr->start,curr->end);
            curr = curr->next;
        }
    }
}
