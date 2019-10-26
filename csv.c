
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
    int state = 0; // 0:OUT 1:IN of string
    char last;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == ',' && state == 0)
        {
            push(line, str, *word);
            free((word));
            word = newStr();
        }
        else if (c == '"')
        {
            switch (state)
            {
            case 0:
                state = 1;
                break;
            case 1:
                state = 0;
                break;
            default:
                error("should not here");
            }
        }
        else if (c == '\r')
        {
            if ((c = fgetc(fp)) == EOF)
                break;
            else if (c == '\n')
            {

                goto LINE_END;
            }
            else
                ungetc(c, fp);
        }
        else if (c == '\n')
        {

            goto LINE_END;
        }
        else
        {
            push(word, char, c); // 228
        }
        last = c;
        continue;
    LINE_END:
        push(line, str, *word);
        free((word));
        word = newStr();
        push(lines, slice, *line);
        printf("%s\n", cstr(toStr(line)));
        free((line));
        line = newslice(str, 0, 0);
        state = 0;
        last = '\n';
    }

    if (!(last == '\n' || last == '\r'))
    {
        push(line, str, *word);
        push(lines, slice, *line);
    }

    free((word));
    // word = newStr();
    free((line));
    // line = newslice(str, 0, 0);
    // printf("%s\n", cstr(toStr(lines)));
    fclose(fp);
    // if (len(lines) > 0)
    // {
    //     slice lastLine = get(lines, len(lines) - 1, slice);
    //     if (len(&lastLine) == 1 && len(&get(&lastLine, 0, str)) == 0)
    //         lines->len--;
    // }
    return lines;
}
// int main(int argc, char *argv[])
// {
//     slice *lines = parseCSVFile(argv[1]);
//     notNull(lines);
//     printf("%s\n", cstr(toStr(lines)));
//     return 0;
// }