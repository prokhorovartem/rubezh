#include "task2.h"
#include <pthread.h>
#include <stdlib.h>

#define lock(_mutex_) pthread_mutex_lock(&(_mutex_))
#define unlock(_mutex_) pthread_mutex_unlock(&(_mutex_))

#ifdef _FREE_
#define free_node(node) do {pthread_mutex_destroy(&node->mutex);\
    free(node);  }while(0);
#else
#define free_node(node) ;
#endif

static ThinNode *create_node(int, int);

static ThinNode *create_node(const int key, const int val) {
    ThinNode *node;

    if ((node = calloc(1, sizeof(ThinNode))) == NULL) {
        return NULL;
    }

    node->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    node->key = key;
    node->val = val;

    return node;
}

FindResult find(ThinList *list, int key) {
    ThinNode *pred, *curr;
    FindResult result;

    lock(list->head->mutex);  /* get lock of list->head */

    pred = list->head;
    curr = pred->next;      /* curr is the next node of list->head */

    lock(curr->mutex);        /* get lock of curr */

    if (curr == list->tail) {
        /* there is no node in the list. */
        unlock(list->head->mutex);
        unlock(curr->mutex);
        result.exists = '0';
        result.value = 0;
    } else {
        while (curr != list->tail && curr->key < key) {
            unlock(pred->mutex);
            pred = curr;
            curr = curr->next;
            lock(curr->mutex);
        }

        /*
         * assert(pred->key < newNode->key);
         * assert(newNode->key <= curr->key);
         */
        if (curr != list->tail && key == curr->key) {
            result.exists = '1';
            result.value = curr->val;
        } else
            result.exists = '0';

        unlock(pred->mutex);
        unlock(curr->mutex);
    }

    return result;
}

char myRemove(ThinList *list, int key) {
    ThinNode *pred, *curr;
    char ret = 1;

    lock(list->head->mutex);  /* get lock of list->head */

    pred = list->head;
    curr = pred->next;      /* curr is the next node of list->head */

    lock(curr->mutex);        /* get lock of curr */

    if (curr == list->tail) {
        /* there is no node in the list. */
        unlock(list->head->mutex);
        unlock(curr->mutex);
        ret = 0;
    } else {
        while (curr != list->tail && curr->key < key) {
            unlock(pred->mutex);
            pred = curr;
            curr = curr->next;
            lock(curr->mutex);
        }

        /* assert((pred->key) < (newNode->key) <= (curr->key)) */
        if (curr != list->tail && key == curr->key) {
//            *val = curr->val;
            pred->next = curr->next;

            unlock(curr->mutex);free_node(curr); /* release lock of curr, and delete curr node. */
            unlock(pred->mutex);      /* after curr node deletes, release lock of pred. */
        } else {
            unlock(pred->mutex);
            unlock(curr->mutex);

            ret = 0;
        }
    }
    return ret;
}

char insert(ThinList *list, int key, int val) {
    ThinNode *pred, *curr;
    ThinNode *newNode;
    char ret = 1;

    if ((newNode = create_node(key, val)) == NULL)
        return 0;

    lock(list->head->mutex);  /* get lock of list->head. */

    pred = list->head;
    curr = pred->next;      /* curr is next node of list->head */

    lock(curr->mutex);        /* get lock of curr */

    if (curr == list->tail) {
        /* there is no node in the list */
        list->head->next = newNode;
        newNode->next = list->tail;

        unlock(list->head->mutex);
        unlock(curr->mutex);
    } else {
        while (curr != list->tail && curr->key < key) {
            /* getting locks of pred and curr */
            unlock(pred->mutex);
            /* rename curr to pred. */
            pred = curr;
            curr = curr->next;
            lock(curr->mutex);
            /* getting locks of pred and curr. */
        }

        /*
         *  assert((pred->key) < (newNode->key) <= (curr->key))
         */

        if (curr != list->tail && key == curr->key) {
            /* already there is a node has same key. */
            free_node(newNode);
            ret = 0;
        } else {
            /* add node */
            newNode->next = curr;
            pred->next = newNode;
        }
        unlock(pred->mutex);
        unlock(curr->mutex);
    }

    return ret;
}

ThinList *init_list(void)
{
    ThinList *list;

    if ((list = (ThinList *) calloc(1, sizeof(ThinList))) == NULL) {
        return NULL;
    }

    if ((list->head = (ThinNode *) calloc(1, sizeof(ThinNode))) == NULL) {
        free (list);
        goto end;
    }
    list->head->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    if ((list->tail = (ThinNode *) calloc(1, sizeof(ThinNode))) == NULL) {
        goto end;
    }
    list->tail->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    list->head->next = list->tail;
    list->tail->next = NULL;

    return list;

    end :
    free (list->head);
    free (list);
    return NULL;
}