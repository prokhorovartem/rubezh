#include "task4.h"

#include <stdio.h>
#include <stdlib.h>

static node_t *create_node(val_t);

static void free_node(node_t *);


static inline bool
cas(volatile pointer_t *addr, pointer_t oldp, const pointer_t newp) {
    char result;
    __asm__ __volatile__("lock; cmpxchg16b %0; setz %1":"=m"(*addr),
    "=q"(result)
    :"m"(*addr), "a"(oldp.count), "d"(oldp.ptr),
    "b"(newp.count), "c"(newp.ptr)
    :"memory");
    return (((int) result == 0) ? false : true);
}


static node_t *create_node(const val_t val) {
    node_t *node;

    if ((node = (node_t *) calloc(1, sizeof(node_t))) == NULL) {
        elog("calloc error");
        return NULL;
    }

    node->val = val;
    node->next.ptr = NULL;
    node->next.count = 0;

    return node;
}

static void free_node(node_t *node) {
#ifdef _FREE_
    free(node);
#endif
}


ConcurrentQueue *init_queue(void) {
    ConcurrentQueue *q;
    node_t *node;

    if ((q = (ConcurrentQueue *) calloc(1, sizeof(ConcurrentQueue))) == NULL) {
        elog("calloc error");
        return NULL;
    }

    if ((node = create_node((val_t) NULL)) == NULL) {
        elog("create_node() error");
        abort();
    }

    q->head.ptr = node;
    q->tail.ptr = node;

    return q;
}

void free_queue(ConcurrentQueue *q) {
    free(q);
}


bool enqueue(ConcurrentQueue *q, const val_t val) {
    node_t *newNode;
    pointer_t tail, next, tmp;

    if ((newNode = create_node(val)) == NULL)
        return false;

    while (1) {
        tail = q->tail;
        next = tail.ptr->next;

        if (tail.count == q->tail.count && tail.ptr == q->tail.ptr) {
            if (next.ptr == NULL) {
                tmp.ptr = newNode;
                tmp.count = next.count + 1;
                if (cas(&tail.ptr->next, next, tmp) == true) {
                    break;
                }
            } else {
                tmp.ptr = next.ptr;
                tmp.count = tail.count + 1;
                cas(&q->tail, tail, tmp);
            }
        }
    }
    tmp.ptr = newNode;
    tmp.count = tail.count + 1;
    cas(&q->tail, tail, tmp);

    return true;
}


bool dequeue(ConcurrentQueue *q, val_t *val) {
    pointer_t head, tail, next, tmp;

    while (1) {
        head = q->head;
        tail = q->tail;
        next = head.ptr->next;

        if (head.count == q->head.count && head.ptr == q->head.ptr) {
            if (head.ptr == tail.ptr) {
                if (next.ptr == NULL) {
                    return false;
                }
                tmp.ptr = next.ptr;
                tmp.count = head.count + 1;
                cas(&q->tail, tail, tmp);
            } else {
                *val = next.ptr->val;
                tmp.ptr = next.ptr;
                tmp.count = head.count + 1;
                if (cas(&q->head, head, tmp) == true) {
                    break;
                }
            }
        }
    }

    free_node(head.ptr);
    return true;
}


void show_queue(ConcurrentQueue *q) {
    node_t *curr;

    curr = q->head.ptr;
    while ((curr = curr->next.ptr) != NULL) {
        printf("[%d]", (int) curr->val);
    }
    printf("\n");

}