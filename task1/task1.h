#include <iostream>
#include <pthread.h>

#ifndef UNTITLED_TASK1_H
#define UNTITLED_TASK1_H

#endif //UNTITLED_TASK1_H

#define lock(_mtx_) pthread_mutex_lock(&(_mtx_))
#define unlock(_mtx_) pthread_mutex_unlock(&(_mtx_))

#ifdef _FREE_
#define free_node(_node_) free(_node_)
#else
#define free_node(_node_) ;
#endif

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

char myRemove(RoughList *list, int key);
char insert(RoughList *list, int key, int value);
FindResult find(RoughList *list, int key);
RoughNode *create_node(int key, int value);
RoughList *init();
