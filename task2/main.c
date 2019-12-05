#include <assert.h>
#include <task2.h>
#include <stdio.h>

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
}