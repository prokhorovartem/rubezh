#include <assert.h>
#include "task1.h"

int main() {
    RoughList *list = init_list();
    insert(list, 1, 2);
    {
        FindResult result = find(list, 1);
        assert(result.exists == 1);
        assert(result.value == 2);
    }

    FindResult result = find(list, 0);
    assert(result.exists == 0);

    insert(list, 0, 1);
    {
        FindResult result = find(list, 0);
        assert(result.exists == 1);
        assert(result.value == 1);
        insert(list, 2, 3);
        {
            FindResult result = find(list, 2);
            assert(result.exists == 1);
            assert(result.value == 3);
        }
        assert(myRemove(list, 1));
    }
    {
        FindResult result = find(list, 1);
        assert(result.exists == 0);
    }
    assert(myRemove(list, 0));
    assert(myRemove(list, 2));
    {
        FindResult result = find(list, 0);
        assert(result.exists == 0);
    }
    {
        FindResult result = find(list, 1);
        assert(result.exists == 0);
    }
    {
        FindResult result = find(list, 2);
        assert(result.exists == 0);
    }

    insert(list, 1, 2);
    insert(list, 6, 4);
    insert(list, 4, 6);

    RoughList *roughList = get_snapshot(list);

    myRemove(list, 0);
    myRemove(list, 1);
    myRemove(list, 2);
    myRemove(list, 6);
    myRemove(list, 5);
    myRemove(list, 4);
    myRemove(list, 10);

    assert(roughList->head != NULL);
    {
        FindResult result = find(roughList, 1);
        assert(result.exists == 1);
        assert(result.value == 2);
    }
    {
        FindResult result = find(roughList, 4);
        assert(result.exists == 1);
        assert(result.value == 6);
    }
    {
        FindResult result = find(roughList, 6);
        assert(result.exists == 1);
        assert(result.value == 4);
    }
    {
        FindResult result = find(roughList, 0);
        assert(result.exists == 0);
    }
    {
        FindResult result = find(roughList, 2);
        assert(result.exists == 0);
    }
    {
        FindResult result = find(roughList, 5);
        assert(result.exists == 0);
    }
    {
        FindResult result = find(roughList, 10);
        assert(result.exists == 0);
    }
}