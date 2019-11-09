#include <pthread.h>
#include <stdlib.h>
typedef struct ThinNode {
    int key;
    int value;
    pthread_mutex_t mutex;
    struct ThinNode *next;
} ThinNode;

typedef struct {
    ThinNode *root;
} ThinList;

typedef struct {
    char exists;
    int value;
} FindResult;

FindResult find(ThinList *list, int key);

char insert(ThinList *list, int key, int value);

char remove(ThinList *list, int key);

char remove(ThinList *list, int key) {
    ThinNode *prev;
    ThinNode *curr;
    char ret = 1;

    pthread_mutex_lock(list->root->mutex);

    prev = list->root;
    curr = prev->next;

    pthread_mutex_lock(curr->mutex);

    if (curr->next == NULL) {
        pthread_mutex_unlock(list->root->mutex);
        pthread_mutex_unlock(curr->mutex);
        return ret;
    } else {
        while (curr->next != NULL && curr->key < key) {
            pthread_mutex_unlock(prev->mutex);
            prev = curr;
            curr = curr->next;
            pthread_mutex_lock(curr->mutex);
        }

        if (curr->next != NULL && key == curr->key) {
            prev->next = curr->next;
            pthread_mutex_unlock(curr->mutex);
            free(curr);
            pthread_mutex_unlock(prev->mutex);
        } else{
            pthread_mutex_unlock(prev->mutex);
            pthread_mutex_unlock(curr->mutex);
            ret = 0;
        }
    }

    return ret;
}