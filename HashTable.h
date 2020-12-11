#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "Defs.h"
#include "LinkedList.h"
#include "KeyValuePair.h"


/* generic ADT of HashTable */
typedef struct hashTable_s *hashTable;

/* creates a HashTable. If fails, returns NULL */
hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber);

/* releases the memory allocated for the HashTable */
status destroyHashTable(hashTable);

/* appends key and value to the HashTable */
status addToHashTable(hashTable, Element key,Element value);

/* search key in the HashTable. If the key not exist, returns NULL */
Element lookupInHashTable(hashTable, Element key);

/* remove key from the HashTable. If the key not exist, returns failure */
status removeFromHashTable(hashTable, Element key);

/* print the HashTable elements */
status displayHashElements(hashTable);

#endif /* HASH_TABLE_H */