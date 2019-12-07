#include "task1.h"
#include <stdlib.h>
#include <pthread.h>

#define lock(_mtx_) pthread_mutex_lock(&(_mtx_))
#define unlock(_mtx_) pthread_mutex_unlock(&(_mtx_))
#define free_node(_node_) free(_node_);

static RoughNode *create_node(int, int);

char insert(RoughList *list, int key, int value) {
    RoughNode *pred, *curr;
    RoughNode *newNode;
    char result = '1';

    if ((newNode = create_node(key, value)) == NULL)
        return '0';

    lock(list->mtx);

    pred = list->head;
    curr = pred->next;

    if (curr == list->tail) {
        list->head->next = newNode;
        newNode->next = list->tail;
    } else {
        while (curr != list->tail && curr->key < key) {
            pred = curr;
            curr = curr->next;
        }

        if (curr != list->tail && key == curr->key) {
            free_node(newNode)
            result = '0';
        } else {
            newNode->next = curr;
            pred->next = newNode;
        }
    }

    unlock(list->mtx);

    return result;
}

FindResult find(RoughList *list, int key) {
    RoughNode *pred, *curr;
    FindResult res;

    lock(list->mtx);

    pred = list->head;
    curr = pred->next;

    if (curr == list->tail) {
        res.exists = '0';
        res.value = 0;
    } else {
        while (curr != list->tail && curr->key < key) {
            curr = curr->next;
        }
        if (curr != list->tail && key == curr->key) {
            res.exists = '1';
            res.value = curr->value;
        } else
            res.exists = '0';
    }

    unlock(list->mtx);
    return res;
}

static RoughNode *create_node(const int key, const int val) {
    RoughNode *node;

    if ((node = calloc(1, sizeof(RoughNode))) == NULL) {
        return NULL;
    }
    node->key = key;
    node->value = val;

    return node;
}

char myRemove(RoughList *list, int key) {
    RoughNode *pred, *curr;
    char res = '1';

    lock(list->mtx);

    pred = list->head;
    curr = pred->next;

    if (curr == list->tail) {
        res = '0';
    } else {
        while (curr->key < key && curr != list->tail) {
            pred = curr;
            curr = curr->next;
        }

        if (key == curr->key) {
            pred->next = curr->next;free_node(curr);
        } else
            res = '0';
    }

    unlock(list->mtx);
    return res;
}

RoughList *init_list(void) {
    RoughList *list;

    if ((list = (RoughList *) calloc(1, sizeof(RoughList))) == NULL) {
        return NULL;
    }

    if ((list->head = (RoughNode *) calloc(1, sizeof(RoughNode))) == NULL) {
        goto end;
    }

    if ((list->tail = (RoughNode *) calloc(1, sizeof(RoughNode))) == NULL) {
        goto end;
    }

    list->head->next = list->tail;
    list->tail->next = NULL;
    list->mtx = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return list;

    end:
    free(list->head);
    free(list);
    return NULL;
}

RoughList get_snapshot(RoughList *list) {
    lock(list->mtx);

    RoughList roughList = {.head = list->head,
            .tail = list->tail};

    unlock(list->mtx);

    return roughList;
}