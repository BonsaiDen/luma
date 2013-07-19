#include "elib.h"
#include <stdlib.h>


/* Declarations ------------------------------------------------------------- */
static void clear(LinkedList *list);
static void reverse(LinkedList *list);
static void destroy(LinkedList **list);

/* Array conversion */
static ArrayList *array(LinkedList *list);

/* Search */
static bool has(LinkedList *list, void *p);
static int find(LinkedList *list, void *p);
static int findLast(LinkedList *list, void *p);

/* Insertion */
static LinkedNode *insertFront(LinkedList *list, void *value);
static LinkedNode *insertBack(LinkedList *list, void *value);
static LinkedNode *insertBefore(LinkedList *list, LinkedNode *p, void *value);
static LinkedNode *insertAfter(LinkedList *list, LinkedNode *p, void *value);

/* Removal */
static void *removeFront(LinkedList *list);
static void *removeNode(LinkedList *list, LinkedNode **p);
static void *removeBack(LinkedList *list);

/* Iteration */
static void each(LinkedList *list, void (*cb)(void *value));

/* Helpers */
static LinkedNode *createNode(LinkedList *list, void *value);


/* Constructor -------------------------------------------------------------- */
LinkedList *linkedList() {

    LinkedList *list = malloc(sizeof(LinkedList));

    if (list == NULL) {
        return NULL;
    }

    list->length = 0;

    list->front = NULL;
    list->back = NULL;

    /* Map methods */
    list->clear = &clear;
    list->reverse = &reverse;
    list->destroy = &destroy;

    list->array = &array;

    list->has = &has;
    list->find = &find;
    list->findLast = &findLast;

    list->insertFront = &insertFront;
    list->insertBack = &insertBack;

    list->insertBefore = &insertBefore;
    list->insertAfter = &insertAfter;

    list->removeFront = &removeFront;
    list->removeNode = &removeNode;
    list->removeBack = &removeBack;

    list->each = &each;
    
    return list;

}


/* Generics ----------------------------------------------------------------- */
static void clear(LinkedList *list) {
    
    while(list->front) {
        removeFront(list);
    }

}

static void reverse(LinkedList *list) {

    LinkedNode *current;
    LinkedNode *node;

    if (list->length <= 1) {
        return;
    }

    current = list->front;
    current->next->prev = NULL;
    list->back = current;

    node = current->next;
    current->next = NULL;

    while(node->next) {

        current->prev = node;
        node->next = current;
        
        current = node;
        node = node->next;

    }

    list->front = current;

}

static void destroy(LinkedList **list) {

    clear(*list);
    free(*list);
    *list = NULL;

}


/* Conversion --------------------------------------------------------------- */
static ArrayList *array(LinkedList *list) {
    
    ArrayList *arr = arrayList(list->length);

    LinkedNode *node = list->front;
    while(node) {
        arr->append(arr, node->value);
        node = node->next;
    }

    return arr;

}


/* Search ------------------------------------------------------------------- */
static bool has(LinkedList *list, void *p) {

    LinkedNode *node = list->front;
    while(node && node->value != p) {
        node = node->next;
    }

    return node ? true : false;
    
}

static int find(LinkedList *list, void *p) {

    int index = 0;

    LinkedNode *node = list->front;
    while(node) {

        if (node->value == p) {
            return index;
        }

        node = node->next;
        index++;

    }

    return -1;

}

static int findLast(LinkedList *list, void *p) {

    LinkedNode *node = list->back;
    int index = 0;

    while(node) {

        if (node->value == p) {
            return index;
        }

        node = node->prev;
        index++;

    }

    return -1;

}


/* Inserting ---------------------------------------------------------------- */
static LinkedNode *insertFront(LinkedList *list, void *value) {
    
    if (list->front == NULL) {

        LinkedNode *node = createNode(list, value);
        if (node == NULL) {
            return NULL;

        } else {
            list->front = node;
            list->back = node;

            return node;
        }


    } else {
        return insertBefore(list, list->back, value);
    }

}

static LinkedNode *insertBack(LinkedList *list, void *value) {
    
    if (list->front == NULL) {
        return insertFront(list, value);

    } else {
        return insertAfter(list, list->back, value);
    }

}

static LinkedNode *insertBefore(LinkedList *list, LinkedNode *p, void *value) {
    
    LinkedNode *node = createNode(list, value);
    if (node == NULL) {
        return NULL; 

    } else {

        if (p->prev == NULL) {
            list->front = node;

        } else {
            node->prev = p->prev;
            p->prev->next = node;
        }

        node->next = p;
        p->prev = node;

        return node;
    }

}

static LinkedNode *insertAfter(LinkedList *list, LinkedNode *p, void *value) {

    LinkedNode *node = createNode(list, value);
    if (node == NULL) {
        return NULL; 

    } else {

        if (p->next == NULL) {
            list->back = node;

        } else {
            node->next = p->next;
            p->next->prev = node;
        }

        node->prev = p;
        p->next = node;

        return node;
    }

}

static LinkedNode *createNode(LinkedList *list, void *value) {

    LinkedNode *node = calloc(sizeof(LinkedNode), 1);
    if (node == NULL) {
        return NULL;

    } else {
        node->value = value;
        list->length++;
        
        return node;
    }

}


/* Removing ----------------------------------------------------------------- */
static void *removeFront(LinkedList *list) {
    
    if (list->front) {
        return removeNode(list, &list->front);

    } else {
        return NULL;
    }

}

static void *removeNode(LinkedList *list, LinkedNode **p) {

    LinkedNode *node = *p;
    void *data = node->value;

    if (node == list->front) {
        list->front = node->next;

        if (list->front) {
            list->front->prev = NULL;
        }

    } else if (node == list->back) {
        list->back = node->prev;

        if (list->back) {
            list->back->next = NULL;
        }

    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    list->length--;

    free(node);
    *p = NULL;

    return data;

}

static void *removeBack(LinkedList *list) {
    
    if (list->back) {
        return removeNode(list, &list->back);

    } else {
        return NULL;
    }

}


/* Iteration ---------------------------------------------------------------- */
static void each(LinkedList *list, void (*cb)(void *p)) {

    LinkedIter *iter = linkedIter(list);
    LinkedNode *node;
    
    while((node = iter->next(iter))) {
        cb(node->value);
    }

    iter->destroy(&iter);

}

