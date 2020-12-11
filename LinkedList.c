#include "LinkedList.h"

struct linkedList_s
{
    Node head;
    Node tail;
    CopyFunction copyElement;
    FreeFunction freeElement;
    PrintFunction printElement;
    EqualFunction compareByKey;
    EqualFunction compareByValue;
    int listSize;
};

struct node_s
{
    Element data;
    Node next;
};

linkedList createLinkedList(CopyFunction copyElement, FreeFunction freeElement, PrintFunction printElement, EqualFunction compareByKey, EqualFunction compareByValue)
{
    if (copyElement == NULL || freeElement == NULL || printElement == NULL || compareByValue == NULL || compareByKey == NULL) return NULL;

    linkedList list = (linkedList) malloc(sizeof(struct linkedList_s));
    if (list == NULL) return NULL;

    list->listSize = 0;

    /* Set head and tail nodes */
    list->head = NULL;
    list->tail = NULL;

    /* Set Pointers functions */
    list->copyElement = copyElement;
    list->freeElement = freeElement;
    list->printElement = printElement;
    list->compareByKey = compareByKey;
    list->compareByValue = compareByValue;

    return list;
}


Node createNode(linkedList list, Element element)
{
    if (list == NULL || element == NULL) return NULL;

    Node node = (Node) malloc(sizeof(struct node_s));
    if (node == NULL) return NULL;

    node->data = list->copyElement(element);
    if (node->data == NULL) return NULL;

    node->next = NULL;

    return node;
}


status destroyList(linkedList list)
{
    if (list == NULL) return failure;

    Node current = list->head;

    while(current != NULL)
    {
        list->head = list->head->next;
        list->freeElement(current->data);
        current->data = NULL;
        free(current);
        current = NULL;
        list->listSize--;
        current = list->head;
    }

    list->head = NULL;
    list->tail = NULL;
    free(list);
    list = NULL;
    return success;
}


status appendNode(linkedList list, Element element)
{
    if (list == NULL || element == NULL) return failure;

    Node node = createNode(list, element);
    if (node == NULL) return failure;

    if (list->head == NULL)  /* the list is empty */
    {
        list->head = node;
        list->head->next = list->tail;
        list->listSize++;
        return success;
    }

    if (list->tail == NULL)  /* we have only one node */
    {
        list->tail = node;
        list->head->next = list->tail;
        list->tail->next = NULL;
        list->listSize++;
        return success;
    }

    else
    {
        list->tail->next = node;
        list->tail = list->tail->next;
        node->next = NULL;
        list->listSize++;
        return success;
    }
}


status deleteNode(linkedList list, Element element)
{
    if (list == NULL || element == NULL) return failure;

    Node current = list->head;

    while (current != NULL)
    {
        if (list->compareByValue(current->data, element))    /* current == element */
            break;
        current = current->next;
    }

    if (current == NULL) return failure;    /* the element doesn't exist */

    if (list->head == current)  /* the element is in the head of the list */
    {
        list->head = list->head->next;
        list->freeElement(current->data);
        current->data = NULL;
        free(current);
        current = NULL;
    }

    else
        {
        Node temp = list->head;
        while (temp->next != current)
        {
            temp = temp->next;
        }

        if (temp->next != NULL && temp->next == current)
        {
            temp->next = current->next;
            list->freeElement(current->data);
            current->data = NULL;
            free(current);
            current = NULL;
        }
    }

    list->listSize--;
    return success;
}

status displayList(linkedList list)
{
    if (list == NULL) return failure;

    Node current = list->head;

    while (current != NULL)
    {
        status s = list->printElement(current->data);
        if (s == failure) return failure;

        current = current->next;
    }

    return success;
}


Element getDataByIndex(linkedList list, int index)
{
    if (list == NULL || index > list->listSize || index < 1) return NULL;

    Node current = list->head;

    while (index > 1)
    {
        current = current->next;
        index--;
    }

    return current->data;
}


int getLengthList(linkedList list)
{
    if (list == NULL) return -1;
    return list->listSize;
}


Element searchByKeyInList(linkedList list, Element element)
{
    if (list == NULL || element == NULL) return NULL;

    Node current = list->head;

    while (current != NULL)
    {
        if (list->compareByKey(current->data, element))     /* the element is found */
            return current->data;

        current = current->next;
    }

    return NULL;    /* the element is not found */
}