#ifndef structures_list_h
#define structures_list_h

#define LIST_LOOP(_list) for (Node *node = _list->firstNode; node; node = node->next)

typedef struct _node
{
    void * object;
    struct _node *prev;
    struct _node *next;
} Node;

typedef struct
{
    Node * firstNode;
    Node * lastNode;
    int count;
} List;

List * list_init();

//Insert at the end
List * list_append(List *aList, void *object);

// Insert at the begining
List * list_push(List *aList, void *object);

// Remove the first
void * list_pop(List *aList);

// Remove the last
void * list_dequeue(List *aList);

void list_iterate(List *aList, void (*obj_iteration)(void *obj, int i, int count, int *stop));


#endif
