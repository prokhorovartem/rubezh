#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "task2.h"

typedef struct ThinTest {
    ThinList *list;
    int low;
    int high;
} ThinTest;

void *remove_test(ThinTest test){
    for(int i = test.low; i < test.high; i++)
        myRemove(test.list, i);
}

void *create_arr(ThinList *list){
    for(int i = 0; i < 100; i++)
        insert(list, i * 3, i);
}

int main() {
    ThinList *list = init_list();
    insert(list, 1, 2);
    {
        FindResult result = find(list, 1);
        assert(result.exists == '1');
        assert(result.value == 2);
    }

    FindResult result = find(list, 0);
    assert(result.exists == '0');

    insert(list, 0, 1);
    {
        FindResult result = find(list, 0);
        assert(result.exists == '1');
        assert(result.value == 1);
        insert(list, 2, 3);
        {
            FindResult result = find(list, 2);
            assert(result.exists == '1');
            assert(result.value == 3);
        }
        assert(myRemove(list, 1));
    }
    {
        FindResult result = find(list, 1);
        assert(result.exists == '0');
    }
    assert(myRemove(list, 0));
    assert(myRemove(list, 2));
    {
        FindResult result = find(list, 0);
        assert(result.exists == '0');
    }
    {
        FindResult result = find(list, 1);
        assert(result.exists == '0');
    }
    {
        FindResult result = find(list, 2);
        assert(result.exists == '0');
    }

    insert(list, 0, 1);
    insert(list, 1, 2);
    insert(list, 2, 3);
    insert(list, 3, 4);

    ThinList *copyThinList = get_snapshot(list);
    assert(copyThinList->head != NULL);



    ///////////////////////////
    pthread_t create_thread;
    pthread_create(&create_thread, NULL, create_arr, list);


    pthread_t thread1, thread2, thread3;

    ThinTest *thinTest1;
    thinTest1 = (ThinTest *) calloc(1, sizeof(ThinTest));
    thinTest1->list = list;
    thinTest1->low = 0;
    thinTest1->high = 33;
    pthread_create(&thread1, NULL, remove_test, thinTest1);

    ThinTest *thinTest2;
    thinTest2 = (ThinTest *) calloc(1, sizeof(ThinTest));
    thinTest2->list = list;
    thinTest2->low = 34;
    thinTest2->high = 66;
    pthread_create(&thread1, NULL, remove_test, thinTest2);

    ThinTest *thinTest3;
    thinTest3 = (ThinTest *) calloc(1, sizeof(ThinTest));
    thinTest3->list = list;
    thinTest3->low = 67;
    thinTest3->high = 100;
    pthread_create(&thread1, NULL, remove_test, thinTest1);

}