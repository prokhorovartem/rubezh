#include <iostream>
#include <pthread.h>
#include "task1.h"

RoughList *init()
{
    RoughList *list;

    if ((list = (RoughList *) calloc(1, sizeof(RoughList))) == NULL) {
        perror("calloc error");
        return NULL;
    }

    if ((list->head = (RoughNode *) calloc(1, sizeof(RoughNode))) == NULL) {
        perror("calloc error");
        goto end;
    }

    if ((list->tail = (RoughNode *) calloc(1, sizeof(RoughNode))) == NULL) {
        perror("calloc error");
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

RoughNode *create_node(int key, int value) {
    RoughNode *node;

    if ((node = (RoughNode *) (calloc(1, sizeof(RoughNode)))) == NULL) {
        perror("calloc error");
        return NULL;
    }
    node->key = key;
    node->value = value;

    return node;
}

FindResult find(RoughList *list, int key) {
    RoughNode *pred, *curr;
    FindResult res;

    lock(list->mtx);

    pred = list->head;
    curr = pred->next;

    if (curr == list->tail) {
        res = { .exists = false, .value = 0 };
    } else {
        while (curr != list->tail && curr->key < key) {
            curr = curr->next;
        }
        if (curr != list->tail && key == curr->key) {
            res = { .exists = true, .value = curr->value};
        }
        else
            res = { .exists = false};
    }

    unlock(list->mtx);
    return res;
}

char insert(RoughList *list, int key, int value) {
    RoughNode *pred, *curr;
    RoughNode *newNode;
    char res = true;

    if ((newNode = create_node(key, value)) == NULL)
        return false;

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
            free_node(newNode);
            res = false;
        } else {
            newNode->next = curr;
            pred->next = newNode;
        }
    }

    unlock(list->mtx);

    return res;
}

char myRemove(RoughList *list, int key) {
    RoughNode *pred, *curr;
    char res = true;

    lock(list->mtx);

    pred = list->head;
    curr = pred->next;

    if (curr == list->tail) {
        res = false;
    } else {
        while (curr->key < key && curr != list->tail) {
            pred = curr;
            curr = curr->next;
        }

        if (key == curr->key) {
            pred->next = curr->next;
            free_node(curr);
        } else
            res = false;
    }

    unlock(list->mtx);
    return res;
}
