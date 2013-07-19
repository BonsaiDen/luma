#include "elib.h"
#include <stdlib.h>
#include <string.h>


/* Declarations ------------------------------------------------------------- */
static void clear(HashMap *map);
static void destroy(HashMap **map);

/* Checks */
static bool hasKey(HashMap *map, const char *key);
static bool hasValue(HashMap *map, void *value);

/* Actions */
static void *set(HashMap *map, const char *str, void *value);
static void *get(HashMap *map, const char *str);
static void *del(HashMap *map, const char *str);

/* Values */
static ArrayList *values(HashMap *map);
static ArrayList *keys(HashMap *map);

/* Iteration */
static void each(HashMap *map, void (*cb)(const char *key, void *value));

/* Helper functions */
/* static unsigned long hash_key(const char *key); */
static unsigned int jenkins_hash(const char *key);
                                                       

/* Constructor -------------------------------------------------------------- */
HashMap *hashMap(const unsigned int size) {

    HashMap *map = calloc(1, sizeof(HashMap));
    if (map == NULL) {
        return NULL;
    }

    map->size = size > 0 ? size : HASH_MAP_DEFAULT_SIZE;
    
    map->buckets = calloc(map->size, sizeof(HashEntry*));
    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }

    map->entries = linkedList();
    if (map->entries == NULL) {
        free(map->buckets);
        free(map);
        return NULL;
    }

    /* Map methods */
    map->clear = &clear;
    map->destroy = &destroy;

    map->hasKey = &hasKey;
    map->hasValue = &hasValue;

    map->set = &set;
    map->get = &get;
    map->del = &del;

    map->values = &values;
    map->keys = &keys;

    map->each = &each;

    return map;

}


/* Generics ----------------------------------------------------------------- */
static void clear(HashMap *map) {
    
    LinkedList *entries = map->entries;
    LinkedIter *iter = linkedIter(entries);
    LinkedNode *node = NULL;

    while((node = iter->next(iter))) {

        HashEntry *entry = (HashEntry*)node->value;
        free(entry->key);
        free(entry);

        iter->prev(iter);
        entries->removeNode(entries, &node);

    }

    iter->destroy(&iter);

}

static void destroy(HashMap **map) {

    clear(*map);
    (*map)->entries->destroy(&((*map)->entries));
    free((*map)->buckets);
    free(*map);
    *map = NULL;

}

static void resize(HashMap *map) {

    unsigned int new_size = map->size * 2,
                 l = map->size, i = 0;

    HashEntry **new_buckets = calloc(new_size, sizeof(HashEntry*));
    if (new_buckets == NULL) {
        return;
    }

    map->filled = 0;
    for(;i < l; i++) {
        
        unsigned int index;
        HashEntry *move_entry = NULL,
                  *entry = map->buckets[i];

        while(entry && (move_entry = entry)) {

            entry = entry->next;

            index = jenkins_hash(move_entry->key) % new_size;
            move_entry->next = NULL;

            if (new_buckets[index] == NULL) {
                new_buckets[index] = move_entry;
                map->filled++;
                
            } else {

                HashEntry *prev = new_buckets[index];
                while (prev->next) {
                    prev = prev->next;
                }

                prev->next = move_entry;

            }

        }

    }

    free(map->buckets);

    map->size = new_size;
    map->buckets = new_buckets;

}


/* Checks ------------------------------------------------------------------- */
static bool hasKey(HashMap *map, const char *key) {

    HashEntry *entry = map->buckets[jenkins_hash(key) % map->size];
    while((entry && strcmp(entry->key, key))) {
        entry = entry->next;
    }

    return entry ? true : false;
    
}

static bool hasValue(HashMap *map, void *value) {

    LinkedIter *iter = linkedIter(map->entries);
    LinkedNode *node = NULL;
    bool found = false;

    while((node = iter->next(iter))) {

        HashEntry *entry = (HashEntry*)node->value;
        if (entry->value == value) {
            found = true;
            break;
        }

    }

    iter->destroy(&iter);

    return found;
    
}


/* Actions ------------------------------------------------------------------ */
static void *get(HashMap *map, const char *key) {

    HashEntry *entry = map->buckets[jenkins_hash(key) % map->size];
    while((entry && strcmp(entry->key, key))) {
        entry = entry->next;
    }

    return entry ? entry->value : NULL;

}

static void *set(HashMap *map, const char *key, void *value) {

    unsigned int index = jenkins_hash(key) % map->size;

    HashEntry *prev = NULL,
              *entry = map->buckets[index];

    while((entry && strcmp(entry->key, key))) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == NULL) {

        entry = malloc(sizeof(HashEntry));
        entry->key = calloc(strlen(key) + 1, sizeof(char));
        strcpy(entry->key, key);

        entry->lnode = map->entries->insertBack(map->entries, entry);
        entry->value = value;
        entry->next = NULL;

        if (map->buckets[index] == NULL) {
            map->buckets[index] = entry;
            map->filled++;

            if (map->filled > map->size * 0.75) {
                resize(map);
            }
            
        } else {
            prev->next = entry;
        }

        map->length++;

    } else {
        entry->value = value;
    }

    return value;

}

