#include <Python.h>

#include "../include/CooklangParser.h"
#include "../include/ShoppingListParser.h"

// python wrapper methods

static PyObject * methodPrintRecipe(PyObject * self, PyObject * args){

  Py_ssize_t i;
  Py_ssize_t j;
  Py_ssize_t length;
  Py_ssize_t stepLength;
  Py_ssize_t pos = 0;

  const char * keyStr;
  const char * valStr;
  const char * nameStr;
  const char * quanStr;
  const char * unitStr;
  const char * typeStr;

  PyObject * key;
  PyObject * value;
  PyObject * str;
  PyObject * attr;
  PyObject * metas;
  PyObject * ingredient;
  PyObject * ingredients;
  PyObject * cookware;
  PyObject * cookwares;
  PyObject * direction;
  PyObject * step;
  PyObject * steps;
  PyObject * recipe;

  if( !PyArg_ParseTuple(args, "O", &recipe ) ){
    printf("Error printing recipe - argument could not be read\n");
    return NULL;
  }

  // print metadata
  printf("Metadata:\n");

  metas = PyDict_GetItemString(recipe, "metadata");
  while(PyDict_Next(metas, &pos, &key, &value)){

    str = PyUnicode_AsEncodedString(key, "utf-8", "~E~");
    keyStr = PyBytes_AS_STRING(str);

    str = PyUnicode_AsEncodedString(value, "utf-8", "~E~");
    valStr = PyBytes_AS_STRING(str);

    printf("  - %s: %s \n", keyStr, valStr);
  }



  // print ingredients
  printf("\nIngredients:\n");

  ingredients = PyDict_GetItemString(recipe, "ingredients");

  length = PyList_Size(ingredients);
  i = 0;
  if( length != 0 ){

    while( i < length ){
      ingredient = PyList_GetItem(ingredients, i);

      // name
      attr = PyDict_GetItemString(ingredient, "name");
      str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
      nameStr = PyBytes_AS_STRING(str);

      // quanitty
      attr = PyDict_GetItemString(ingredient, "quantity");
      str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
      quanStr = PyBytes_AS_STRING(str);

      // units
      attr = PyDict_GetItemString(ingredient, "units");
      str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
      unitStr = PyBytes_AS_STRING(str);

      printf("  - %s, %s %s\n", nameStr, quanStr, unitStr);

      i++;
    }

  // if there are no ingredients
  } else {
    printf("  none\n");
  }



  // print cookware
  printf("\nCookware\n");

  cookwares = PyDict_GetItemString(recipe, "cookware");

  length = PyList_Size(cookwares);
  i = 0;

  if( length != 0 ){

    while( i < length ){
      cookware = PyList_GetItem(cookwares, i);

      // name
      attr = PyDict_GetItemString(cookware, "name");
      str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
      nameStr = PyBytes_AS_STRING(str);

      // quanitty
      attr = PyDict_GetItemString(cookware, "quantity");
      str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
      quanStr = PyBytes_AS_STRING(str);

      printf("  - %s %s\n", quanStr, nameStr );

      i++;
    }
  } else {
    printf("  none\n");
  }



  // print steps
  printf("\nSteps\n");

  steps = PyDict_GetItemString(recipe, "steps");

  length = PyList_Size(steps);
  i = 0;

  // check that there are steps
  if( length != 0 ){

    // loop through each step
    while( i < length ){

      step = PyList_GetItem(steps, i);

      printf("  - %ld:\n", (i+1));

      stepLength = PyList_Size(step);
      j = 0;

      // loop through each step's directions
      while( j < stepLength){
        direction = PyList_GetItem(step, j);

        printf("    - Direction %ld:\n", (j+1));

        // get type
        attr = PyDict_GetItemString(direction, "type");
        str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
        typeStr = PyBytes_AS_STRING(str);



        if( strcmp(typeStr, "text") == 0 ){
          printf("      - type: %s\n", typeStr);

          // get value
          attr = PyDict_GetItemString(direction, "value");
          str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
          valStr = PyBytes_AS_STRING(str);

          printf("      - %s\n", valStr);

        } else {
          printf("      - type:     %s\n", typeStr);

          // print value
          attr = PyDict_GetItemString(direction, "name");
          str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
          nameStr = PyBytes_AS_STRING(str);

          printf("      - name:     %s\n", nameStr);



          // print quan
          attr = PyDict_GetItemString(direction, "quantity");
          str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
          quanStr = PyBytes_AS_STRING(str);

          printf("      - quantity: %s\n", quanStr);


          // print units
          if( strcmp(typeStr, "cookware") != 0 ){
            attr = PyDict_GetItemString(direction, "units");
            str = PyUnicode_AsEncodedString(attr, "utf-8", "~E~");
            unitStr = PyBytes_AS_STRING(str);

            printf("      - units:    %s\n", unitStr);
          }

        }

        j++;
      }

      i++;
    }
  } else {
    printf("  none\n");
  }


  Py_INCREF(Py_None);
  return Py_None;
}


