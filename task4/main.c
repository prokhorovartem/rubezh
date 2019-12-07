#include <task4.h>
#include <pthread.h>

ConcurrentQueue *q;

pthread_t readerTid, writerTid;
pthread_attr_t attr;

void *read(void* data) {
    for (int i = 0; i < 100; i++)
        show_queue(q);
}

void *write(void* data) {
    for (int i = 0; i < 100; i++)
        enqueue(q, i);
}

int main(int argc, char **argv) {
    int i;
    int val;

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

    {
        q = init_queue();

        pthread_attr_init(&attr);
        pthread_create(&readerTid, &attr, read, NULL);
        pthread_create(&writerTid, &attr, write, NULL);

        pthread_join(readerTid, NULL);
    }

    return 0;
}