#include "task1.h"

#define lock(_mtx_) pthread_mutex_lock(&(_mtx_))
#define unlock(_mtx_) pthread_mutex_unlock(&(_mtx_))

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

char insert(RoughList *list, int key, int value) {
    RoughNode *pred, *curr;
    RoughNode *newNode;
    char res = '1';

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

        if (curr != list->tail && key == curr->key) { free_node(newNode);
            res = '0';
        } else {
            newNode->next = curr;
            pred->next = newNode;
        }
    }

    unlock(list->mtx);

    return res;
}

char remove(RoughList *list, int key) {
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
