#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "strbuf.h"

typedef struct token
{
	int type;
	int line; // 行号
	strbuf str;
} token;

token *tklst = NULL;
int tklstlen = 0;
int tklstcap = 0;
strbuf tklst_str;

#define BEG 1
#define END 2
#define STR 3
#define CMT 4
#define WORD 5
#define OP 6

// CRLF or CR to LF
// rtrim the line
strbuf *step1(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror(filename);
		abort();
	}

	int c;
	strbuf *sb = strbuf_new();
	while ((c = fgetc(fp)) != EOF)
	{
		// CRLF
		if (c == '\r')
		{
			int lf = fgetc(fp);
			if (lf == EOF)
			{
				strbuf_appendlf(sb);
				break;
			}
			else if (lf == '\n')
			{
				;
			}
			else
			{
				if (EOF == ungetc(lf, fp))
				{
					perror("ungetc returned EOF");
					abort();
				}
			}
			strbuf_appendlf(sb);
		}
		else if (c == '\n')
		{
			strbuf_appendlf(sb);
		}
		else
		{
			strbuf_append(sb, c);
		}
	}

	if (sb->data[sb->len - 1] != '\n')
		strbuf_appendlf(sb);

	fclose(fp);
	return sb;
}
void print_tklst()
{
	for (int i = 0; i < tklstlen; i++)
	{
		int t = tklst[i].type;
		switch (t)
		{
		case BEG:
			printf("%d:", tklst[i].line);
		case END:
			printf("%c ", tklst[i].str.data[0]);
			break;
		case WORD:
		case OP:
			strbuf_print(&tklst[i].str);
			printf(" ");
			break;
		case STR:
			printf("\"");
			strbuf_print(&tklst[i].str);
			printf("\" ");
			break;
		case CMT:
			printf("#{");
			strbuf_print(&tklst[i].str);
			printf("} ");
			break;
		default:
			fprintf(stderr, "unknown type %d\n", t);
			abort();
		}
	}
}

