#include <pthread.h>

typedef struct ThinNode {
    int key;
    int val;
    struct _node_t *next;
    pthread_mutex_t mutex;
} ThinNode;

typedef struct ThinList {
    ThinNode *head;
    ThinNode *tail;
} ThinList;

typedef struct {
    char exists;
    int value;
} FindResult;

FindResult find(ThinList *list, int key);

char insert(ThinList *list, int key, int value);

char myRemove(ThinList *list, int key);

ThinList *init_list(void);