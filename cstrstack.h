

// structure:
// char*cur; elems...
// elem:
// [char]; int len;
typedef char **cstrstack;
void *cssnew();

void csspush(cstrstack st, char *str);

bool cssempty(cstrstack st);

char *csspop(cstrstack st);
