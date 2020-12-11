#include "HashTable.h"

struct hashTable_s
{
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;
    int hashNumber;
    linkedList* array;
};

static Element copyKVP(Element element)
{
    if (element == NULL) return NULL;
    keyValuePair pair = (keyValuePair) element;
    return pair;
}

static status freeKVP(Element element)
{
    if (element == NULL) return failure;
    keyValuePair pair = (keyValuePair) element;
    return destroyKeyValuePair(pair);
}

static status printKVP(Element element)
{
    if (element == NULL) return failure;
    keyValuePair pair = (keyValuePair) element;
//    displayKey(pair);
    displayValue(pair);
    return success;
}

static bool compareByKey(Element e1, Element e2)
{
    if (e1 == NULL || e2 == NULL) return false;
    keyValuePair pair1 = (keyValuePair) e1;

    return isEqualKey(pair1, e2);
}

static bool compareByValue(Element e1, Element e2)
{
    if (e1 == NULL || e2 == NULL) return false;
    keyValuePair pair1 = (keyValuePair) e1;
    keyValuePair pair2 = (keyValuePair) e2;
    return isEqualKey(pair1, getKey(pair2));
}

static int hashFunc(hashTable hash, Element element)
{
    int place = hash->transformIntoNumber(element);
    return place = place % hash->hashNumber;
}

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber)
{
    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL || transformIntoNumber == NULL || hashNumber < 0) return NULL;

    hashTable hash = (hashTable) malloc(sizeof(struct hashTable_s));
    if (hash == NULL) return NULL;

    hash->copyKey = copyKey;
    hash->freeKey = freeKey;
    hash->printKey = printKey;
    hash->copyValue = copyValue;
    hash->freeValue = freeValue;
    hash->printValue = printValue;
    hash->equalKey = equalKey;
    hash->transformIntoNumber = transformIntoNumber;
    hash->hashNumber = hashNumber;
    hash->array = (linkedList*) malloc(hashNumber*sizeof(linkedList));
    int i;
    for (i = 0; i < hashNumber; i++)    /* initialize every cell in the hash table array with size of linked list */
    {
        hash->array[i] = createLinkedList(copyKVP, freeKVP, printKVP, compareByKey, compareByValue);
        if (hash->array[i] == NULL) return NULL;
    }

    return hash;
}

status destroyHashTable(hashTable hash)
{
    if (hash == NULL) return failure;

    int i;
    for (i = 0; i < hash->hashNumber; i++)
    {
        status s = destroyList(hash->array[i]);
        if (s == failure) return failure;
    }
    free(hash->array);
    hash->array = NULL;


    free(hash);
    hash = NULL;

    return success;
}


status addToHashTable(hashTable hash, Element key, Element value)
{
    if (hash == NULL || key == NULL || value == NULL) return failure;

    int place = hashFunc(hash, key);     /* the index of the key after calculate his place in the hash table */

    Element keyExists = lookupInHashTable(hash, key);   /* check if the key already exists */

    if (keyExists != NULL) return failure;  /* the key already exists */

    else    /* the key doesn't exists */
    {
        Element toAdd = (Element) createKeyValuePair(key, value, hash->copyKey, hash->freeKey, hash->printKey, hash->copyValue, hash->freeValue, hash->printValue, hash->equalKey);
        if (toAdd == NULL) return failure;

        return appendNode(hash->array[place], toAdd);   /* append the node to tail */
    }
}


Element lookupInHashTable(hashTable hash, Element key)
{
    if (hash == NULL || key == NULL) return NULL;

    int place = hashFunc(hash, key);     /* the index of the key after calculate his place in the hash table */

    keyValuePair toSearch = (keyValuePair) searchByKeyInList(hash->array[place], key);
    if (toSearch == NULL) return NULL;  /* the element not found */

    return getValue(toSearch);
}


status removeFromHashTable(hashTable hash, Element key)
{
    if (hash == NULL || key == NULL) return failure;

    int place = hashFunc(hash, key);     /* the index of the key after calculate his place in the hash table */

    Element toRemove = searchByKeyInList(hash->array[place], key);

    if (toRemove == NULL) return failure;   /* the element not found */

    status s = deleteNode(hash->array[place], toRemove);
    if (s == failure) return failure;

    return success;
}


status displayHashElements(hashTable hash)
{
    if (hash == NULL) return failure;

    int i;
    for(i = 0; i < hash->hashNumber; i++)
    {
        status s = displayList(hash->array[i]);
        if (s == failure) return failure;
    }

    return success;
}