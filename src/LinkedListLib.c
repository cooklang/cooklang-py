
#include "../include/LinkedListLib.h"
#include "assert.h"

/** Function to initialize the list metadata head to the appropriate function pointers. Allocates memory to the struct.
*@return pointer to the list head
*@param printFunction function pointer to print a single node of the list
*@param deleteFunction function pointer to delete a single piece of data from the list
*@param compareFunction function pointer to compare two nodes of the list in order to test for equality or order
**/
List * initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second)){
    //Asserts create a partial function...
    assert(printFunction != NULL);
    assert(deleteFunction != NULL);
    assert(compareFunction != NULL);

    List * tmpList = malloc(sizeof(List));

	tmpList->head = NULL;
	tmpList->tail = NULL;

	tmpList->length = 0;

	tmpList->deleteData = deleteFunction;
	tmpList->compare = compareFunction;
	tmpList->printData = printFunction;

	return tmpList;
}


/** Deletes the entire linked list, freeing all memory.
* uses the supplied function pointer to release allocated memory for the data
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the List-type dummy node
*@return  on success: NULL, on failure: head of list
**/
void freeList(List* list){

    clearList(list);
	free(list);
}

/** Clears the list: frees the contents of the list - Node structs and data stored in them -
 * without deleting the List struct
 * uses the supplied function pointer to release allocated memory for the data
 * @pre 'List' type must exist and be used in order to keep track of the linked list.
 * @post List struct still exists, list head = list tail = NULL, list length = 0
 * @param list pointer to the List-type dummy node
 * @return  on success: NULL, on failure: head of list
**/
void clearList(List* list){
    if (list == NULL){
		return;
	}

	if (list->head == NULL && list->tail == NULL){
		return;
	}

	Node* tmp;

	while (list->head != NULL){
		list->deleteData(list->head->data);
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}

	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
}

/**Function for creating a node for the linked list.
* This node contains abstracted (void *) data as well as previous and next
* pointers to connect to other nodes in the list
* @pre data should be of same size of void pointer on the users machine to avoid size conflicts. data must be valid.
* data must be cast to void pointer before being added.
* @post data is valid to be added to a linked list
* @return On success returns a node that can be added to a linked list. On failure, returns NULL.
* @param data - is a void * pointer to any data type.  Data must be allocated on the heap.
**/
Node* initializeNode(void* data){
	Node* tmpNode = (Node*)malloc(sizeof(Node));

	if (tmpNode == NULL){
		return NULL;
	}

	tmpNode->data = data;
	tmpNode->previous = NULL;
	tmpNode->next = NULL;

	return tmpNode;
}

/**Inserts a Node at the front of a linked list.  List metadata is updated
* so that head and tail pointers are correct.
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the dummy head of the list
*@param toBeAdded a pointer to data that is to be added to the linked list
**/
void insertBack(List* list, void* toBeAdded){
	if (list == NULL || toBeAdded == NULL){
		return;
	}

	(list->length)++;

	Node* newNode = initializeNode(toBeAdded);

    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
		newNode->previous = list->tail;
        list->tail->next = newNode;
    	list->tail = newNode;
    }
}

/**Inserts a Node at the front of a linked list.  List metadata is updated
* so that head and tail pointers are correct.
*@pre 'List' type must exist and be used in order to keep track of the linked list.
*@param list pointer to the dummy head of the list
*@param toBeAdded a pointer to data that is to be added to the linked list
**/
void insertFront(List* list, void* toBeAdded){
	if (list == NULL || toBeAdded == NULL){
		return;
	}

	(list->length)++;

	Node* newNode = initializeNode(toBeAdded);

    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
		newNode->next = list->head;
        list->head->previous = newNode;
    	list->head = newNode;
    }
}

/**Returns a pointer to the data at the front of the list. Does not alter list structure.
 *@pre The list exists and has memory allocated to it
 *@param the list struct
 *@return pointer to the data located at the head of the list
 **/
void* getFromFront(List * list){
	if (list->head == NULL){
		return NULL;
	}

	return list->head->data;
}

/**Returns a pointer to the data at the back of the list. Does not alter list structure.
 *@pre The list exists and has memory allocated to it
 *@param the list struct
 *@return pointer to the data located at the tail of the list
 **/
void* getFromBack(List * list){
	if (list->tail == NULL){
		return NULL;
	}

	return list->tail->data;
}

