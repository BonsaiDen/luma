#ifndef ELIB_H
#define ELIB_H


/* Boolean ----------------------------------------------------------------- */
#define true 1
#define false 0
#define bool int


/* ArrayList --------------------------------------------------------------- */
struct ArrayList;
typedef struct ArrayList {

    unsigned int length;
    unsigned int size;

    void **items;

    void (*clear)(struct ArrayList *list);
    void (*trim)(struct ArrayList *list);
    void (*reverse)(struct ArrayList *list);

    bool (*has)(struct ArrayList *list, void *p);
    int (*find)(struct ArrayList *list, void *p);
    int (*findLast)(struct ArrayList *list, void *p);

    void *(*get)(struct ArrayList *list, const int index);
    struct ArrayList *(*getRange)(struct ArrayList *list, const int startIndex, 
                                                          const int endIndex);

    void *(*set)(struct ArrayList *list, const int index, void *p);

    void *(*append)(struct ArrayList *list, void *p);
    struct ArrayList *(*appendRange)(struct ArrayList *list, 
                                     struct ArrayList *other);

    void *(*insert)(struct ArrayList *list, const int index, void *p);
    struct ArrayList *(*insertRange)(struct ArrayList *list, const int index, 
                                     struct ArrayList *other);

    void *(*pop)(struct ArrayList *list, const int index);
    struct ArrayList *(*popRange)(struct ArrayList *list, const int startIndex, 
                                                          const int endIndex);

    void *(*add)(struct ArrayList *list, void *p);
    void *(*remove)(struct ArrayList *list, void *p);

    void (*each)(struct ArrayList *list, void (*cb)(void *value));

} ArrayList;


/* LinkedList -------------------------------------------------------------- */
struct LinkedNode;
typedef struct LinkedNode {
    
    struct LinkedNode *prev;
    struct LinkedNode *next;
    void *value;
    
} LinkedNode;


struct LinkedList;
typedef struct LinkedIter {

    unsigned int _ref;
    void (*destroy)(struct LinkedIter **iter);

    struct LinkedList *list;
    LinkedNode *node;

    void (*begin)(struct LinkedIter *iter);
    void *(*next)(struct LinkedIter *iter);
    void *(*prev)(struct LinkedIter *iter);
    void (*end)(struct LinkedIter *iter);

} LinkedIter;


typedef struct LinkedList {
    
    unsigned int _ref;
    void (*destroy)(struct LinkedList **list);

    unsigned int length;

    LinkedNode *front;
    LinkedNode *back;

    void (*clear)(struct LinkedList *list);
    void (*reverse)(struct LinkedList *list);

    ArrayList *(*array)(struct LinkedList *list);

    bool (*has)(struct LinkedList *list, void *p);
    int (*find)(struct LinkedList *list, void *p);
    int (*findLast)(struct LinkedList *list, void *p);

    LinkedNode *(*insertFront)(struct LinkedList *list, void *value);
    LinkedNode *(*insertBack)(struct LinkedList *list, void *value);
    LinkedNode *(*insertBefore)(struct LinkedList *list, 
                                struct LinkedNode *p, void *value);

    LinkedNode *(*insertAfter)(struct LinkedList *list, 
                               struct LinkedNode *p, void *value);

    void *(*removeFront)(struct LinkedList *list);
    void *(*removeNode)(struct LinkedList *list, LinkedNode **p);
    void *(*removeBack)(struct LinkedList *list);

    void (*each)(struct LinkedList *list, void (*cb)(void *value));

} LinkedList;


/* HashMap ----------------------------------------------------------------- */
struct HashEntry;
typedef struct HashEntry {

    struct HashEntry *next;
    char *key;
    void *value;
    LinkedNode *lnode;

} HashEntry;

typedef struct HashMap {
    
    unsigned int _ref;
    void (*destroy)(struct HashMap **map);

    unsigned int size;
    unsigned int filled;
    unsigned int length;

    HashEntry **buckets;
    LinkedList *entries;

    void (*clear)(struct HashMap *map);

    bool (*hasKey)(struct HashMap *map, const char *key);
    bool (*hasValue)(struct HashMap *map, void *p);

    void *(*set)(struct HashMap *map, const char *key, void *value);
    void *(*get)(struct HashMap *map, const char *key);
    void *(*del)(struct HashMap *map, const char *key);

    void (*each)(struct HashMap *map, void (*cb)(const char *key, void *value));

    ArrayList *(*values)(struct HashMap *map);
    ArrayList *(*keys)(struct HashMap *map);

} HashMap;


/* Constructors ------------------------------------------------------------ */
ArrayList *arrayList(const unsigned int size);
LinkedList *linkedList();
LinkedIter *linkedIter();
HashMap *hashMap(const unsigned int size);


/* Constants --------------------------------------------------------------- */
#define ARRAY_LIST_MIN_SIZE 16
#define HASH_MAP_DEFAULT_SIZE 256


#endif

