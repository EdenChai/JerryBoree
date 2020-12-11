#ifndef JERRYBOREE_MULTIVALUEHASHTABLE_H
#define JERRYBOREE_MULTIVALUEHASHTABLE_H
#include "HashTable.h"

/* generic ADT of MultiValueHashTable */
typedef struct multiValueHashTable_s *multiValueHashTable;

/* creates a MultiValueHashTable. If fails, return NULL */
multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber, int hashNumber);

/* releases the memory allocated for the MultiValueHashTable */
status destroyMultiValueHashTable(multiValueHashTable multiHash);

/* appends key and value to the MultiValueHashTable.
 * if the key doesn't exist, create new list of values and append the pair to multi hash
 * if the key already exists but the value doesn't, append the value to list of values
 * else the value already exists in the list of values that relate to key return failure */
status addToMultiValueHashTable(multiValueHashTable multiHash, Element key, Element value);

/* search key in the MultiValueHashTable. If the key doesn't exist, return NULL, else return the value's list */
linkedList lookupInMultiValueHashTable(multiValueHashTable multiHash, Element key);

/* remove value from the MultiValueHashTable.
 * if the key doesn't exist, return failure
 * if the key already exists but the value doesn't, return failure
 * else, remove the value from the list of values that relate to key */
status removeFromMultiValueHashTable(multiValueHashTable multiHash, Element key, Element value);

/* print the MultiValueHashTable elements by specific key */
status displayMultiValueHashElementsByKey(multiValueHashTable multiHash, Element key);

#endif //JERRYBOREE_MULTIVALUEHASHTABLE_H
