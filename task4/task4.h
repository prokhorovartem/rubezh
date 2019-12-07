#include <inttypes.h>
#include <stdbool.h>

typedef struct _pointer_t {
    intptr_t count;
    struct _node_t *ptr;
} pointer_t;

typedef struct _node_t {
    pointer_t next;
    int val;
} node_t;

typedef struct ConcurrentQueue {
    pointer_t head;
    pointer_t tail;
} ConcurrentQueue;

struct ConcurrentQueue *init_queue(void);

void free_queue(ConcurrentQueue *);

bool enqueue(ConcurrentQueue *q, int val);

bool dequeue(ConcurrentQueue *q, int *val);

void show_queue(ConcurrentQueue *);