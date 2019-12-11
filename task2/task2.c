#include "task2.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#define lock(_mutex_) pthread_mutex_lock(&(_mutex_))
#define unlock(_mutex_) pthread_mutex_unlock(&(_mutex_))
#define free_node(node);

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

char insert(ThinList *list, int key, int val) {
    ThinNode *pred, *curr;
    ThinNode *newNode;
    char ret = 1;

    if ((newNode = create_node(key, val)) == NULL)
        return 0;

    lock(list->head->mutex);

    pred = list->head;
    curr = pred->next;

    lock(curr->mutex);

    if (curr == list->tail) {
        list->head->next = newNode;
        newNode->next = list->tail;

        unlock(list->head->mutex);
        unlock(curr->mutex);
    } else {
        while (curr != list->tail && curr->key < key) {
            unlock(pred->mutex);
            pred = curr;
            curr = curr->next;
            lock(curr->mutex);
        }

        if (curr != list->tail && key == curr->key) { free_node(newNode);
            ret = 0;
        } else {
            newNode->next = curr;
            pred->next = newNode;
        }
        unlock(pred->mutex);
        unlock(curr->mutex);
    }

    return ret;
}

FindResult find(ThinList *list, int key) {
    ThinNode *pred, *curr;
    FindResult result;

    lock(list->head->mutex);

    pred = list->head;
    curr = pred->next;

    lock(curr->mutex);

    if (curr == list->tail) {

        unlock(list->head->mutex);
        unlock(curr->mutex);
        result.exists = 0;
        result.value = 0;
    } else {
        while (curr != list->tail && curr->key < key) {
            unlock(pred->mutex);
            pred = curr;
            curr = curr->next;
            lock(curr->mutex);
        }


        if (curr != list->tail && key == curr->key) {
            result.exists = 1;
            result.value = curr->val;
        } else
            result.exists = 0;

        unlock(pred->mutex);
        unlock(curr->mutex);
    }

    return result;
}

char myRemove(ThinList *list, int key) {
    ThinNode *pred, *curr;
    char ret = 1;

    lock(list->head->mutex);

    pred = list->head;
    curr = pred->next;

    lock(curr->mutex);

    if (curr == list->tail) {

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

        if (curr != list->tail && key == curr->key) {
//            *val = curr->val;
            pred->next = curr->next;

            unlock(curr->mutex);free_node(curr);
            unlock(pred->mutex);
        } else {
            unlock(pred->mutex);
            unlock(curr->mutex);

            ret = 0;
        }
    }
    return ret;
}

ThinList *init_list(void) {
    ThinList *list;

    if ((list = (ThinList *) calloc(1, sizeof(ThinList))) == NULL) {
        return NULL;
    }

    if ((list->head = (ThinNode *) calloc(1, sizeof(ThinNode))) == NULL) {
        free(list);
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

    end:
    free(list->head);
    free(list);
    return NULL;
}

void *addNode(ThinNode *pred, ThinList *list_copy) {
    ThinNode *pred_temp = list_copy->head,
            *curr_temp = pred_temp->next,
            *newNode = create_node(pred->key, pred->val);

    while (curr_temp != list_copy->tail && curr_temp->key < pred->key) {
        pred_temp = curr_temp;
        curr_temp = curr_temp->next;
    }

    newNode->next = curr_temp;
    pred_temp->next = newNode;
}

//ThinList *get_snapshot(ThinList *list) {
//    ThinList *list_copy = init_list();
//    ThinNode *pred, *curr;
//
//    lock(list->head->mutex);
//    pred = list->head;
//    curr = pred->next;
//    lock(curr->mutex);
//
//    if (curr == list->tail) {
//        unlock(list->head->mutex);
//        unlock(curr->mutex);
//
//        return list_copy;
//    }
//
//    while (curr != list->tail) {
//        unlock(pred->mutex);
//        pred = curr;
//        curr = curr->next;
//        lock(curr->mutex);
//
//        addNode(pred, list_copy);
//    }
//
//    unlock(curr->mutex);
//
//    return list_copy;
//}

bool compareNodes(ThinNode *node1, ThinNode *node2) {
    if (node1->key == node2->key && node1->val == node2->val)
        return true;
    else
        return false;
}

ThinList *copyList(ThinList *list, ThinList *list_copy) {
    ThinNode *curr = list->head;

    list_copy = init_list();

    while (curr->next != list->tail) {
        addNode(curr->next, list_copy);
        curr = curr->next;
    }

    return list_copy;
}

ThinList *get_snapshot(ThinList *list) {
    ThinList *list_copy = init_list();
    ThinNode *curr, *curr_temp_copy;

    list_copy = copyList(list, list_copy);

    curr = list->head;
    curr_temp_copy = list_copy->head;

    while (curr->next != list->tail) {
        if (!compareNodes(curr, curr_temp_copy)) {
            list_copy = copyList(list, list_copy);

            curr = list->head;
            curr_temp_copy = list_copy->head;
        } else {
            curr = curr->next;
            curr_temp_copy = curr_temp_copy->next;
        }
    }

    return list_copy;
}