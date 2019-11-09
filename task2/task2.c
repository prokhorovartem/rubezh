#include "task2.h"

#define lock(_mtx_) pthread_mutex_lock(&(_mtx_))
#define unlock(_mtx_) pthread_mutex_unlock(&(_mtx_))

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

