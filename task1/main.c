#include <cassert>
#include "task1.h"

void main() {
    RoughList *list;
    list = init();
    insert(list, 1, 2);

    {
        FindResult result = find(list, 1);
        assert(result.exists);
        assert(result.value == 2);
    }

    {
        FindResult result = find(list, 0);
        assert(!result.exists);
    }

    insert(list, 0, 1);

    {
        FindResult result = find(list, 0);
        assert(result.exists);
        assert(result.value == 1);
    }

    insert(list, 2, 3);
    {
        FindResult result = find(list, 2);
        assert(result.exists);
        assert(result.value == 3);
    }
    assert(myRemove(list, 1));

    {
        FindResult result = find(list, 1);
        assert(!result.exists);
    }

    assert(myRemove(list, 0));
    assert(myRemove(list, 2));

    {
        FindResult result = find(list, 0);
        assert(!result.exists);
    }

    {
        FindResult result = find(list, 1);
        assert(!result.exists);
    }

    {
        FindResult result = find(list, 2);
        assert(!result.exists);
    }
}

int main() {
    simple_test();
    return 0;
}