#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "buildin.h"
#include "str.h"
#include "print.h"

map priorityTable;
char *top(slice *stack)
{
    return get(stack, len(stack) - 1, char *);
}
int pr(const char *op)
{
    return get(&priorityTable, op, int);
}
bool GreaterPrecedence(slice *stack, const char *tk)
{
    return (len(stack) > 0 &&
            // priority of stack operator
            pr(top(stack)) > pr(tk));
}
void popTo(slice *stack, slice *output)
{
    char *op;
    pop(stack, char *, op);
    push(output, char *, op);
}
int main()
{
    initmap(&priorityTable, int);
    set(&priorityTable, "+", int, 10);
    set(&priorityTable, "-", int, 10);
    set(&priorityTable, "*", int, 20);
    set(&priorityTable, "/", int, 20);

    char *tklst[] = {"1", "*", "2", "+", "3","*", "4"};
    int tklstpos = 0;

    slice output;
    initslice(&output, char *, 0, 0);
    slice stack;
    initslice(&stack, char *, 0, 0);

    for (int i = 0; i < sizeof(tklst) / sizeof(tklst[0]); i++)
    {
        char *tk = tklst[i];
        if (isalnum(tk[0])) // operand
            push(&output, char *, tk);
        else if (strcmp(tk, "(") == 0)
            push(&stack, char *, tk);
        else if (strcmp(tk, ")") == 0)
        {
            while (len(&stack) > 0 && strcmp(top(&stack), "(") != 0)
                popTo(&stack, &output);
            if (len(&stack) == 0)
                error("parasis not compair");
            stack.len--;
        }
        else
        {
    printf("%s\n", cstr(toStr(&output)));
            while (GreaterPrecedence(&stack, tk))
                popTo(&stack, &output);
            push(&stack, char *, tk);
    printf("%s\n", cstr(toStr(&output)));
        }
    }
    while (len(&stack) > 0)
        popTo(&stack, &output);
    printf("%s\n", cstr(toStr(&output)));
}