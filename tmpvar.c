typedef struct tmpvarname
{
    int id;
    char *name;
} tmpvarname;
tmpvarname tmpvar()
{
    static int i = 0;
    char *s = malloc(55);
    strcpy(s, "_tmp_var_");
    strcpy(s + strlen("_tmp_var_"), itoa(i));
    return (tmpvarname){i++, s};
}