void* deleteDataFromList(List* list, void* toBeDeleted){
	if (list == NULL || toBeDeleted == NULL){
		return NULL;
	}

	Node* tmp = list->head;

	while(tmp != NULL){
		if (list->compare(toBeDeleted, tmp->data) == 0){
			//Unlink the node
			Node* delNode = tmp;

			if (tmp->previous != NULL){
				tmp->previous->next = delNode->next;
			}else{
				list->head = delNode->next;
			}

			if (tmp->next != NULL){
				tmp->next->previous = delNode->previous;
			}else{
				list->tail = delNode->previous;
			}

			void* data = delNode->data;
			free(delNode);

			(list->length)--;

			return data;

		}else{
			tmp = tmp->next;
		}
	}

	return NULL;
}


/** Uses the comparison function pointer to place the element in the
* appropriate position in the list.
* should be used as the only insert function if a sorted list is required.
*@pre List exists and has memory allocated to it. Node to be added is valid.
*@post The node to be added will be placed immediately before or after the first occurrence of a related node
*@param list a pointer to the dummy head of the list containing function pointers for delete and compare, as well
as a pointer to the first and last element of the list.
*@param toBeAdded a pointer to data that is to be added to the linked list
**/
void insertSorted(List *list, void *toBeAdded){
	if (list == NULL || toBeAdded == NULL){
		return;
	}

	if (list->head == NULL){
		insertBack(list, toBeAdded);
		return;
	}

	if (list->compare(toBeAdded, list->head->data) <= 0){
		insertFront(list, toBeAdded);
		return;
	}

	if (list->compare(toBeAdded, list->tail->data) > 0){
		insertBack(list, toBeAdded);
		return;
	}

	Node* currNode = list->head;

	while (currNode != NULL){
		if (list->compare(toBeAdded, currNode->data) <= 0){

			char* currDescr = list->printData(currNode->data);
			char* newDescr = list->printData(toBeAdded);

			//printf("Inserting %s before %s\n", newDescr, currDescr);

			free(currDescr);
			free(newDescr);

			Node* newNode = initializeNode(toBeAdded);
			newNode->next = currNode;
			newNode->previous = currNode->previous;
			currNode->previous->next = newNode;
			currNode->previous = newNode;
			(list->length)++;

			return;
		}

		currNode = currNode->next;
	}

	return;
}

/**Returns a string that contains a string representation of the list traversed from  head to tail.
Utilize an iterator and the list's printData function pointer to create the string.
returned string must be freed by the calling function.
 *@pre List must exist, but does not have to have elements.
 *@param list Pointer to linked list dummy head.
 *@return on success: char * to string representation of list (must be freed after use).  on failure: NULL
 **/
char* toString(List * list){
	ListIterator iter = createIterator(list);
	char* str;

	str = (char*)malloc(sizeof(char));
	strcpy(str, "");

	void* elem;
	while((elem = nextElement(&iter)) != NULL){
		char* currDescr = list->printData(elem);
		int newLen = strlen(str)+50+strlen(currDescr);
		str = (char*)realloc(str, newLen);
		strcat(str, "\n");
		strcat(str, currDescr);

		free(currDescr);
	}

	return str;
}

char* toStringDelim(List * list, char * delim){
	ListIterator iter = createIterator(list);
	char* str;
	int i = 1;
	str = (char*)malloc(sizeof(char));
	strcpy(str, "");

	void* elem;
	while((elem = nextElement(&iter)) != NULL){
		char* newDelim = malloc(strlen(delim) + 50);
		sprintf(newDelim, "%s %d:\n", delim, i);
		char* currDescr = list->printData(elem);
		int newLen = strlen(str)+50+strlen(currDescr)+strlen(delim);
		str = (char*)realloc(str, newLen);
		strcat(str, newDelim);
		strcat(str, currDescr);
		free(currDescr);
		free(newDelim);
		i++;
	}

	return str;
}



ListIterator createIterator(List* list){
    ListIterator iter;

    iter.current = list->head;

    return iter;
}

void* nextElement(ListIterator* iter){
    Node* tmp = iter->current;

    if (tmp != NULL){
        iter->current = iter->current->next;
        return tmp->data;
    }else{
        return NULL;
    }
}

int getLength(List* list){
	return list->length;
}

void* findElement(List * list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord){
	if (customCompare == NULL)
		return NULL;

	ListIterator itr = createIterator(list);

	void* data = nextElement(&itr);
	while (data != NULL)
	{
		if (customCompare(data, searchRecord))
			return data;

		data = nextElement(&itr);
	}

	return NULL;
}
