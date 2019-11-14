
if [ $# -eq 1 ]; then
    if [ ! -f "$1.c" ]; then
        echo "no file $1.c"
    fi
    if [ ! -f "$1_test.c" ]; then
        echo "no file $1_test.c"
    fi

    # gen
    fname=.test$1.c
    echo "#include \"test.h\"" > $fname
    grep -o -P '\btest\w+\(\)' "$1_test.c" | sed 'i\void
    a\;' >> $fname
    echo "int main(int argc, char *argv[]){" >> $fname
    grep -o -P '\btest\w+\(\)' "$1_test.c"
    grep -o -P '\btest\w+\(\)' "$1_test.c" | sed 'a\;' >> $fname
    echo "printf(\"OK.\\n\"); }" >> $fname

    # compile and run
    gcc -std=c11 -o .test$1 "util.c" "$1.c" "$1_test.c" $fname && ./.test$1
else
    echo "todo all test"
fi