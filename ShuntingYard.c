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

        set(&directionTable, key, int, atoi(cstr(getp(&line, 'G' - 'A', str))));
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
// 比较优先级 * &
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
    char *op = pop(stack, char *);
    push(output, char *, op);
}
// parentheses	括号 brackets
static bool isPBLeft(char *op)
{
    return strcmp(op, "(") == 0 || strcmp(op, "[") == 0;
}
void *compute(slice *computer, char *op);
int main()
{
    initTable();

    // char *tklst[] = {"1", "*", "(", "2", "+", "3", ")", "*", "4"}; // 括号
    char *tklst = "1-2+3"; // 结合性
    // char *tklst = "-1+a*(-2/6)";
    // char *tklst = "1-a[-3*2]+4";
    // char *tklst = "*&p";
    int tklstpos = 0;

    slice output;
    initslice(&output, char *, 0, 0);
    slice stack;
    initslice(&stack, char *, 0, 0);

    int state = 0; // 0:start, 1:in
    for (int i = 0; i < strlen(tklst); i++)
    {
        char *tk = malloc(99);
        tk[0] = tklst[i];
        tk[1] = 0;

        printf("---\nSTK: %s\n", cstr(toStr(&stack)));
        printf("OUT: %s\n", cstr(toStr(&output)));

        if (isalnum(tk[0]))
        { // operand
            push(&output, char *, tk);
            state = 1;
        }
        else if (get(&priorityTable, tk, int)) // If it's an operator
        {
            if (state == 0) // unary
            {
                strcpy(tk + 1, ".");
            }
            // 任何符号都要到栈里走一遭
            // 让优先级高的大佬先离开
            // 括号相当于一个新的栈
            while (len(&stack) > 0 && !isPBLeft(top(&stack)) && cmp(top(&stack), tk) > 0)
            {
                popTo(&stack, &output);
            }
            push(&stack, char *, tk);
            state = 0;
        }
        else if (strcmp(tk, "(") == 0)
        {
            push(&stack, char *, tk);
            state = 0;
        }
        else if (strcmp(tk, ")") == 0)
        {
            while (len(&stack) > 0 && strcmp(top(&stack), "(") != 0)
                popTo(&stack, &output);
            if (len(&stack) == 0)
                error("parentheses() not balanced");
            stack.len--;
            state = 1;
        }
        else if (strcmp(tk, "[") == 0)
        {
            push(&stack, char *, "[]");
            push(&stack, char *, tk);
            state = 0;
        }
        else if (strcmp(tk, "]") == 0)
        {
            while (len(&stack) > 0 && strcmp(top(&stack), "[") != 0)
                popTo(&stack, &output);
            if (len(&stack) == 0)
                error("brackets[] not balanced");
            stack.len--;
            state = 1;
        }
    }
    while (len(&stack) > 0)
    {
        popTo(&stack, &output);
    }
    printf("%s\n", cstr(toStr(&output)));

    // output
    slice *computer = newslice(str, 0, len(&output));
    forslice(&output, i, char *, tk)
    {
        if (isalnum(tk[0]))
        {
            push(computer, str, *newStrFromCStr(tk));
        }
        else
        {
            // operator
            compute(computer, tk);
        }
    }
    endforslcie;
    if (len(computer) != 1)
        error("result?");
    printf("%s\n", cstr(getp(computer, 0, str)));

    return 0;
}
void *compute(slice *computer, char *op)
{
    if (op[1] == '.')
    {
        // unary
        str *res = newStrFromCStr("( ");
        strExtendCstr(res, op);
        strExtend(res, popp(computer, str));
        strExtendCstr(res, " )");
        push(computer, str, *res);
    }
    else
    {
        // binary
        str *res = newStrFromCStr("( ");
        str *op2 = popp(computer, str);
        str *op1 = popp(computer, str);
        strExtend(res, op1);
        strExtendCstr(res, op);
        strExtend(res, op2);
        strExtendCstr(res, " )");
        push(computer, str, *res);
    }
}