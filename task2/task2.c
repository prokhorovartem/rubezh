#include <stdio.h>
#include <stdlib.h>
#include "thinList.h"

//FindResult find(ThinList *list, int key);

char insert(ThinList *list, int key, int value) {
    if (pthread_mutex_lock(&list->root[key].mutex) != 0) {
        perror("pthread_mutex_lock() error");
        exit(1);
    }
    if (pthread_mutex_lock(&list->root[key].next->mutex) != 0) {
        perror("pthread_mutex_lock() error");
        exit(1);
    }

    ThinNode savedThinNode = list->root[key];
    struct ThinNode thinNode;
    thinNode.key = key;
    thinNode.value = value;
    thinNode.next = &savedThinNode;

    list->root[key] = thinNode;

    if (pthread_mutex_unlock(&list->root[key].mutex) != 0) {
        perror("pthread_mutex_unlock() error");
        exit(1);
    }
    if (pthread_mutex_unlock(&list->root[key].next->mutex) != 0) {
        perror("pthread_mutex_unlock() error");
        exit(1);
    }

    printf("Element inserted");
}

//char remove(ThinList *list, int key);