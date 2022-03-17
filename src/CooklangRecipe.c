#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/CooklangRecipe.h"


void test2(){
  printf("Test working\n");
}


// * * * * * * * * * * * * * * * * * * * *
// ******** Metadata Functions ***********
// * * * * * * * * * * * * * * * * * * * *



Metadata * createMetadata( char * metaString ){

  // parse the metadata first
  char ** results = parseMetaString(metaString);

  char * identifier = results[0];
  char * content = results[1];

  // check valid input
  if( identifier == NULL || content == NULL ){
    return NULL;
  }

  Metadata * tempMeta = malloc(sizeof(Metadata));

  if( tempMeta == NULL ){
    printf("error, malloc failed - createMetadata1\n");
    return NULL;
  }

  // create space for and copy in the input strings
  tempMeta->identifier = strdup(identifier);
  tempMeta->content = strdup(content);

  free(results[0]);
  free(results[1]);
  free(results);

  return tempMeta;
}

// parse meta data function



void deleteMetadata( void * data ){
  Metadata * meta = data;

  free(meta->content);
  free(meta->identifier);
  free(meta);
}

char * metadataToString( void * data ){
  Metadata * meta = data;

  int length = strlen(meta->identifier) + strlen(meta->content) + 50;

  char * returnString = malloc(sizeof(char) * length);

  sprintf(returnString, "  -\"%s\": \"%s\"", meta->identifier, meta->content);

  return returnString;
}

int compareMetadata( const void * first, const void * second ){
  return 0;
}








// * * * * * * * * * * * * * * * * * * * *
// ******** Direction Functions **********
// * * * * * * * * * * * * * * * * * * * *





Direction * createDirection( char * type, char * value, char * amountString ){
  char ** amountResults = NULL;
  char * quantityString;
  char * unit;
  double quantity;

  // default amount value if there is none given
  char * defaultAmountString = malloc(20);
  sprintf(defaultAmountString, "some");

  // check input - must be a type
  if( type == NULL ){
    return NULL;
  }

  // value can only be null if the type is a timer, this case comes from a no name timer
  if( strcmp(type, "timer") != 0 && value == NULL ){
    return NULL;
  }

  // parse amountString
  if( amountString == NULL ){
    quantityString = NULL;
    quantity = -1;
    unit = NULL;

  } else {
    amountResults = parseAmountString(amountString);

    quantityString = amountResults[0];
    unit = amountResults[1];

    // check if quantity string can be a double
    if( quantityString == NULL ){
      quantity = -1;
    } else {
      // check if string is all numbers
      int check = checkIsNumber(quantityString);
      if( check == 1 ){
        quantity = strtod(quantityString, NULL);
      } else {
        quantity = -1;
      }
    }
  }

  // if quantity is a double, that means the quantity string can be represented as a double, so it should be saved as one
  // so convert it to a double if it can be else make it negative one and keep quantityString as is
  if( quantity > 0 ){
    quantityString = NULL;
  } else {
    quantity = -1;
  }

  Direction * tempDir = malloc(sizeof(Direction));

  if( tempDir == NULL ){
    printf("error, malloc failed - createDirection 1\n");
    return NULL;
  }

  tempDir->type = strdup(type);

  if( value != NULL ){
    tempDir->value = strdup(value);
  } else {
    tempDir->value = NULL;
  }

  // there is no quantity and the direction is done
  if( quantity == -1 && quantityString == NULL ){
    if( strcmp(type, "ingredient") == 0 ){
      tempDir->quantityString = strdup(defaultAmountString);
    } else {
      tempDir->quantityString = NULL;
    }
    tempDir->quantity = -1;
    tempDir->unit = NULL;

    if( amountResults != NULL){
      free(amountResults[0]);
      free(amountResults[1]);
      free(amountResults);
    }

    if( defaultAmountString != NULL ){
      free(defaultAmountString);
    }

    return tempDir;
  }

  if( quantity == -1 ){
    // quantity must be a string
    tempDir->quantityString = strdup(quantityString);
    tempDir->quantity = -1;
  } else {
    // must be a double
    tempDir->quantity = quantity;
    tempDir->quantityString = NULL;
  }

  // if unit input, set, else, set null
  if( unit != NULL ){
    tempDir->unit = strdup(unit);
  } else {
    tempDir->unit = NULL;
  }

  if( amountResults != NULL){
    free(amountResults[0]);
    free(amountResults[1]);
    free(amountResults);
  }

  if( defaultAmountString != NULL ){
    free(defaultAmountString);
  }

  return tempDir;
}


