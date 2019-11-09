#include <pthread.h>
#include <stdio.h>
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

int main() {
    ThinList thinList;
    ThinNode thinNode;
    thinNode.key = 0;
    thinNode.value = 1;

    thinList.root = &thinNode;

    insert(&thinList, 1, 2);

    printf("123");
}

