TEMP=`getopt -o c --long compile-only \
     -n 'error' -- "$@"`

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi

# Note the quotes around `$TEMP': they are essential!
#set 会重新排列参数的顺序，也就是改变$1,$2...$n的值，这些值在getopt中重新排列过了
eval set -- "$TEMP"

#经过getopt的处理，下面处理具体选项。

COMPILE_ONLY=0
while true ; do
        case "$1" in
                -c|--compile-only) COMPILE_ONLY=1 ; shift ;;
                --) shift ; break ;;
                *) echo "Internal error!" ; exit 1 ;;
        esac
done

if [ $# -eq 1 ]; then
    if [ ! -f "$1.c" ]; then
        echo "no file $1.c"
        exit 1
    fi
    if [ ! -f "$1_test.c" ]; then
        echo "no file $1_test.c"
        exit 1
    fi

    # gen
    fname=.test$1.c
    echo "#include \"test.h\"" > $fname
    grep -o -P '\btest\w+\(\)' "$1_test.c" | sed 'i\void
    a\;' >> $fname
    echo "int main(int argc, char *argv[]){" >> $fname
    if [[ $COMPILE_ONLY -eq 0 ]]; then
        grep -o -P '\btest\w+\(\)' "$1_test.c"
    fi
    grep -o -P '\btest\w+\(\)' "$1_test.c" | sed 'a\;' >> $fname
    echo "printf(\"OK.\\n\"); }" >> $fname

    # compile and run
    gcc -std=c11 -ggdb -o .test$1 "util.c" "$1.c" "$1_test.c" $fname
    if [[ $COMPILE_ONLY -eq 0 && $? -eq 0 ]]; then
        ./.test$1
    fi
else
    echo "todo all test"
fi