// parse a recipe
static PyObject * methodParseRecipe(PyObject *self, PyObject * args){
  int check;
  int dirLength;
  char * fileName = NULL;

  ListIterator stepIter;
  Step * curStep;
  ListIterator dirIter;
  Direction * curDir;
  ListIterator metaIter;
  Metadata * curMeta;

  PyObject * metaListObject;

  PyObject * stepListObject;
  PyObject * stepObject;
  PyObject * directionObject;

  PyObject * ingredientListObject;
  PyObject * cookwareListObject;


  // get args - no embedded null code points
  if(!PyArg_ParseTuple(args, "s", &fileName)){
    return NULL;
  }

  // parse the recipe recipe
  Recipe * parsedRecipe = parseRecipe(fileName);

  // build a python object to represent the recipe
  PyObject * recipeObject = PyDict_New();




  // add all the metadata

  // loop through each metadata
  metaIter = createIterator(parsedRecipe->metaData);
  curMeta = nextElement(&metaIter);

  metaListObject = PyDict_New();

  while( curMeta != NULL ){
    // add it to the list
    check = PyDict_SetItemString(metaListObject, curMeta->identifier, PyUnicode_FromString(curMeta->content));
    if( check == -1 ){
      printf("Error adding new meta data to list\n");
    }

    // get the next one
    curMeta = nextElement(&metaIter);
  }


  // add the whole list to the final object
  check = PyDict_SetItemString(recipeObject, "metadata", metaListObject);
  if( check == -1 ){
    printf("Error adding meta data list to recipe object\n");
  }




  // add all the steps

  // the lists to store steps/ingredients/cookware
  stepListObject = PyList_New(0);
  ingredientListObject = PyList_New(0);
  cookwareListObject = PyList_New(0);

  // loop through every step
  stepIter = createIterator(parsedRecipe->stepList);
  curStep = nextElement(&stepIter);

  while( curStep != NULL ){
    // only add non-empty steps
    dirLength = getLength(curStep->directions);

    if(dirLength > 0 ){

      // loop through every direction
      dirIter = createIterator(curStep->directions);
      curDir = nextElement(&dirIter);

      // create a new list of directions
      stepObject = PyList_New(0);

      while( curDir != NULL ){
        // build a pyobject out of its values

        // if its a text direction
        if( strcmp(curDir->type, "text") == 0 ){
          directionObject = Py_BuildValue("{s:s, s:s}", "type", "text", "value", curDir->value);

        // other kinds
        } else {
          directionObject = PyDict_New();
          // add each attribute of the direction to the pyobject

          // type
          check = PyDict_SetItemString(directionObject, "type", PyUnicode_FromString(curDir->type));
          if( check == - 1 ){
            printf("Error adding type to new direction object\n");
          }

          // value
          if( curDir->value != NULL ){
            check = PyDict_SetItemString(directionObject, "name", PyUnicode_FromString(curDir->value));
            if( check == - 1 ){
              printf("Error adding name to new direction object\n");
            }
          } else {
            check = PyDict_SetItemString(directionObject, "name", PyUnicode_FromString(""));
            if( check == - 1 ){
              printf("Error adding name to new direction object\n");
            }
          }

          // quantity
          if( curDir->quantityString == NULL ){
            if( curDir->quantity != -1 ){
              check = PyDict_SetItemString(directionObject, "quantity", PyFloat_FromDouble(curDir->quantity));
              if( check == - 1 ){
                printf("Error adding quantity double to new direction object\n");
              }
            } else {
              check = PyDict_SetItemString(directionObject, "quantity", PyUnicode_FromString(""));
              if( check == - 1 ){
                printf("Error adding empty quantity to new direction object\n");
              }
            }
          } else {
            check = PyDict_SetItemString(directionObject, "quantity", PyUnicode_FromString(curDir->quantityString));
            if( check == - 1 ){
              printf("Error adding quantity string to new direction object\n");
            }
          }

          // unit
          if(strcmp(curDir->type, "cookware") != 0 ){
            if( curDir->unit != NULL ){
              check = PyDict_SetItemString(directionObject, "units", PyUnicode_FromString(curDir->unit));
              if( check == - 1 ){
                printf("Error adding units to new direction object\n");
              }
            } else {
              check = PyDict_SetItemString(directionObject, "units", PyUnicode_FromString(""));
              if( check == - 1 ){
                printf("Error adding units to new direction object\n");
              }
            }
          }
        }

        // add it to the step
        check = PyList_Append(stepObject, directionObject);
        if(check == -1){
          printf("Error adding new direction to direction list\n");
        }

        // if its an ingredient add it to the ingredient list
        if( strcmp(curDir->type, "ingredient") == 0 ){
         check = PyList_Append(ingredientListObject, directionObject);

          if(check == -1){
            printf("Error adding new direction to direction list\n");
          }
        }

        // if its a cookware add it to the cookware list
        if( strcmp(curDir->type, "cookware") == 0 ){
         check = PyList_Append(cookwareListObject, directionObject);

          if(check == -1){
            printf("Error adding new direction to direction list\n");
          }
        }

        // get the next direction
        curDir = nextElement(&dirIter);
      }
      check = PyList_Append(stepListObject, stepObject);
      if(check == -1){
        printf("Error adding step to step list\n");
      }
    }

    // get the next step
    curStep = nextElement(&stepIter);
  }

  // add ingredient list to the recipe
  check = PyDict_SetItemString(recipeObject, "ingredients", ingredientListObject);
  if(check == -1){
    printf("Error adding step list to recipe\n");
  }

  // add cookware list to the recipe
  check = PyDict_SetItemString(recipeObject, "cookware", cookwareListObject);
  if(check == -1){
    printf("Error adding step list to recipe\n");
  }

  //add steps to the recipe
  check = PyDict_SetItemString(recipeObject, "steps", stepListObject);
  if(check == -1){
    printf("Error adding step list to recipe\n");
  }

  // return
  return (PyObject *) recipeObject;
}


