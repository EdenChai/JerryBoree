#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H
#include "Defs.h"
#include "LinkedList.h"

/* generic ADT of keyValuePair */
typedef struct keyValuePair_s *keyValuePair;

/* creates a KeyValuePair. If fails, returns NULL */
keyValuePair createKeyValuePair(Element key, Element value, CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey);

/* releases the memory allocated for the KeyValuePair */
status destroyKeyValuePair(keyValuePair pair);

/* print the value of the pair */
status displayValue(keyValuePair pair);

/* print the key of the pair */
status displayKey(keyValuePair pair);

/* returns the value of the pair. If doesn't exists, returns NULL */
Element getValue(keyValuePair pair);

/* returns the key of the pair. If doesn't exists, returns NULL */
Element getKey(keyValuePair pair);

/* check if the keys are equals. If not equals, returns false */
bool isEqualKey(keyValuePair pair, Element key);



#endif
