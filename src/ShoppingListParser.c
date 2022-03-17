#include "../include/LinkedListLib.h"
#include "../include/ShoppingListParser.h"

// * * * * * * * * * * * * * * * * * * * *
// ******  Functions Definitions  ********
// * * * * * * * * * * * * * * * * * * * *




// parse a shopping list file
// does not use flex/bison just simple string manipulation techniques in C
List * parseShoppingLists( char * fileName ){

  int maxLineLength = 1024;

  char * inputLine;

  if( fileName == NULL ){
    return NULL;
  }

  FILE * file = fopen(fileName, "r");

  if( file == NULL ){
    printf("problem openning file\n");
    return NULL;
  }

  // initialize a list of shopping lists
  List * shoppingLists = initializeList(shoppingListToString, deleteShoppingList, compareShoppingList);

  // start taking in lines lines of file
  inputLine = malloc(sizeof(char) * maxLineLength);

  while( fgets(inputLine, maxLineLength, file) != NULL ){
    // remove empty lines
    if( inputLine[0] != 10 && inputLine[0] != 13 ){
      // look for [ and
      if( inputLine[0] == '['){
        // create a list from it
        ShoppingList * tempSList = parseShoppingList(inputLine);
        insertBack(shoppingLists, tempSList);
      } else {
        // get each input and add it to the list after that until found another [ and ]
        ShoppingItem * tempSItem = parseShoppingItem(inputLine);

        if( tempSItem != NULL ){
          ShoppingList * currentSList = getFromBack(shoppingLists);
          insertBack(currentSList->shoppingItems, tempSItem);
        }
      }
    }
  }

  fclose(file);
  free(inputLine);

  return shoppingLists;
}






// * * * * * * * * * * * * * * * * * * * *
// *****  Shopping List Functions  *******
// * * * * * * * * * * * * * * * * * * * *





ShoppingList * createShoppingList( char * category ){

  if( category == NULL ){
    return NULL;
  }

  ShoppingList * sList = malloc(sizeof(ShoppingList));

  sList->category = malloc(strlen(category) + 1);
  strcpy(sList->category, category);

  sList->shoppingItems = initializeList(shoppingItemToString, deleteShoppingItem, compareShoppingItem);

  return sList;
}


char * shoppingListToString( void * data ){
  if( data == NULL ){
    return NULL;
  }

  ShoppingList * sList = data;

  char * itemsString = NULL;
  char * listString = NULL;

  itemsString = toString(sList->shoppingItems);

  listString = malloc(strlen(itemsString) + strlen(sList->category) + 50);

  sprintf(listString, "List:\n Category: %s\n Items:\n%s\n", sList->category, itemsString);

  free(itemsString);

  return listString;
}


void deleteShoppingList( void * data ){
  ShoppingList * sList = data;
  free(sList->category);
  freeList(sList->shoppingItems);
  free(sList);
}

int compareShoppingList( const void * first, const void * second ){
   return 0;
}




ShoppingList * parseShoppingList(char * inputLine){
  int i = 0;

  // remove the [
  inputLine++;

  // remove the ]
  while( inputLine[i] != ']'){
    i++;
  }

  inputLine[i] = '\0';

  ShoppingList * tempSList = createShoppingList(inputLine);

  return tempSList;
}







// * * * * * * * * * * * * * * * * * * * *
// *****  Shopping Item Functions  *******
// * * * * * * * * * * * * * * * * * * * *





ShoppingItem * createShoppingItem( char * name ){

  int loc = strcspn(name, "\r");

  if(loc != strlen(name)){
    name[loc] = 0;
  }

  loc = strcspn(name, "\n");

  if(loc != strlen(name)){
    name[loc] = 0;
  }



  ShoppingItem * tempSItem = malloc(sizeof(ShoppingItem));

  tempSItem->name = malloc(strlen(name) + 1);
  strcpy(tempSItem->name, name);

  tempSItem->synonyms = NULL;

  return tempSItem;
}


char * shoppingItemToString( void * data ){

  ShoppingItem * sItem = data;

  if( sItem == NULL ){
    return NULL;
  }

  int i = 0;
  char * synsString = NULL;
  char * itemString = NULL;

  // get all the synonyms
  char ** syns = sItem->synonyms;

  synsString = malloc(10);
  sprintf(synsString, " ");

  if( syns != NULL ){
    while( syns[i] != NULL ){
      synsString = realloc(synsString, strlen(synsString) + strlen(syns[i]) + 5);

      strcat(synsString, syns[i]);
      i++;
    }
  }

  itemString = malloc(strlen(synsString) + strlen(sItem->name) + 50);

  sprintf(itemString, "  Name: |%s|, Synonyms: %s", sItem->name, synsString);

  free(synsString);

  return itemString;
}


void deleteShoppingItem( void * data ){
  ShoppingItem * sItem = data;
  free(sItem->name);

  int i = 0;

  if(sItem->synonyms != NULL ){
    while(sItem->synonyms[i] != NULL){
      free(sItem->synonyms[i]);
      i++;
    }
  }

  free(sItem->synonyms);
  free(sItem);
}

int compareShoppingItem( const void * first, const void * second ){
  return 0;
}



ShoppingItem * parseShoppingItem( char * inputLine ){
  // check input
  if( inputLine == NULL || inputLine[0] == '\0' || inputLine[0] == '\n' ){
    return NULL;
  }


  // get the first string
  int i = 0;
  char * token;

  token = strtok(inputLine, "|");

  if( token == NULL ){
    return NULL;
  }

  // create the item
  ShoppingItem * tempSItem = createShoppingItem(token);

  // get the first next synonym if there is any
  token = strtok(NULL, "|");

  // if there is none return the item as is
  if(token == NULL){
    return tempSItem;
  }

  // otherwise add the synonym and continue adding them
  tempSItem->synonyms = malloc(sizeof(char *) * 2);

  tempSItem->synonyms[0] = malloc(sizeof(char) * strlen(token) + 1);
  strcpy(tempSItem->synonyms[0], token);
  tempSItem->synonyms[1] = NULL;

  i += 2;

  // add the other synonyms, terminate with NULL
  token = strtok(NULL, "|");

  while( token != NULL ){
    // reallocate to hold enough
    tempSItem->synonyms = realloc(tempSItem->synonyms, sizeof(char *) * (i + 1));

    // add the new token
    tempSItem->synonyms[i - 1] = malloc(sizeof(char) * strlen(token) + 1);
    strcpy(tempSItem->synonyms[i - 1], token);

    // null terminate
    tempSItem->synonyms[i] = NULL;

    i++;
    token = strtok(NULL, "|");
  }

  return tempSItem;
}
