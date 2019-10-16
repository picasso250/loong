#ifndef S1_LINES_H
#define S1_LINES_H

typedef struct line
{
    int no;
    int len;
    int cap;
    char *data;
} line;

bool iscrlf(c);
line empty_line();
line line_append(line ln, char c);
line lines_append(int line_no, line ln);
int lgetc();
int lgetoldc();
void lungetc(int c);

#endif