#include "task2.h"

int main() {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    ThinNode node1, node2;
    node1.key = 1;
    node1.value = 1;
    node1.mutex = mutex;
    node2.key = 0;
    node2.value = 2;
    node2.mutex = mutex;
    node2.next = &node1;
    ThinList list;
    list.root = &node2;
    FindResult find1 = find(&list, 1);
}