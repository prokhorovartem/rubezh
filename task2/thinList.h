#include <pthread.h>

typedef struct ThinNode {
    int key;
    int value;
    pthread_mutex_t mutex;
    struct ThinNode *next;
} ThinNode;

typedef struct {
    ThinNode *root;
} ThinList;

typedef struct {
    char exists;
    int value;
} FindResult;

FindResult find(ThinList *list, int key);

char insert(ThinList *list, int key, int value);

char remove(ThinList *list, int key);