#include "elib.h"
#include <stdlib.h>


/* Declarations ------------------------------------------------------------- */
static void destroy(LinkedIter **iter);

static void begin(LinkedIter *iter);
static void *next(LinkedIter *iter);
static void *prev(LinkedIter *iter);
static void end(LinkedIter *iter);


/* Constructor -------------------------------------------------------------- */
LinkedIter *linkedIter(LinkedList *list) {

    LinkedIter *iter = malloc(sizeof(LinkedIter));

    if (iter == NULL) {
        return NULL;
    }

    iter->list = list;
    iter->node = NULL;

    /* Map methods */
    iter->destroy = &destroy;

    iter->begin = &begin;
    iter->begin = &begin;
    iter->next = &next;
    iter->prev = &prev;
    iter->end = &end;

    return iter;
    
}


/* Generics ----------------------------------------------------------------- */
static void destroy(LinkedIter **iter) {

    free(*iter);
    *iter = NULL;

}


/* Iterating ---------------------------------------------------------------- */
static void begin(LinkedIter *iter) {
    iter->node = NULL;
}

static void end(LinkedIter *iter) {
    iter->node = NULL;
}

static void *next(LinkedIter *iter) {

    if (iter->node == NULL) {
        iter->node = iter->list->front;

    } else {
        iter->node = iter->node->next;
    }

    return iter->node;

}

static void *prev(LinkedIter *iter) {

    if (iter->node == NULL) {
        iter->node = iter->list->back;

    } else {
        iter->node = iter->node->prev;
    }

    return iter->node;

}

