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
    slice *csv = parseCSVFile("operators.csv");
    initmap(&priorityTable, int, len(csv));
    initmap(&directionTable, int, len(csv));
    initmap(&nTable, int, len(csv));
    notNull(csv);
    int p = 0;
    forslice(csv, i, slice, line)
    {
        if (i == 0)
            continue;
        str k = get(&line, 'B' - 'A', str);
        int n = atoi(cstr(getp(&line, 'I' - 'A', str)));
        if (n == 1) // unary, add . after
        {
            strExtendCstr(&k, ".");
        }
        char *key = cstr(&k);

        str *s = (getp(&line, 'A' - 'A', str));
        if (len(s) > 0)
            p = atoi(cstr(s));
        set(&priorityTable, key, int, p);

        set(&directionTable, key, int, atoi(cstr(getp(&line, 'F' - 'A', str))));
    }
    endforslcie
    // _mapGraph(&priorityTable, "%d");
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

void popTo(slice *stack, slice *output)
{
    char *op;
    pop(stack, char *, op);
    push(output, char *, op);
}
static bool isPBLeft(char *op)
{
    return strcmp(op, "(") == 0 || strcmp(op, "[") == 0;
}
int main()
{
    initTable();

    char *tklst[] = {"1", "*", "(", "2", "+", "3", ")", "*", "4"};
    // char *tklst[] = {"1", "-", "2", "+", "3"};
    // char *tklst[] = {"1", "-", "2", "+", "3"};
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
        else if (get(&priorityTable, tk, int)) // If it's an operator
        {
            printf("%s\n", cstr(toStr(&output)));
            if (len(&stack))
            {
            }
            // 让优先级高的大佬先走
            // 括号相当于一个新的栈
            while (len(&stack) > 0 && !isPBLeft(top(&stack)) && cmp(top(&stack), tk) > 0)
            {
                popTo(&stack, &output);
            }
            push(&stack, char *, tk);
        }
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
    }
    while (len(&stack) > 0)
        popTo(&stack, &output);
    printf("%s\n", cstr(toStr(&output)));
}