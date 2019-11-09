//#include <stdio.h>
//#include <assert.h>
//
//typedef struct RoughNode {
//    int key;
//    int value;
//    struct RoughNode *next;
//} RoughNode;
//
//typedef struct {
//    RoughNode *root;
//} RoughList;
//
//typedef struct {
//    char exists;
//    int value;
//} FindResult;
//
//FindResult find(RoughList *list, int key);
//
//char insert(RoughList *list, int key, int value);
//
//char myRemove(RoughList *list, int key);
//
//int main() {
//    RoughList list;
//    list.root = NULL;
//
//    insert(&list, 1, 2);
//
//    {
//        FindResult result = find(&list, 1);
//        assert(result.exists);
//        assert(result.value == 2);
//    }
//
//    {
//        FindResult result = find(&list, 0);
//        assert(!result.exists);
//    }
//
//    insert(&list, 0, 1);
//
//    {
//        FindResult result = find(&list, 0);
//        assert(result.exists);
//        assert(result.value == 1);
//    }
//
//    insert(&list, 2, 3);
//    {
//        FindResult result = find(&list, 2);
//        assert(result.exists);
//        assert(result.value == 3);
//    }
//    assert(myRemove(&list, 1));
//
//    {
//        FindResult result = find(&list, 1);
//        assert(!result.exists);
//    }
//
//    assert(myRemove(&list, 0));
//    assert(myRemove(&list, 2));
//
//    {
//        FindResult result = find(&list, 0);
//        assert(!result.exists);
//    }
//
//    {
//        FindResult result = find(&list, 1);
//        assert(!result.exists);
//    }
//
//    {
//        FindResult result = find(&list, 2);
//        assert(!result.exists);
//    }
//
//    printf("success");
//}