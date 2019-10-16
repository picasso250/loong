#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"

bool iscrlf(int c)
{
    return c == '\r' || c == '\n';
}
strbuf *strbuf_new()
{
    strbuf *sb = strbuf_alloc();
    strbuf_init(sb);
    return sb;
}
strbuf *strbuf_alloc()
{
    strbuf *sb = malloc(sizeof(strbuf));
    if (sb == NULL)
    {
        perror("not enough memory");
        abort();
    }
    return sb;
}
void strbuf_init(strbuf *sb)
{
    sb->len = sb->pos = 0;
    sb->cap = 10;
    sb->data = malloc(sb->cap);
    if (sb->data == NULL)
    {
        perror("not enough memory");
        abort();
    }
    sb->buf[0] = sb->buf[1] = sb->buf[2] = -2;
}
void strbuf_expand(strbuf *sb, int minlen)
{
    int nature = (int)((sb->len + 4) * 1.75);
    sb->cap = nature < minlen ? minlen : nature;
    sb->data = realloc(sb->data, sb->cap);
    if (sb->data == NULL)
    {
        perror("realloc strbuf.data failed");
        abort();
    }
}
void strbuf_append(strbuf *sb, const char c)
{
    if (sb->len == sb->cap)
        strbuf_expand(sb, 1);
    sb->data[sb->len++] = c;
}
void strbuf_fit(strbuf *sb)
{
    if (sb->len != sb->cap)
    {
        sb->data = realloc(sb->data, sb->len);
        if (sb->data == NULL)
        {
            perror("realloc strbuf.data failed");
            abort();
        }
        sb->cap = sb->len;
    }
}
void rtrim(strbuf *sb)
{
    while (sb->len - 1 >= 0)
    {
        char c = sb->data[sb->len - 1];
        if (!(c == ' ' || iscrlf(c)))
            break;
        sb->len--;
    }
}
void strbuf_appendlf(strbuf *sb)
{
    rtrim(sb);
    strbuf_append(sb, '\n');
}
int strbuf_getc(strbuf *sb)
{
    int c;
    if (sb->buf[2] != -2)
    {
        c = sb->buf[2];
        sb->buf[2] = -2;
        return c;
    }

    if (sb->pos < sb->len)
        c = sb->data[sb->pos];
    else
        c = EOF;
    sb->pos++;

    //shift
    sb->buf[0] = sb->buf[1];
    sb->buf[1] = c;

    return c;
}
void strbuf_ungetc(strbuf *sb, int c)
{
    if (sb->buf[2] != -2)
    {
        perror("strbuf_ungetc already buffer");
        abort();
    }
    sb->buf[2] = c;
}
void strbuf_concat(strbuf *sb, strbuf *b)
{
    int newlen = sb->len + b->len;
    if (newlen > sb->cap)
        strbuf_expand(sb, newlen);
    memcpy(sb->data + sb->len, b->data, b->len);
    sb->len = newlen;
}
void strbuf_print(strbuf *sb){
    for(int i = 0; i < sb->len; i++){
        putchar(sb->data[i]);
    }
}