#ifndef _CAS_LOCKFREE_QUEUE_H_
#define _CAS_LOCKFREE_QUEUE_H_

#include <inttypes.h>
//#include <stdbool.h>

#ifndef __COMMON_H__
#define __COMMON_H__

#include <inttypes.h>

#ifndef C_H
#ifndef bool
typedef char bool;
#endif
#ifndef true
#define true    ((bool) 1)
#endif
#ifndef false
#define false   ((bool) 0)
#endif
typedef bool *BoolPtr;
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef NULL
#define NULL    ((void *) 0)
#endif
#endif

typedef bool bool_t;
typedef intptr_t lkey_t;
typedef intptr_t val_t;


#define elog(_message_)  do {fprintf(stderr,                    \
                     "%s():%s:%u: %s\n",        \
                     __FUNCTION__, __FILE__, __LINE__,    \
                     _message_); fflush(stderr);}while(0);

#define MB()  __asm__ __volatile__ ("lock; addl $0,0(%%esp)" : : : "memory")
#define WMB() __asm__ __volatile__ ("" : : : "memory")
#define RMB() MB()

#endif

typedef struct _pointer_t {
    intptr_t count;
    struct _node_t *ptr;
}__attribute__((packed)) pointer_t;

typedef struct _node_t {
    /* Do not change this order. */
    pointer_t next;
    val_t val;
}__attribute__((packed)) node_t;


typedef struct ConcurrentQueue {
    pointer_t head;
    pointer_t tail;
} ConcurrentQueue;

struct ConcurrentQueue *init_queue(void);

void free_queue(ConcurrentQueue *);

bool enqueue(ConcurrentQueue *q, val_t val);

bool dequeue(ConcurrentQueue *q, val_t *val);

void show_queue(ConcurrentQueue *);

#endif