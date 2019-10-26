#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "buildin.h"
#include "str.h"
#include "print.h"
#include "csv.h"

map priorityTable;
map directionTable;
map nTable;

void initTable()
{
    initmap(&priorityTable, int);
    initmap(&directionTable, int);
    initmap(&nTable, int);
    slice *csv = parseCSVFile("operators.csv");
    notNull(csv);
    int p = 0;
    forslice(csv, i, slice, line)
    {
        if (i==0) continue;
        str k = get(&line, 'B' - 'A', str);
        int n = atoi(cstr(getp(&line, 'I' - 'A', str)));
        if (n == 1) // unary
        {
            str ss;
            initslice(&ss, char, 0, 0);
            strExtendCstr(&ss, ".");
            strExtend(&ss, &k);
            k = ss;
        }
        char *key = cstr(&k);

        str *s = (getp(&line, 'A' - 'A', str));
        if (len(s) > 0)
            p = atoi(cstr(s));
        set(&priorityTable, key, int, p);

        set(&directionTable, key, int, atoi(cstr(getp(&line, 'F' - 'A', str))));
    }
    endforslcie
}
int priority(const char *op)
{
    return get(&priorityTable, op, int);
}
int direction(const char *op)
{
    return get(&directionTable, op, int);
}

int cmp(const char *op1, const char *op2)
{
    int pd = -(priority(op1) - priority(op2));
    if (pd == 0)
    {
        return direction(op1);
    }
}

char *top(slice *stack)
{
    return get(stack, len(stack) - 1, char *);
}

bool GreaterPrecedence(slice *stack, const char *tk)
{
    return len(stack) > 0 &&
           // priority of stack operator
           cmp(top(stack), (tk)) > 0;
}
void popTo(slice *stack, slice *output)
{
    char *op;
    pop(stack, char *, op);
    push(output, char *, op);
}
int main()
{
    initTable();

    // char *tklst[] = {"1", "*", "(", "2", "+", "3", ")","*", "4"};
    char *tklst[] = {"1", "-", "2", "+", "3"};
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