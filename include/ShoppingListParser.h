

// shopping list
typedef struct {

  char * category;
  List * shoppingItems;

} ShoppingList;

typedef struct {
  
  // required, the first name
  char * name;

  // any other synonyms
  char ** synonyms;

} ShoppingItem;



List * parseShoppingLists( char * fileName );


ShoppingList * createShoppingList( char * category );
char * shoppingListToString( void * data );
void deleteShoppingList( void * data );
int compareShoppingList( const void * first, const void * second );
ShoppingList * parseShoppingList(char * inputLine);


ShoppingItem * createShoppingItem( char * name );
char * shoppingItemToString( void * data );
void deleteShoppingItem( void * data );
int compareShoppingItem( const void * first, const void * second );
ShoppingItem * parseShoppingItem( char * inputLine );
