#include "elib.h"
#include <stdlib.h>
#include <assert.h>


/* Declarations ------------------------------------------------------------- */
static void clear(ArrayList *list);
static void trim(ArrayList *list);
static void reverse(ArrayList *list);
static void destroy(void **p);

/* Search */
static bool has(ArrayList *list, void *p);
static int find(ArrayList *list, void *p);
static int findLast(ArrayList *list, void *p);

/* These return elements from a list */
static void *get(ArrayList *list, const int index);
static ArrayList *getRange(ArrayList *list, const int startIndex,
                                            const int endIndex);

/* These set elements in a list */
static void *set(ArrayList *list, const int index, void *p);

/* These add elements to a list */
static void *append(ArrayList *list, void *p);
static ArrayList *appendRange(ArrayList *list, ArrayList *other);

/* These insert elements into a list */
static void *insert(ArrayList *list, const int index, void *p);
static ArrayList *insertRange(ArrayList *list, const int index, ArrayList* other);

/* These remove elements from a list */
static void *pop(ArrayList *list, const int index);
static ArrayList *popRange(ArrayList *list, const int startIndex,
                                            const int endIndex);

/* Iteration */
static void each(ArrayList *list, void (*cb)(void *p));

/* These check for existence of *p */
static void *array_add(ArrayList *list, void *p);
static void *array_remove(ArrayList *list, void *p);

/* Helper functions */
static bool index_valid(int *index, const int length);
static void double_size(ArrayList* list);
static void quarter_size(ArrayList* list);


/* Constructor -------------------------------------------------------------- */
ArrayList *arrayList(const unsigned int size) {

    ArrayList *list = malloc(sizeof(ArrayList));
    if (list == NULL) {
        return NULL;
    }

    list->length = 0;
    list->size = size > 0 ? size : ARRAY_LIST_MIN_SIZE;

    /* The list CANNOT hold NULL items since many methods will return NULL 
       in case of a failure */
    list->items = calloc(list->size, sizeof(void*));

    if (!list->items) {
        free(list);
        return NULL;
    }

    /* Map methods */
    list->clear = &clear;

    list->has = &has;
    list->find = &find;
    list->findLast = &findLast;

    list->get = &get;
    list->getRange = &getRange;

    list->set = &set;

    list->append = &append;
    list->appendRange = &appendRange;

    list->insert = &insert;
    list->insertRange = &insertRange;

    list->pop = &pop;
    list->popRange = &popRange;

    list->add = &array_add;
    list->remove = &array_remove;

    list->trim = &trim;
    list->reverse = &reverse;

    list->each = &each;
                             
    return list;
 
}


/* Generics ----------------------------------------------------------------- */
static void clear(ArrayList *list) {

    void *tmp;
    while((tmp = pop(list, 0))) {
        free(tmp);
    }

    if (list->size > ARRAY_LIST_MIN_SIZE && list->length < list->size / 8) {
        quarter_size(list);
    }

}

static void trim(ArrayList *list) {

    unsigned int size = list->length > 0 ? list->length : 1;

    void *tmp = realloc(list->items, sizeof(void*) * size);
    if (tmp != NULL) {
        list->items = tmp;
        list->size = list->length;
    }

}

static void reverse(ArrayList *list) {
    
    unsigned int i, l = list->length, e;
    for(i = 0, e = l - 1; i < l / 2; i++, e--) {

        void *tmp = list->items[i];
        list->items[i] = list->items[e];
        list->items[e] = tmp;

    }

}

static void destroy(void **p) {
    ArrayList **list = (ArrayList**)p;
    free((*list)->items);
    free(*list);
    *list = NULL;
}


/* Search ------------------------------------------------------------------- */
static bool has(ArrayList *list, void *p) {
    return list->find(list, p) != -1;
}

static int find(ArrayList *list, void *p) {

    unsigned int i, l;
    for(i = 0, l = list->length; i < l; i++) {
        if (list->items[i] == p) {
            return i;
        }
    }

    return -1;

}

static int findLast(ArrayList *list, void *p) {

    int i;
    for(i = list->length; i >= 0; i--) {
        if (list->items[i] == p) {
            return i;
        }
    }

    return -1;

}


/* Getting ------------------------------------------------------------------ */
static void *get(ArrayList *list, const int offset) {

    int index = offset;
    if (index_valid(&index, list->length)) {
        return list->items[index];
    
    } else {
        return NULL;
    }

}

