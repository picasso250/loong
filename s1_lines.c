#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "s1_lines.h"

line *lines = NULL;
int lines_len = 0;
int lines_cap = 0;

bool iscrlf(c)
{
    return c == '\r' || c == '\n';
}
line line_append(line ln, char c)
{
    if (ln.len == ln.cap)
    {
        ln.cap = (int)((ln.len + 4) * 1.75);
        ln.data = realloc(ln.data, sizeof(char) * (ln.cap));
        if (ln.data == NULL)
        {
            perror("realloc line.data failed");
            abort();
        }
    }
    ln.data[ln.len++] = c;
    return ln;
}
line rtrim(line ln)
{
    while (ln.len - 1 >= 0)
    {
        char c = ln.data[ln.len - 1];
        if (!(c == ' ' || iscrlf(c)))
            break;
        ln.len--;
    }
    return ln;
}
line empty_line()
{
    line ln = {.no = 0, .len = 0, .cap = 0, .data = NULL};
    return ln;
}
// return new line which is empty
line lines_append(int line_no, line ln)
{
    if (lines_len == lines_cap)
    {
        lines_cap = (int)((lines_len + 4) * 1.75);
        lines = realloc(lines, sizeof(ln) * (lines_cap));
        if (lines == NULL)
        {
            perror("realloc line.data failed");
            abort();
        }
    }
    ln.no = line_no;
    lines[lines_len++] = rtrim(ln); // it takes ln.data away
    return empty_line();
}
int lgetc_pos = 0;
int lgetc_line = 0;
int lgetc_line_pos = 0;
int lgetc_buffer[3] = {-2, -2, -2};
int lgetc()
{
    int c;
    if (lgetc_buffer[2] != -2)
    {
        c = lgetc_buffer[2];
        lgetc_buffer[2] = -2;
        return c;
    }
    if (lgetc_line >= lines_len)
    {
        return EOF;
    }
    if (lgetc_line_pos >= lines[lgetc_line].len)
    {
        return EOF;
    }
    c = lines[lgetc_line].data[lgetc_line_pos];
    lgetc_line_pos++;
    if (lgetc_line_pos >= lines[lgetc_line].len)
    {
        lgetc_line_pos = 0;
        lgetc_line++;
    }
    lgetc_pos++;

    //shift
    lgetc_buffer[0] = lgetc_buffer[1];
    lgetc_buffer[1] = c;

    return c;
}
int lgetoldc()
{
    return lgetc_buffer[0];
}
void lungetc(int c)
{
    if (lgetc_buffer[2] == -2)
    {
        perror("lungetc already buffer");
        abort();
    }
    lgetc_buffer[2] = c;
}