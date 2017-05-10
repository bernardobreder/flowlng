#include <stdio.h>
#include <stdlib.h>

static void flow_linked_list_sort_split(void *source, void **frontRef, void **backRef, void* (*next)(void*), void (*setnext)(void*, void*)) {
    void *fast, *slow;
    if (!source || !next(source)) {
        *frontRef = source;
        *backRef = 0;
    } else {
        slow = source;
        fast = next(source);
        while (fast) {
            fast = next(fast);
            if (fast) {
                slow = next(slow);
                fast = next(fast);
            }
        }
        *frontRef = source;
        *backRef = next(slow);
        setnext(slow, 0);
    }
}

static void* flow_linked_list_sort_node(void* a, void* b, int (*cmp)(void*, void*), void* (*next)(void*), void (*setnext)(void*, void*)) {
    void* result = 0;
    if (!a) return b;
    else if (!b) return a;
    if (cmp(a, b) <= 0) {
        result = a;
        setnext(result, flow_linked_list_sort_node(next(a), b, cmp, next, setnext));
    } else {
        result = b;
        setnext(result, flow_linked_list_sort_node(a, next(b), cmp, next, setnext));
    }
    return (result);
}

void flow_linked_list_sort(void** self, int (*cmp)(void*, void*), void* (*next)(void*), void (*setnext)(void*, void*)) {
    void *head = *self;
    void *a, *b;

    if (!head || !next(head)) return;

    flow_linked_list_sort_split(head, &a, &b, next, setnext);

    flow_linked_list_sort(&a, cmp, next, setnext);
    flow_linked_list_sort(&b, cmp, next, setnext);

    *self = flow_linked_list_sort_node(a, b, cmp, next, setnext);
}

