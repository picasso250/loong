#ifndef _CSTRSTACK_H_
#define _CSTRSTACK_H_

// structure:
// char*cur; elems...
// elem:
// [char]; padding; char* prev;
typedef char **cstrstack;

void *cssnew();

cstrstack csspush(cstrstack st, char *str);

bool cssempty(cstrstack st);

char *csspop(cstrstack st);

#endif // _CSTRSTACK_H_
