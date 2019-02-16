/**
 * @file ueb01.c
 * Uebung Systemnahe Programmierung im WS 18/19.
 *
 * Ein einfaches Programm fuer Primzahlen, welches testen kann ob
 * eine Zahl Prim ist und ausgehend von einer Zahl die naechstgelegene
 * Primzahl (aufsteigend oder absteigend) finden.
 *
 * @author Andre Kloodt, Alexander Loeffler
 *
 */

#include <stdio.h>

/**
 * Gibt die Liste aller Fehlerfaelle an und referenziert auf
 * den Namen "Error" 
 * 
 */

typedef enum Error {
    ERR_NONE = 0,
    ERR_INVALID_OP = 1,
    ERR_INVALID_NUM = 2,
    ERR_PARA_COUNT = 3
} Error;

/**Erster Teil der Usage**/
char* const USAGE_1 = "Usage:\n"
"\n"
"  ueb01 -h\n"
"\n"
"  shows this help and exits.\n"
"\n"
"  - or -\n"
"\n"
"  ueb01 NUM OP\n";

/**Erster Teil der Usage**/
char* const USAGE_2 = "\n"
"  checks whether a given number is prime or finds the closest prime number\n"
"  (downwards or upwards from the given number). A call always consists of\n"
"  a number followed by an operation, where:\n"
"\n"
"  NUM has to be a positive integer number (including 0).\n"
"\n"
"  OP is one of:\n"
"    = checks whether NUM is prime or not\n"
"    + looks for the next prime number upwards from NUM (exclusive)\n"
"    - looks for the next prime number downwards from NUM (exclusive)\n";

/**
 * Gibt die Hilfe auf dem uebergebenen Stream (stdout/stderr) aus.
 *
 * @param[in] stream Ausgabestrom
 */
void printUsage (FILE * stream) {

	fprintf(stream, "%s%s",USAGE_1,USAGE_2);
}


/**
 * Händelt die Fehlerausgabe und gibt Abhängig vom übergebenen Fehler, eine Fehlernachricht aus.
 *
 * @param[in] err Fehlerfall
 */
Error error_handling(Error err){
    
    switch (err)
    {
        case ERR_INVALID_OP:
            fprintf(stderr, "Error: Parameter OP ist kein bekannter Operator.\n");
            printUsage(stderr);
            
            break;
        case ERR_INVALID_NUM:
            fprintf(stderr, "Error: Parameter NUM ist keine positive Ganzzahl.\n");
            printUsage(stderr);
            
            break;
        case ERR_PARA_COUNT:
            fprintf(stderr, "Error: Die Anzahl der Kommandozeilenparameter ist falsch.\n");
            printUsage(stderr);
          
            break;
        default:
            
            break;
    }

    return err;
}

/**
 * Überprüft, ob die Übergebene Zahl eine Primzahl ist
 *
 * @param[in] n die zu überprüfende Zahl
 * @return int 1 Wenn die Zahl prim ist, 0 wenn nicht
 */
int isPrime(unsigned int n){

	unsigned int i;
    
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if ((n%2 == 0) || (n%3 == 0)) {
        return 0;
    }

    for(i = 5; i*i <= n; i = i+6)
    {
        if ((n%i == 0) || (n%(i+2) == 0)) {
            return 0;
        }   
    }

    return 1;
}

/**
 * Sucht die nächst kleiner Primzahl als n
 *
 * @param[in] n die Zahl bei der wir anfangen
 * @return int returnt die gefundene Primzahl, 0 wenn es keine gibt
 */
int smallerPrime(unsigned int n){

	unsigned int i;
    
    if (n <= 2) {
        return 0;
    }
    
    if (n == 3) {
        return 2;
    }

    if (n%2 == 0) {
        n -= 1;
    }else{
        n -= 2;
    }

    for(i = n; i > 2; i = i-2)
    {
        if (isPrime(i) != 0) {
            return i;
        }
    }
    
    return 0;
}

/**
 * Sucht die nächst größer Primzahl als n
 *
 * @param[in] n die Zahl bei der wir anfangen
 * @return int returnt die gefundene Primzahl, 1 wenn 
 */
int biggerPrime(unsigned int n){
    
    if ((n == 0) || (n == 1)) {
        return 2;
    }
    
    if (n%2 == 0) {
        n += 1;
    }
    else {
        n += 2;
    }
	
	while(!isPrime(n)){    
        n += 2;
		}
	
    return n;
}

/**
 * Prüft die übergebenen Kommandozeilenparameter auf Fehler
 *
 * @param[in] argc Anzahl der Programmargumente.
 * @param[in] argv Array mit den Programmargumenten.
 * @return die Fehlernummer bzw den Fehler 
 */
int checkInput(int argc, char * argv[]){

    Error err = ERR_NONE;
    int i = 0;
    char z = ' ';
    char b = ' ';

	if (argc != 3 ) {
        err = ERR_PARA_COUNT;
      } else if ((sscanf(argv[2],"%c%c", &z , &b) != 1) || (z != '+' && z != '-' && z != '=')) { 
        err = ERR_INVALID_OP;
    } else 
    if ((sscanf(argv[1],"%i%c", &i, &z) != 1)  || (i < 0)) {
        err = ERR_INVALID_NUM;
    }
    
    return err;
}

/**
 * Hauptprogramm.
 *
 * Ueberpruft zunaechst die Eingaben auf gueltige Syntax und fuehrt
 * sofern kein Fehler aufgetreten ist die dem angegebenen Operator 
 * entsprechende Berechnung aus:
 *
 * + findet die naechstgroessere Primzahl von der angegebenen Zahl aus
 * - findet die naechstkleinere Primzahl von der angegebenen Zahl aus
 * = prueft ob die angegebene Zahl prim ist  
 *
 * Es wird entweder das Ergebnis der Berechnung ausgegeben oder eine 
 * entsprechende Fehlermeldung gefolgt von der Hilfe.
 *
 * @param[in] argc Anzahl der Programmargumente.
 * @param[in] argv Array mit den Programmargumenten.
 *
 * @return Fehlercode.
 */
int main (int argc, char * argv[]) {

    Error err = ERR_NONE;
    err = checkInput(argc, argv);
    
    if (err == ERR_NONE) {

        int number = 0;
        char catcher = ' ';
        int c = 0;

        sscanf(argv[1],"%i%c", &number, &catcher);
           
        switch (argv[2][0])
        {
            case '=':
                if(isPrime(number)){
                    fprintf(stdout, "*\\o/* The number %i is a prime number. *\\o/*\n", number);
                }else{
                    fprintf(stdout, "O_o The number %i is not a prime number. o_O\n", number);
                }
                break;
            case '+':
                fprintf(stdout, "+++ The next greater prime number to %i is: %d. +++\n", number,
                biggerPrime(number));
                
                break;
            case '-':
                
                if((c = smallerPrime(number)) != 0){
                    fprintf(stdout, "--- The next smaller prime number to %i is: %i. ---\n", number,
                c);
                }
                else{
                    fprintf(stdout, "%%%%%% There is no prime number smaller than %i. %%%%%%\n", number);
                }
                
                break;
        
            default:
                break;
        }

    }
    
    return error_handling(err);
}