void append(int type, int line, strbuf *sb)
{
	strbuf_fit(sb);

	if (tklstlen == tklstcap || tklst == NULL)
	{
		tklstcap = (int)((tklstlen + 4) * 1.75);
		tklst = realloc(tklst, sizeof(token) * tklstcap);
		if (tklst == NULL)
		{
			perror("realloc tklst failed");
			abort();
		}
	}

	token tk;
	tk.type = type;
	tk.line = line;
	tk.str = *sb;
	tklst[tklstlen++] = tk; // take str's char* away

	// for debugging
	strbuf_concat(&tklst_str, sb);
}
void append_end(int line)
{
	static strbuf *sb = NULL;
	if (sb == NULL)
	{
		sb = strbuf_new();
		strbuf_append(sb, '$');
	}
	append(END, line, sb);
}
void append_begin(int line)
{
	static strbuf *sb = NULL;
	if (sb == NULL)
	{
		sb = strbuf_new();
		strbuf_append(sb, '^');
	}
	append(BEG, line, sb);
}
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s filename\n", argv[0]);
		abort();
	}

	strbuf *sb = step1(argv[1]); // lines

	// for debug
	strbuf_init(&tklst_str);

	// ^, $, string, and comment (1 2 3 4)
	int state = 0;
	append_begin(1); // append( 1, "^", 1); // first is ^
	strbuf *string = strbuf_new();
	strbuf *comment = strbuf_new();
	strbuf *word = strbuf_new();
	strbuf *operator= strbuf_new();
	int c;
	int line = 1;
	while ((c = strbuf_getc(sb)) != EOF)
	{
		switch (state)
		{
		case 0: // we are in the space
			if (isalnum(c))
			{
				state = 1; // we're in a word
				strbuf_init(word);
				strbuf_append(word, c);
			}
			else if (c == ' ')
			{
				// go on
			}
			else if (iscrlf(c))
			{
				append_end(line);
				line++;
				append_begin(line);
				state = 0;
			}
			else if (c == '"')
			{
				strbuf_init(string);
				state = 21; // we're in a string
			}
			else if (c == '`')
			{
				strbuf_init(string);
				int next = strbuf_getc(sb);
				if (next == EOF)
				{
					append(STR, line, string);
					break;
				}
				if (next == '{')
				{
					state = 23; // here doc string
				}
				else
				{
					strbuf_ungetc(sb, next);
					state = 22; // literal string
				}
			}
			else if (c == '#')
			{
				strbuf_init(comment);
				int next = strbuf_getc(sb);
				if (next == EOF)
					break;
				if (next == '{')
					state = 12; // multi-line comment
				else
				{
					strbuf_ungetc(sb, next);
					state = 11; // comment
				}
			}
			else
			{
				// operator
				state = 2;
				strbuf_init(operator);
				strbuf_append(operator, c);
			}
			break;

		case 1: // word
			if (isalnum(c))
			{
				strbuf_append(word, c);
			}
			else if (c == ' ')
			{
				// word end
				append(WORD, line, word);
				state = 0;
			}
			else if (iscrlf(c))
			{
				append(WORD, line, word);
				append_end(line);
				line++;
				append_begin(line);
				state = 0;
			}
			else if (c == '"')
			{
				append(WORD, line, word);
				strbuf_init(string);
				state = 3; // we're in a string
			}
			else
			{
				// operator
				append(WORD, line, word);
				strbuf_init(operator);
				strbuf_append(operator, c);
				state = 2;
			}
			break;

		case 2: // operator
			if (isalnum(c))
			{
				append(OP, line, operator);

				strbuf_init(word);
				strbuf_append(word, c);
				state = 1; // we're in a word
			}
			else if (c == ' ')
			{
				append(OP, line, operator);
				state = 0;
			}
			else if (iscrlf(c))
			{
				append(OP, line, operator);
				append_end(line);
				line++;
				append_begin(line);
				state = 0;
			}
			else if (c == '"')
			{
				append(OP, line, operator);

				strbuf_init(string);
				state = 21; // we're in a string
			}
			else if (NULL != strchr("()[]{},_#.", c)) // these operators can not be overloaded
			{
				append(OP, line, operator);

				strbuf_init(operator);
				strbuf_append(operator, c);
				append(OP, line, operator);

				state = 0;
			}
			else
			{
				strbuf_append(operator, c);
				state = 2;
			}
			break;

		case 11: // comment
			if (c == '\n')
			{
				append(CMT, line, comment);
				append_end(line);
				line++;
				append_begin(line);
				state = 0;
			}
			else
			{
				strbuf_append(comment, c);
			}
			break;
		case 12: // multi-line comment
			if (c == '}' && comment->len > 0 && comment->data[comment->len - 1] == '\n')
			{
				append(CMT, line, comment);
				state = 0;
			}
			else
			{
				if (c == '\n')
					line++;
				strbuf_append(comment, c);
			}
			break;

		case 21: // string
			if (c == '\\')
			{
				strbuf_append(string, c);
				if ((c = strbuf_getc(sb)) == EOF)
				{
					perror("string end with \\");
					abort();
				}
				strbuf_append(string, c); // escape one char
			}
			else if (c == '"')
			{
				append(STR, line, string);
				strbuf_init(string);
				state = 0; // we're out of string
			}
			else
			{
				if (c == '\n')
				{
					line++;
				}
				strbuf_append(string, c);
			}
			break;

		case 22: // literal string
			if (c == '\n')
			{
				append(STR, line, string);

				append_end(line);
				line++;
				append_begin(line);

				state = 0;
			}
			else
			{
				strbuf_append(string, c);
			}
			break;

		case 23: // here doc string
			if (c == '}' && string->len > 0 && string->data[string->len - 1] == '\n')
			{
				append(STR, line, string);
				state = 0;
			}
			else
			{

				strbuf_append(string, c);
				if (c == '\n')
				{
					line++;
				}
			}
			break;

		default:
			fprintf(stderr, "unknown state %d\n", state);
			abort();
		}
	}
	if (tklstlen > 0)
		tklstlen--; // remove trailing ^
	print_tklst();
}