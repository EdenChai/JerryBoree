#include "MultiValueHashTable.h"

static status freeValueList(Element element)
{
    if (element == NULL) return failure;
    linkedList list = (linkedList) element;
    return destroyList(list);
}

struct multiValueHashTable_s
{
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    EqualFunction equalValue;
    hashTable _hashTable;

};


multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, EqualFunction equalValue, TransformIntoNumberFunction transformIntoNumber, int hashNumber)
{
    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL || transformIntoNumber == NULL || hashNumber < 0) return NULL;

    multiValueHashTable multiHash = (multiValueHashTable) malloc(sizeof(struct multiValueHashTable_s));
    if (multiHash == NULL) return NULL;

    multiHash->copyValue = copyValue;
    multiHash->freeValue = freeValue;
    multiHash->printValue = printValue;
    multiHash->equalKey = equalKey;
    multiHash->equalValue = equalValue;

    multiHash->_hashTable = createHashTable(copyKey, freeKey, printKey, copyValue, freeValueList, printValue, equalKey, transformIntoNumber, hashNumber);
    if (multiHash->_hashTable == NULL) return NULL;

    return multiHash;
}


status destroyMultiValueHashTable(multiValueHashTable multiHash)
{
    if (multiHash == NULL) return failure;

    /* destroy the inner value's lists */


    destroyHashTable(multiHash->_hashTable);
    free(multiHash);
    multiHash = NULL;

    return success;
}


status addToMultiValueHashTable(multiValueHashTable multiHash, Element key, Element value)
{
    if (multiHash == NULL || key == NULL || value == NULL) return failure;

    linkedList list = (linkedList) lookupInMultiValueHashTable(multiHash, key);

    if (list == NULL)   /* the key doesn't exist, we create a new list of values */
    {
        linkedList newList = createLinkedList(multiHash->copyValue, multiHash->freeValue, multiHash->printValue, multiHash->equalKey, multiHash->equalValue);
        if (newList == NULL) return failure;

        status s = appendNode(newList, value);
        if (s == failure) return failure;

        s = addToHashTable(multiHash->_hashTable, key, newList);
        if (s == failure) return failure;

        return success;
    }

    else    /* the key exists, we append the value to the end of the list */
    {
        Element toAdd = (Element) searchByKeyInList(list, value);   /* check if the value already exists in the list's value */

        if (toAdd != NULL) return failure;  /* the value already exists */

        else
        {
        status s = appendNode(list, value);
        if (s == failure) return failure;

        return success;
        }
    }
}


linkedList lookupInMultiValueHashTable(multiValueHashTable multiHash, Element key)
{
    if (multiHash == NULL || key == NULL) return NULL;

    linkedList list = (linkedList) lookupInHashTable(multiHash->_hashTable, key);
    if (list == NULL) return NULL;

    return list;
}


status removeFromMultiValueHashTable(multiValueHashTable multiHash, Element key, Element value)
{
    if (multiHash == NULL || key == NULL) return failure;

    linkedList list = lookupInMultiValueHashTable(multiHash, key);

    if (list == NULL) return failure;  /* the key doesn't exist */

    else
    {
        Element toRemove = (Element) searchByKeyInList(list, value);   /* check if the value already exists in the list's value */

        if (toRemove == NULL) return failure;  /* the value doesn't exist */

        else
        {
            status s = deleteNode(list, value);     /* delete the value from list's value */
            if (s == failure) return failure;

            if (getLengthList(list) == 0)
            {
                s = removeFromHashTable(multiHash->_hashTable, key);
                if (s == failure) return failure;
            }
            return success;
        }
    }
}


status displayMultiValueHashElementsByKey(multiValueHashTable multiHash, Element key) {
    if (multiHash == NULL || key == NULL) return failure;

    linkedList list = lookupInMultiValueHashTable(multiHash, key);

    if (list == NULL) return failure;  /* the key doesn't exist */

    else
    {
        status s = displayList(list);
        if (s == failure) return failure;
    }
    return success;
}