static void *del(HashMap *map, const char *key) {
    
    unsigned int index = jenkins_hash(key) % map->size;

    HashEntry *prev = NULL, 
              *entry = map->buckets[index];

    while((entry && strcmp(entry->key, key))) {
        prev = entry;
        entry = entry->next;
    }

    if (entry != NULL) {

        void *value = entry->value;
        if (prev != NULL) {
            prev->next = entry->next;

        } else {

            if (entry->next == NULL) {
                map->buckets[index] = NULL; 
                map->filled--;

            } else {
                map->buckets[index] = entry->next;
            }
            
        }

        map->length--;
        map->entries->removeNode(map->entries, &(entry->lnode));

        free(entry->key);
        free(entry);

        return value;

    } else {
        return NULL;
    }
    
}


/* Getters ------------------------------------------------------------------ */
static ArrayList *values(HashMap *map) {
    
    LinkedList *entries = map->entries;
    LinkedIter *iter = linkedIter(entries);
    LinkedNode *node = NULL;

    ArrayList *list = arrayList(map->entries->length);

    while((node = iter->next(iter))) {
        list->append(list, ((HashEntry*)node->value)->value);
    }

    iter->destroy(&iter);

    return list;

}

static ArrayList *keys(HashMap *map) {
    
    LinkedList *entries = map->entries;
    LinkedIter *iter = linkedIter(entries);
    LinkedNode *node = NULL;

    ArrayList *list = arrayList(map->entries->length);

    while((node = iter->next(iter))) {
        list->append(list, ((HashEntry*)node->value)->key);

    }

    iter->destroy(&iter);

    return list;

}


/* Iteration ---------------------------------------------------------------- */
static void each(HashMap *map, void (*cb)(const char *key, void *value)) {

    LinkedList *entries = map->entries;
    LinkedIter *iter = linkedIter(entries);
    LinkedNode *node = NULL;

    while((node = iter->next(iter))) {

        HashEntry *entry = (HashEntry*)node->value;
        cb(entry->key, entry->value);

    }

    iter->destroy(&iter);

}


/* Hashing ------------------------------------------------------------------ */
/*
static unsigned long hash_key(const char *key) {

    unsigned char *str = (unsigned char*)key;
    unsigned long hash = 5381;

    while (*str) {
        hash = ((hash << 5) + hash) + *str++; 
    }

    return hash;
    
}
*/

/* See: http://home.comcast.net/~bretm/hash/7.html */
static unsigned int jenkins_hash(const char *key) {

    unsigned char *data = (unsigned char*)key;
    unsigned int len = strlen(key);
    unsigned int a = 0x9e3779b9;
    unsigned int b = a;
    unsigned int c = 0;

    unsigned int i = 0;
    while (i + 12 <= len) {

        a += (unsigned int)data[i] |
            ((unsigned int)data[i + 1] << 8) |
            ((unsigned int)data[i + 2] << 16) |
            ((unsigned int)data[i + 3] << 24);

        i += 4;

        b += (unsigned int)data[i] |
            ((unsigned int)data[i + 1] << 8) |
            ((unsigned int)data[i + 2] << 16) |
            ((unsigned int)data[i + 2] << 24);

        i += 4;

        c += (unsigned int)data[i] |
            ((unsigned int)data[i + 1] << 8) |
            ((unsigned int)data[i + 2] << 16) |
            ((unsigned int)data[i + 3] << 24);

        i += 4;

        a -= b; a -= c; a ^= (c >> 13);
        b -= c; b -= a; b ^= (a << 8);
        c -= a; c -= b; c ^= (b >> 13);
        a -= b; a -= c; a ^= (c >> 12);
        b -= c; b -= a; b ^= (a << 16);
        c -= a; c -= b; c ^= (b >> 5);
        a -= b; a -= c; a ^= (c >> 3);
        b -= c; b -= a; b ^= (a << 10);
        c -= a; c -= b; c ^= (b >> 15);

    }

    c += (unsigned int) len;

    if (i < len) a += data[i++];
    if (i < len) a += (unsigned int)data[i++] << 8;
    if (i < len) a += (unsigned int)data[i++] << 16;
    if (i < len) a += (unsigned int)data[i++] << 24;
    if (i < len) b += (unsigned int)data[i++];
    if (i < len) b += (unsigned int)data[i++] << 8;
    if (i < len) b += (unsigned int)data[i++] << 16;
    if (i < len) b += (unsigned int)data[i++] << 24;
    if (i < len) c += (unsigned int)data[i++] << 8;
    if (i < len) c += (unsigned int)data[i++] << 16;
    if (i < len) c += (unsigned int)data[i++] << 24;

    a -= b; a -= c; a ^= (c >> 13);
    b -= c; b -= a; b ^= (a << 8);
    c -= a; c -= b; c ^= (b >> 13);
    a -= b; a -= c; a ^= (c >> 12);
    b -= c; b -= a; b ^= (a << 16);
    c -= a; c -= b; c ^= (b >> 5);
    a -= b; a -= c; a ^= (c >> 3);
    b -= c; b -= a; b ^= (a << 10);
    c -= a; c -= b; c ^= (b >> 15);
    
    return c;

}

