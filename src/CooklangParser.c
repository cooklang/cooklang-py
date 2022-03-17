
#include <stdlib.h>
#include <stdio.h>

#include "../include/CooklangParser.h"
#include "../Cooklang.tab.h"


extern FILE * yyin;


// wrapper functions
Recipe * parseRecipe( char * fileName ){

  FILE * file;

  if( fileName != NULL ){
    file = fopen(fileName, "r");
    if( file != NULL ){
      yyin = file;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }


  // setup the recipe
  Recipe * finalRecipe = createRecipe();

  // create the first step
  Step * currentStep = createStep();
  insertBack(finalRecipe->stepList, currentStep);

  yyparse(finalRecipe);

  fclose(file);

  // // initialize the iterators on the recipe
  // if( finalRecipe->stepList != NULL ){
  //   finalRecipe->stepIter = createIterator(finalRecipe->stepList);
  // } else {
  //   finalRecipe->stepIter = NULL;
  // }
  return finalRecipe;
}



// function to get the i-th step in a recipe

// function to get the i-th direction in a step

// function to return the number of steps in a recipe

// function to return the number of direction in a step

// function to nicely print the step data

// function to nicely print the direction data






char * addTwoStrings(char * first, char * second){
  char * result = NULL;
  int length = 0;

  if( first == NULL || second == NULL){
    return NULL;
  }

  length += strlen(first);
  length += strlen(second);
  length += 5;

  result = malloc(sizeof(char) * length);

  if( result == NULL ){
    printf("Error, malloc failed");
    return NULL;
  }

  sprintf(result, "%s%s", first, second);


  return result;
}


char * addThreeStrings(char * first, char * second, char * third){
  char * result = NULL;
  int length = 0;

  if( first == NULL || second == NULL || third == NULL ){
    return NULL;
  }

  length += strlen(first);
  length += strlen(second);
  length += strlen(third);
  length += 5;

  result = malloc(sizeof(char) * length);

  if( result == NULL ){
    printf("Error, malloc failed");
    return NULL;
  }

  sprintf(result, "%s%s%s", first, second, third);


  return result;
}


// adds the new direction corresponding to arguments 2-6, to the recipe in argument 1
// always adds to the last step in the list
void addDirection( Recipe * recipe, char * type, char * value, char * amountString ){

  // create a direction then add it to the direction list
  Direction * tempDir = createDirection(type, value, amountString);

  Step * step = getFromBack(recipe->stepList);

  if( tempDir != NULL ){
    insertBack(step->directions, tempDir);
  }

  // if cookware/ingredient, add to the appropriate lists
  if( strcmp(type, "Cookware") == 0 ){
    insertBack(step->equipmentList, tempDir);
  }

  if( strcmp(type, "Ingredient") == 0 ){
    insertBack(step->ingredientList, tempDir);
  }
}


void addMetaData( Recipe * recipe, char * metaDataString ){

  // create a new metadata and add it to the list at the back
  Metadata * tempMeta = createMetadata(metaDataString);

  if( tempMeta != NULL ){
    insertBack(recipe->metaData, tempMeta);
  }
}