void deleteDirection( void * data ){
  Direction * dir = data;

  // free all the strings
  free(dir->type);
  free(dir->value);
  free(dir->quantityString);
  free(dir->unit);

  free(dir);
}

void dummyDeleteDirection( void * data){
  // does nothing
}

char * directionToString( void * data ){
  Direction * dir = data;

  char * tempString = NULL;

  int length = 0;

  if( dir->type != NULL ){
    length += strlen(dir->type);
  }

  if( dir->value != NULL ){
    length += strlen(dir->value);
  } else {
    length += 50;
  }

  if( dir->quantityString != NULL ){
    length += strlen(dir->quantityString);
  }

  if( dir->unit != NULL ){
    length += strlen(dir->unit);
  } else {
    length += 50;
  }

  length += 100;

  if( length != 15 ){
    tempString = malloc( sizeof(char) * length );
  } else {
    return NULL;
  }

  if( tempString == NULL ){
    printf("error, malloc failed - directionToString1\n");
    return NULL;
  }

  // make an output string based on what kind it is

  // if its a text iten use value instead of name
  if( strcmp(dir->type, "text") == 0 ){
    sprintf(tempString, "      - type: text\n      - value: \"%s\"\n", dir->value);

  // timers, cookware, and ingredients
  } else {
    // name and value
    if( dir->value != NULL ){
      sprintf(tempString, "      - type: %s\n      - name: \"%s\"\n", dir->type, dir->value);
    } else {
      sprintf(tempString, "      - type: %s\n      - name: \"\"\n", dir->type);
    }

    // quantity, string format
    if( dir->quantityString != NULL ){
      char * tempQuanString = malloc(sizeof(char) * strlen(dir->quantityString) + 30);
      sprintf(tempQuanString, "      - quantity: \"%s\"\n", dir->quantityString);

      strcat(tempString, tempQuanString);
      free(tempQuanString);
    // quantity, double format
    } else if( dir->quantity != -1 ){
      char * tempQuanString = malloc(sizeof(char) * 30);
      sprintf(tempQuanString, "      - quantity: %.3f\n", dir->quantity);

      strcat(tempString, tempQuanString);
      free(tempQuanString);
    // no quantity - end of string
    } else {
      char * tempQuanString = malloc(sizeof(char) * 30);
      sprintf(tempQuanString, "      - quantity: \"\"\n");

      strcat(tempString, tempQuanString);
      free(tempQuanString);
      return tempString;
    }

    // add the unit if there is one, else add empty units
    // only ingredients/timers
    if( strcmp(dir->type, "cookware") != 0 ){
      if( dir->unit != NULL ){
        char * unitString = malloc(sizeof(dir->unit) + 20);
        sprintf(unitString, "      - units: \"%s\"\n", dir->unit);

        strcat(tempString, unitString);
        free(unitString);
      } else {
        char * unitString = malloc(sizeof(char) * 50);
        strcpy(unitString, "      - units: \"\"\n");

        strcat(tempString, unitString);
        free(unitString);
      }
    }
  }

  return tempString;
}

int compareDirections( const void * first, const void * second ){
  return 0;
}





// * * * * * * * * * * * * * * * * * * * *
// *********  Step Functions  ************
// * * * * * * * * * * * * * * * * * * * *


