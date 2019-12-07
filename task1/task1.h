#include <pthread.h>

typedef struct RoughNode {
    int key;
    int value;
    struct RoughNode *next;
} RoughNode;

typedef struct {
    RoughNode *head;
    RoughNode *tail;
    pthread_mutex_t mtx;
} RoughList;

typedef struct {
    char exists;
    int value;
} FindResult;

char insert(RoughList *list, int key, int value);

FindResult find(RoughList *list, int key);

char myRemove(RoughList *list, int key);

RoughList *init_list();

RoughList *get_snapshot(RoughList *list);