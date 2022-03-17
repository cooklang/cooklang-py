#ifndef INCLUDED_PAR
#define INCLUDED_PAR

#include "LinkedListLib.h"
#include "CooklangRecipe.h"

#endif



// wrappers
Recipe * parseRecipe( char * fileName );


// others
char * addTwoStrings(char * first, char * second);
char * addThreeStrings(char * first, char * second, char * third);
void addDirection( Recipe * recipe, char * type, char * value, char * amountString );
void addMetaData( Recipe * recipe, char * metaDataString );
