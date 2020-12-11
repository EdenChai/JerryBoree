#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"
#include "KeyValuePair.h"

/* generic ADT of LinkedList */
typedef struct linkedList_s *linkedList;

/* generic ADT of nodes */
typedef struct node_s *Node;

/* creates a LinkedList. If fails, return NULL */
linkedList createLinkedList(CopyFunction copyElement, FreeFunction freeElement, PrintFunction printElement, EqualFunction compareByKey, EqualFunction compareByValue);

/* releases the memory allocated for the LinkedList */
status destroyList(linkedList list);

/* appends Node to the end of the list. If the element already exists, return failure */
status appendNode(linkedList list, Element element);

/* delete node from the list. If the element doesn't exist, return failure */
status deleteNode(linkedList list, Element element);

/* print the list */
status displayList(linkedList list);

/* returns the data in the list at place index. If the index illegal, return NULL  */
Element getDataByIndex(linkedList list, int index);

/* returns the number of elements in the list */
int getLengthList(linkedList list);

/* search element in the list. If the element doesn't exist, return NULL */
Element searchByKeyInList(linkedList list, Element element);

#endif