Step * createStep(){

  // initalize a list of directions
  Step * tempStep = malloc(sizeof(Step));

  List * dirList = initializeList( directionToString, deleteDirection, compareDirections );
  tempStep->directions = dirList;

  List * ingredientList = initializeList(ingredientToString, dummyDeleteDirection, compareDirections);
  tempStep->ingredientList = ingredientList;

  List * equipmentList = initializeList(ingredientToString, dummyDeleteDirection, compareDirections);
  tempStep->equipmentList = equipmentList;

  return tempStep;
}

void deleteStep( void * data ){
  Step * step = data;

  freeList(step->directions);
  freeList(step->ingredientList);
  freeList(step->equipmentList);
  free(step);
}

char * stepToString( void * data ){
  Step * step = data;

  char * stepString;
  char * dirString;
  char * ingredientString;
  char * equipmentString;

  int length = 0;

  // get the directions string
  if( getLength(step->directions) != 0 ){
    dirString = toStringDelim(step->directions, "    - Direction");
  } else {
    stepString = malloc(sizeof(char) * 20);
    sprintf(stepString, "Empty step}\n");
    return stepString;
  }

  // get the ingredients string
  if( getLength(step->ingredientList) != 0 ){
    ingredientString = toString(step->ingredientList);
  } else {
    ingredientString = malloc(20);
    sprintf(ingredientString, " ");
  }

  // get the equipment string
  if( getLength(step->equipmentList) != 0 ){
    equipmentString = toString(step->equipmentList);
  } else {
    equipmentString = malloc(20);
    sprintf(equipmentString, " ");
  }

  // find the length the string should be
  length += strlen(dirString);
  length += strlen(ingredientString);
  length += strlen(equipmentString);
  length += 50;

  // make the string
  stepString = malloc(sizeof(char) * length);

  sprintf(stepString, "%s\n", dirString);

  free(dirString);
  free(ingredientString);
  free(equipmentString);

  return stepString;
}

int compareSteps( const void * first, const void * second){
  return 0;
}





// * * * * * * * * * * * * * * * * * * * *
// ********  Recipe Functions  ***********
// * * * * * * * * * * * * * * * * * * * *

// creation functions

Recipe * createRecipe(){

  // make a new recipe
  Recipe * tempRec = malloc(sizeof(Recipe));

  // steps
  List * stepList = initializeList(stepToString, deleteStep, compareSteps);
  tempRec->stepList = stepList;

  // metadata
  List * metaDataList = initializeList(metadataToString, deleteMetadata, compareMetadata);
  tempRec->metaData = metaDataList;

  return tempRec;
}

void deleteRecipe( void * data ){
  // free all the lists

}

char * recipeToString( void * data ){
  return "empty recipe\n";
}



// * * * * * * * * * * * * * * * * * * * *
// ********   Other Functions  ***********
// * * * * * * * * * * * * * * * * * * * *


// print ingredient - used for the ingredient list instead of a the usual direction printing function
// also works for cookware
char * ingredientToString( void * data ){
  Direction * ingredient = data;

  if( ingredient == NULL ){
    return NULL;
  }

  int length = 0;
  char * ingrString = NULL;
  char * quanString = NULL;
  char * unitString = NULL;

  // need value, quantity and unit if any
  length += strlen(ingredient->value);

  // if uses double  type
  if(ingredient->quantity != -1){
    length += 10;
  }

  // if uses string type
  if( ingredient->quantityString != NULL ){
    length += strlen(ingredient->quantityString);
  }

  // check for unit
  if(ingredient->unit != NULL){
    length += strlen(ingredient->unit);
  }

  length += 50;

  // start making string
  ingrString = malloc(sizeof(char) * length);

  if(ingrString == NULL){
    printf("error, malloc failed - ingredientToString11\n");
    return NULL;
  }

  sprintf(ingrString, "%s", ingredient->value);

  // add quantity string
  if( ingredient->quantity == -1 ){
    if( ingredient->quantityString != NULL ){
      quanString = malloc(sizeof(char) * strlen(ingredient->quantityString) + 20);
      sprintf(quanString, ": %s", ingredient->quantityString);
    }
  } else {
    quanString = malloc(sizeof(char) * 20);
    sprintf(quanString, ": %.3f", ingredient->quantity);
  }

  // there is no quantity and no quantity string, therefore no units
  if( quanString == NULL ){
    return ingrString;
  } else {
    strcat(ingrString, quanString);
  }

  // add the unit string if there is one
  // if there isnt
  if( ingredient->unit == NULL ){
    free(unitString);
    free(quanString);
    return ingrString;

  // if there is
  } else {
    unitString = malloc(sizeof(char) * strlen(ingredient->unit) + 20);
    sprintf(unitString, " %s", ingredient->unit);
  }

  strcat(ingrString, unitString);

  free(unitString);
  free(quanString);

  return ingrString;
}




