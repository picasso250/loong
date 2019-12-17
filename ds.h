
typedef struct IntList
{
    int cap;
    int start;
    int end;
    int *data;
} IntList;

void U(IntList *lst, int a); // pUsh
int O(IntList *lst);         // pOp

void E(IntList *lst, int a); // Enque
int D(IntList *lst);         // Deque

IntList *_NewIntList();
IntList *NewIntList();
int LenIntList(IntList *lst);

#include "util.h"

void U(IntList *lst, int a); // pUsh
int O(IntList *lst);         // pOp

static int incrAfter(int *p, int cap)
{
    return *p < cap ? (*p)++ : (*p = 0);
}
static int decrBefore(int *p, int cap)
{
    return *p >= 0 ? --*p : (*p = cap - 1);
}
void E(IntList *lst, int a) // Enque
{
    lst->data[incrAfter(&lst->end, lst->cap)] = a;
}
int D(IntList *lst) // Deque
{
    return lst->data[decrBefore(&lst->start, lst->cap)];
}

IntList *_NewIntList(int cap)
{
    IntList *lst = alloc(IntList);
    if (cap)
    {
        lst->data = (int *)_alloc_and_zero(sizeof(int) * cap, __FILE__, __LINE__, "data");
    }
    return lst;
}
IntList *NewIntList()
{
    return _NewIntList(0);
}

int LenIntList(IntList *lst)
{
    return lst->end - lst->start;
}
