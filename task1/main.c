#include <assert.h>
#include "task1.h"

int main() {
    RoughList *list = init_list();
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

    insert(list, 1, 2);
    insert(list, 3, 3);
    insert(list, 2, 4);

    RoughList roughList = get_snapshot(list);

    assert(roughList.head != NULL);
}