// print cookware - used for the ingredient list instead of a the usual direction printing function


// amount can be:
//  - just a number - number + unit - just a word - word + unit - just a multiword - multiword + unit
// parses an amount string to get the unit and quantity
char ** parseAmountString( char * amountString ){

  if( amountString == NULL ){
    return NULL;
  }

  char ** results = malloc(sizeof(char *) * 2);
  char * quantityDest = NULL;
  char * unitDest = NULL;

  // check if empty - an empty amount - if so set null results and return
  if( amountString[0] == '\0' ){
    results[0] = NULL;
    results[1] = NULL;
    return results;
  }

  // all other cases either string or string + unit string
  char * token = strtok(amountString, "%");

  if( token != NULL ){
    quantityDest = malloc(sizeof(char) * strlen(token) + 1);
    strcpy(quantityDest, token);
  } else {
    // no quantity therefore no unit, set null results and return
    results[0] = NULL;
    results[1] = NULL;
    return results;
  }

  token = strtok(NULL, "%");

  if( token != NULL ){
    unitDest = malloc(sizeof(char) * strlen(token) + 1);
    strcpy(unitDest, token);
  } else {
    unitDest = NULL;
  }

  // remove white space
  char * trimmedQuan = trimWhiteSpace(quantityDest);
  results[0] = strdup(trimmedQuan);
  free(trimmedQuan);

  if( unitDest != NULL ){
    char * trimmedUnit = trimWhiteSpace(unitDest);
    results[1] = strdup(trimmedUnit);
    free(trimmedUnit);
  } else {
    results[1] = NULL;
  }

  free(quantityDest);
  free(unitDest);

  return results;
}


// parses a meta data string to get the identifier and content of metadata
char ** parseMetaString( char * metaString ){
  char * beginning;
  char ** results;
  int i = 0;

  if( metaString == NULL ){
    return NULL;
  }

  // remove first two '>'
  if( metaString[0] == '>' && metaString[1] == '>' ){
    metaString++;
    metaString++;
  }
  beginning = metaString;

  results = malloc(sizeof(char *) * 2);
  if( results == NULL) {
    printf("error, malloc failed - parseMetaString1");
  }

  // loop through to find the first ':' then set to end of string
  while( metaString[i] != ':' && metaString[i] != '\0'){
    metaString++;
  }
  metaString[i] = '\0';

  // copy the into the identifier
  char * trimmed = trimWhiteSpace(beginning);
  results[0] = strdup(trimmed);
  free(trimmed);

  metaString++;
  // copy the remainder into the content
  trimmed = trimWhiteSpace(metaString);
  results[1] = strdup(trimmed);
  free(trimmed);

  return results;
}






// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.

// function provided courtesy of stack overflow users Adam Rosenfield and Dave Gray, found here:
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
// slightly modified to remove the above issue
char * trimWhiteSpace(char * str){
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  char * output = strdup(str);

  return output;
}


// check is if a given string is all numbers -  and can therefore be used in strtod
// 1 is a true
int checkIsNumber( char * input ){
  int i = 0;

  while( input[i] != '\0' ){
    if( isdigit(input[i]) == 0 ){
      return 0;
    }
    i++;
  }
  return 1;
}
