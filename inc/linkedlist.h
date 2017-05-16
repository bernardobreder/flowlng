
#define flow_linkedlist_inline_new(HEAD, TAIL, TYPE) TYPE *HEAD = 0, *TAIL = 0

#define flow_linkedlist_inline_add(HEAD, TAIL, NODE) { if (TAIL) { TAIL->next = NODE; } else { HEAD = NODE; } } TAIL = NODE;
