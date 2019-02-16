/**
 * @file polynom.c Implementierung eines Moduls mit Polynomfunktionen.
 *
 * @author andre kloodt, alexander loeffler
 */

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "polynom.h"

/** Referenzzaehler fuer die gespeicherten Polynome */
int polynom_refs = 0;

Polynom
polynom_clear(Polynom p)
{
    free(p->koeff);
    p->koeff = NULL;
    polynom_refs--;
    free(p);
    p = NULL;
    polynom_refs--;
    return p;
}

double
polynom_evaluate(Polynom p, double x)
{
    assert(p != INVALID_POLYNOM);
    {
        double result = 0;
        int runner = p->max;
        for (; runner >= 0; runner--)
        {
            result += p->koeff[runner] * (pow(x, runner));
        }
        return result;
    }
}

Errorcode polynom_computeRoot(Polynom p, double ak, double bk, unsigned int maxSteps, double *root,
                              Printer print, FILE *stream)
{
    assert(p != NULL);
    assert(root != NULL);
    {
        unsigned int runner = 0;

        double c = 0,
               fbk = 0,
               fak = 0,
               fc = 0;

        State myState;

        if (maxSteps == 0)
        {
            maxSteps = 1;
        }

        /**regular falsi algorithmus**/
        while (runner <= maxSteps)
        {
            fbk = polynom_evaluate(p, bk);
            fak = polynom_evaluate(p, ak);

            /**test ob zaehler 0 ist**/
            if(fabs((fbk-fak)) < POLYNOM_EPSILON){
                myState = STATE_DIV_BY_ZERO;
                print(stream, p, ak, bk, myState, runner);
                return ERR_DIVISION_BY_ZERO;
            }

            c = ((ak * fbk) - (bk * fak)) / (fbk - fak);
            fc = polynom_evaluate(p, c);

            /**abbruchbedingung**/
            if (fabs(fc) < POLYNOM_EPSILON)
            {
                (*root) = c;
                myState = STATE_COMPLETED_LAST_STEP;
                print(stream, p, ak, bk, myState, runner);
                myState = STATE_COMPLETED_RESULT;
                print(stream, p, c, c, myState, runner);
                return ERR_NULL;
            }

            /**abbruchbedingung**/
            if (runner == maxSteps)
            {
                myState = STATE_MAX_STEPS_REACHED;
                print(stream, p, ak, bk, myState, runner);
                return ERR_MAX_STEPS_REACHED;
            }

            /**anpassung der Werte**/
            if ((fak < 0 && fc < 0) || (fak >= 0 && fc >= 0))
            {
                myState = STATE_RUNNING_FROM_THE_LEFT;
                print(stream, p, ak, bk, myState, runner);
                ak = c;
            }
            else if (((fbk < 0 && fc < 0) || (fbk >= 0 && fc >= 0)))
            {
                myState = STATE_RUNNING_FROM_THE_RIGHT;
                print(stream, p, ak, bk, myState, runner);
                bk = c;
            }
            runner++;
        }

        myState = STATE_MAX_STEPS_REACHED;
        print(stream, p, ak, bk, myState, runner);
        return ERR_MAX_STEPS_REACHED;
    }
}

Polynom polynom_parse(char *str, Errorcode *error)
{
    assert(str != NULL);
    assert(error != NULL);
    {
        char
            x = '\0',
            power = '\0',
            end = '\0',
            *newStr = str;

        int
            num = 0,
            expo = 0,
            maxExpo = 0;

        double
            koeff = 0;

        /* Speicher fuer den Struct reservieren */
        Polynom p = malloc(sizeof(struct PolynomStruct));
        if (p == NULL)
        {
            exit(ERR_OUT_OF_MEMORY);
        }

        polynom_refs++;
        p->max = 0;

        /* hoechster Exponent und Syntax pruefung */
        while (!(*error) && str[0] != '\0')
        {
            if ((sscanf(str, " %lf%c%c%d%n", &koeff, &x, &power, &expo, &num) >= 4) &&
                x == 'x' && power == '^')
            {
                str += num;
                if (fabs(koeff) >= DBL_EPSILON && expo > maxExpo)
                {
                    maxExpo = expo;
                }
            }
            else if (sscanf(str, " %c", &end) == EOF)
            {
                break;
            }
            else
            {
                (*error) = ERR_SYNTAX_ERROR;
            }
        }

        /* Speicher fuer das Polynom reservieren**/
        if (maxExpo == 0)
        {
            p->koeff = calloc(1, sizeof(double));
            if (p == NULL)
            {
                exit(ERR_OUT_OF_MEMORY);
            }
            polynom_refs++;
        }
        else
        {
            p->koeff = calloc(maxExpo + 1, sizeof(double));
            if (p == NULL)
            {
                exit(ERR_OUT_OF_MEMORY);
            }
            polynom_refs++;
        }

        /* Koeffizienten uebertragen */
        if (!(*error))
        {
            while (sscanf(newStr, " %lf%c%c%d%n", &koeff, &x, &power, &expo, &num) >= 4)
            {
                newStr += num;
                if (expo <= maxExpo)
                {

                    if (fabs(p->koeff[expo]) >= DBL_EPSILON)
                    {

                        p->koeff[expo] += koeff;
                    }
                    else
                    {

                        p->koeff[expo] = koeff;
                    }
                }
            }
        }
        p->max = maxExpo;
        return p;
    }
}

void
polynom_print (FILE * stream, Polynom p) {
    assert(stream != NULL);
    assert(p != INVALID_POLYNOM);
    {   
        int runner = p->max;
        int checker = 0;
       
        for(; runner >= 0; runner--)
        {
            if(fabs(p->koeff[runner]) >= DBL_EPSILON){
                
                if (p->koeff[runner] < 0) {
                    fprintf(stream,"%.3fx^%d",p->koeff[runner], runner);
                    if(checker == 0){
                        checker = 1;
                    }
                }
                else if(checker == 1){
                fprintf(stream,"+%.3fx^%d",p->koeff[runner], runner);
                }else{
                   fprintf(stream,"%.3fx^%d",p->koeff[runner], runner); 
                   checker = 1;
                }
            }
            
        }
        fprintf(stdout, "\n");
    }
}
