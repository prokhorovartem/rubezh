#include "task1.h"

int main() {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    RoughNode node1, node2;
    node1.key = 1;
    node1.value = 1;
    node2.key = 0;
    node2.value = 2;
    node2.next = &node1;
    RoughList list;
    list.head = &node2;
    list.tail = &node1;
    list.mtx = mutex;
    FindResult find1 = find(&list, 1);
}
