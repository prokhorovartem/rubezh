#include <pthread.h>

typedef struct RoughNode {
    int key;
    int value;
    struct RoughNode *next;
} RoughNode;

typedef struct {
    RoughNode *root;
} RoughList;

typedef struct {
    char exists;
    int value;
} FindResult;

FindResult find(RoughList *list, int key);

char insert(RoughList *list, int key, int value);

char remove(RoughList *list, int key);