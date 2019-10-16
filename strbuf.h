#ifndef STRBUF_H
#define STRBUF_H

typedef struct strbuf
{
    int len;
    int cap;
    char *data;
    int pos;
    char buf[3];
} strbuf;

bool iscrlf(int c);
strbuf *strbuf_new();
strbuf *strbuf_alloc();
void strbuf_init(strbuf *sb);
void strbuf_append(strbuf *sb, const char c);
void strbuf_appendlf(strbuf *sb);
void strbuf_concat(strbuf *sb, strbuf *b);
void strbuf_fit(strbuf *sb);
int strbuf_getc(strbuf *sb);
void strbuf_ungetc(strbuf *sb, int c);
void strbuf_print(strbuf *sb);

#endif