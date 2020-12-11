#include "KeyValuePair.h"

struct keyValuePair_s
{
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    Element _key;
    Element _value;
};

keyValuePair createKeyValuePair(Element key,Element value, CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey)
{
    if (key == NULL || value == NULL || copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL) return NULL;

    keyValuePair pair = (keyValuePair) malloc(sizeof(struct keyValuePair_s));
    if(pair == NULL) return NULL;

    pair->copyKey = copyKey;
    pair->freeKey = freeKey;
    pair->printKey = printKey;
    pair->copyValue = copyValue;
    pair->freeValue = freeValue;
    pair->printValue = printValue;
    pair->equalKey = equalKey;
    pair->_key = pair->copyKey(key);
    pair->_value = pair->copyValue(value);

    return pair;
}


status destroyKeyValuePair(keyValuePair pair)
{
    if(pair == NULL) return failure;

    pair->freeKey(pair->_key);
    pair->_key = NULL;

    pair->freeValue(pair->_value);
    pair->_value = NULL;

    free(pair);
    pair = NULL;

    return success;
}


status displayValue(keyValuePair pair)
{
    if(pair == NULL) return failure;
    return pair->printValue(pair->_value);
}


status displayKey(keyValuePair pair)
{
    if(pair == NULL) return failure;
    return pair->printKey(pair->_key);
}


Element getValue(keyValuePair pair)
{
    return pair->_value;
}


Element getKey(keyValuePair pair)
{
     return pair->_key;
}


bool isEqualKey(keyValuePair pair, Element key)
{
    if (pair == NULL || key == NULL) return false;
    return pair->equalKey(pair->_key, key);
}

