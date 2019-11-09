#include "task2.h"

#define lock(_mtx_) pthread_mutex_lock(&(_mtx_))
#define unlock(_mtx_) pthread_mutex_unlock(&(_mtx_))
#include <pthread.h>
#include <stdlib.h>

FindResult find(ThinList *list, int key) {
    ThinNode *pred, *curr;
    FindResult result = {'0'};
    lock(list->root->mutex);

    pred = list->root;
    curr = pred->next;

    lock(curr->mutex);

    if (curr->next == NULL) {
        //if there is no nodes in list
        unlock(list->root->mutex);
        unlock(curr->mutex);
    } else {
        if (pred->key == key) {
            result.exists = '1';
            result.value = pred->value;
        }
        while (curr->next != NULL && curr->key < key) {
            unlock(pred->mutex);
            pred = curr;
            curr = curr->next;
            lock(curr->mutex);
        }
        if (curr->next != NULL && curr->key == key) {
            result.exists = '1';
            result.value = curr->value;
        }

        unlock(pred->mutex);
        unlock(curr->mutex);
    }

    return result;
}

char remove(ThinList *list, int key) {
    ThinNode *prev;
    ThinNode *curr;
    char ret = 1;

    lock(list->root->mutex);

    prev = list->root;
    curr = prev->next;

    lock(curr->mutex);

    if (curr->next == NULL) {
        unlock(list->root->mutex);
        unlock(curr->mutex);
        return ret;
    } else {
        while (curr->next != NULL && curr->key < key) {
            unlock(prev->mutex);
            prev = curr;
            curr = curr->next;
            lock(curr->mutex);
        }

        if (curr->next != NULL && key == curr->key) {
            prev->next = curr->next;
            unlock(curr->mutex);
            free(curr);
            unlock(prev->mutex);
        } else{
            unlock(prev->mutex);
            unlock(curr->mutex);
            ret = 0;
        }
    }

    return ret;
}