static PyObject * methodParseShoppingList(PyObject *self, PyObject * args){

  int check;
  int synCount = 0;

  char * fileName;

  List * shoppingLists;

  ListIterator sListIter;
  ShoppingList * curList;
  ListIterator itemIter;
  ShoppingItem * curItem;

  PyObject * shopItemObject;
  PyObject * synListObject;
  PyObject * itemListObject;
  PyObject * shopListObject;

  PyObject * shopListList;

  // get args - no embedded null code points
  if(!PyArg_ParseTuple(args, "s", &fileName)){
    return NULL;
  }


  // parse the shopping list
  shoppingLists = parseShoppingLists(fileName);


  // convert to python object - a list of lists
  shopListList = PyList_New(0);

  // loop through each shopping list
  sListIter = createIterator(shoppingLists);
  curList = nextElement(&sListIter);

  while( curList != NULL ){

    // create a new pyobject for the list
    shopListObject = PyDict_New();

    // add the category
    check = PyDict_SetItemString(shopListObject, "category", PyUnicode_FromString(curList->category));
    if( check == - 1 ){
      printf("Error adding name to item object\n");
    }

    // add each shopping item to the list
    itemListObject = PyList_New(0);
    // loop through each shopping item
    itemIter = createIterator(curList->shoppingItems);
    curItem = nextElement(&itemIter);

    while( curItem != NULL ){

      // make new pyobject for item
      shopItemObject = PyDict_New();
      check = PyDict_SetItemString(shopItemObject, "name", PyUnicode_FromString(curItem->name));
      if( check == - 1 ){
        printf("Error adding name to item object\n");
      }

      // loop through each synonym
      synListObject = PyList_New(0);
      if( curItem->synonyms != NULL ){

        while( curItem->synonyms[synCount] != NULL ){

          check = PyList_Append(synListObject, PyUnicode_FromString(curItem->synonyms[synCount]));
          if( check == -1){
            printf("Error adding new synonym to synonym list\n");
          }

          synCount++;
        }
      }

      // add the synonyms to the item
      check = PyDict_SetItemString(shopItemObject, "synonyms", synListObject);
      if( check == - 1 ){
        printf("Error adding synonyms to item\n");
      }

      // add the item to the list of items for the shopping list
      check = PyList_Append(itemListObject, shopItemObject);
      if( check == - 1 ){
        printf("Error adding new shopping item to shopping list\n");
      }

      curItem = nextElement(&itemIter);
    }

    // add the list of shopping items to the shopping list
    check = PyDict_SetItemString(shopListObject, "items", itemListObject);
    if( check == - 1 ){
      printf("Error adding list of items to shopping list\n");
    }


    // add the shopping list to the list of lists
    check = PyList_Append(shopListList, shopListObject);
    if( check == - 1 ){
      printf("Error adding new shopping list to list of shopping lists\n");
    }

    curList = nextElement(&sListIter);
  }


  return (PyObject *) shopListList;
}





// python module methods array
static PyMethodDef cooklangMethods[] = {
  {"parseRecipe", methodParseRecipe, METH_VARARGS, "Python wrapper function that parses recipes written in the cooklang language specification."},
  {"parseShoppingList", methodParseShoppingList, METH_VARARGS, "Python wrapper function that parses shopping lists written in the cooklang language specification."},
  {"printRecipe", methodPrintRecipe, METH_VARARGS, "Python wrapper function for printing the contents of a recipe."},
  {NULL, NULL, 0, NULL}
};

// define the module
static PyModuleDef cooklang = {
    PyModuleDef_HEAD_INIT,
    .m_name = "cooklang",
    .m_doc = "A python module that uses C to parse recipe files using cooklang.",
    .m_size = -1,
    cooklangMethods
};

// initialization function
PyMODINIT_FUNC PyInit_cooklang(void){
  return PyModule_Create(&cooklang);
}
