
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "util.h"
#include "buildin.h"
#include "print.h"

slice *parseCSVFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;
    int c;
    str *word = newStr();
    slice *line = newslice(str, 0, 0);
    slice *lines = newslice(slice, 0, 0);
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == ',')
        {
            push(line, str, *word);
            free((word));
            word = newStr();
        }
        else if (c == '\r')
        {
            if ((c = fgetc(fp)) == EOF)
                goto FILE_END;
            else if (c == '\n')
            {
                if ((c = fgetc(fp)) == EOF)
                    goto FILE_END_LINE;
                else
                {
                    ungetc(c, fp);
                    goto LINE_END;
                }
            }
            else
                ungetc(c, fp);
        }
        else if (c == '\n')
        {
            if ((c = fgetc(fp)) == EOF)
                goto FILE_END_LINE;
            else
            {
                ungetc(c, fp);
                goto LINE_END;
            }
        }
        else
        {
            push(word, char, c);// 228
            // char *ppp = (char *)_push("char", (word));
            // (*ppp = (char)(c));
        }
        continue;
    LINE_END:
        push(line, str, *word);
        free((word));
        word = newStr();
        push(lines, slice, *line);
        // printf("%s\n", cstr(toStr(line)));
        free((line));
        line = newslice(str, 0, 0);
    }
FILE_END:
    push(line, str, *word);
    push(lines, slice, *line);
FILE_END_LINE:
    free((word));
    // word = newStr();
    free((line));
    // line = newslice(str, 0, 0);
    printf("%s\n", cstr(toStr(lines)));
    fclose(fp);
    // if (len(lines) > 0)
    // {
    //     slice lastLine = get(lines, len(lines) - 1, slice);
    //     if (len(&lastLine) == 1 && len(&get(&lastLine, 0, str)) == 0)
    //         lines->len--;
    // }
    return lines;
}
int main(int argc, char *argv[])
{
    slice*lines=parseCSVFile(argv[1]);
    notNull(lines);
    return 0;
}