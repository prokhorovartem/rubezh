#include "task4.h"

ConcurrentQueue *q;

int main(int argc, char **argv) {
    int i;
    val_t val;

    int max = 10;

    q = init_queue();

    for (i = 0; i < max; i++) {
        enqueue(q, i);
        show_queue(q);
    }

    for (i = 0; i < max; i++) {
        dequeue(q, &val);
        show_queue(q);
    }

    free_queue(q);
    return 0;
}