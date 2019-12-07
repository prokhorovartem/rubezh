#include <pthread.h>

typedef struct ThinNode {
    int key;
    int val;
    struct ThinNode *next;
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

char insert(ThinList *list, int key, int value);

FindResult find(ThinList *list, int key);

char myRemove(ThinList *list, int key);

ThinList *init_list(void);

ThinList *get_snapshot(ThinList *list);