static ArrayList *getRange(ArrayList *list, const int startIndex,
                                            const int endIndex) {

    int start = startIndex, end = endIndex;
    if (index_valid(&start, list->length) && index_valid(&end, list->length)) {
        
        ArrayList *range = arrayList(end - start);

        while(start < end) {
            range->items[range->length] = list->items[start++];
            range->length++;
        }

        return range;
        
    } else {
        return NULL;
    }

}


/* Setting ------------------------------------------------------------------ */
static void *set(ArrayList *list, const int offset, void *p) {

    int index = offset;
    if (p != NULL && index_valid(&index, list->length)) {
        return list->items[index] = p;
    
    } else {
        return NULL;
    }

}


/* Appending ---------------------------------------------------------------- */
static void *append(ArrayList *list, void *p) {

    if (p == NULL) {
        return NULL;

    } else if (list->length > list->size - 1) {
        double_size(list);
    }
    
    list->items[list->length] = p;
    list->length++;

    return p;

}

static ArrayList *appendRange(ArrayList *list, ArrayList *other) {
    
    unsigned int i = 0;
    while(i < other->length) {

        if (list->length > list->size - 1) {
            double_size(list);
        }
        
        list->items[list->length] = other->items[i++];
        list->length++;

    }

    return other;

}


/* Inserting ---------------------------------------------------------------- */
static void *insert(ArrayList *list, const int offset, void *p) {
    
    int index = offset;
    if (p != NULL && index_valid(&index, list->length + 1)) {

        int i;
        if (list->length > list->size - 1) {
            double_size(list);
        }
        
        for(i = list->length; i > index; i--) {
            list->items[i] = list->items[i - 1];
        }
        
        list->items[index] = p;
        list->length++;

        return p;
    
    } else {
        return NULL;
    }

}

static ArrayList *insertRange(ArrayList *list, const int offset,
                                               ArrayList *other) {

    int index = offset;
    if (other != NULL && index_valid(&index, list->length + 1)) {

        int i;
        for(i = other->length; i >= 0; i--) {
            insert(list, index, other->items[i]);
        }

        return other;

    } else {
        return NULL;
    }

}


/* Popping ------------------------------------------------------------------ */
static void *pop(ArrayList *list, const int offset) {

    int index = offset;
    if (index_valid(&index, list->length)) {
        
        void *item = list->items[index];

        unsigned int i, l;
        for(i = index, l = list->length - 1; i < l; i++) {
            list->items[i] = list->items[i + 1];
        }
        
        list->length--;
        list->items[list->length] = NULL;

        if (list->size > ARRAY_LIST_MIN_SIZE && list->length < list->size / 8) {
            quarter_size(list);
        }

        return item;
    
    } else {
        return NULL;
    }

}

static ArrayList *popRange(ArrayList *list, const int startIndex,
                                            const int endIndex) {
    
    int start = startIndex, end = endIndex;
    if (index_valid(&start, list->length) && index_valid(&end, list->length)) {
        
        int length = end - start;
        ArrayList *range = arrayList(length);

        while(length--) {
            range->items[range->length] = list->pop(list, start);
            range->length++;
        }

        return range;
        
    } else {
        return NULL;
    }

}


/* Elements ----------------------------------------------------------------- */
static void *array_add(ArrayList *list, void *p) {

    if (find(list, p) == -1) {
        return append(list, p);
    
    } else {
        return NULL;
    }

}

static void *array_remove(ArrayList *list, void *p) {

    const int index = find(list, p);
    if (index != -1) {
        return pop(list, index);
    
    } else {
        return NULL;
    }

}


/* Iteration ---------------------------------------------------------------- */
static void each(ArrayList *list, void (*cb)(void *p)) {

    unsigned int i, l;
    for(i = 0, l = list->length; i < l; i++) {
        cb(list->items[i]);
    }

}


/* Helpers ------------------------------------------------------------------ */
static bool index_valid(int *index, const int length) {

    *index = *index < 0 ? length + *index : *index;
    return (*index >= 0 && *index <= length - 1);

}

static void double_size(ArrayList *list) {

    void *tmp = realloc(list->items, sizeof(void*) * list->size * 2);
    if (tmp != NULL) {
        list->size *= 2;

    } else {
        tmp = realloc(list->items, sizeof(void*) * list->size + 1);
        assert(tmp != NULL);

        list->size++;
    }

    list->items = tmp;

}

static void quarter_size(ArrayList *list) {

    void *tmp;
    unsigned int size = list->size / 4;

    if (size < ARRAY_LIST_MIN_SIZE) {
        size = ARRAY_LIST_MIN_SIZE;
    }

    tmp = realloc(list->items, sizeof(void*) * size);
    if (tmp != NULL) {
        list->items = tmp;
        list->size = size;
    }

}

