#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define _capture1(a) typeof(a) a
#define _capture2(aa, b) typeof(aa) aa, typeof(b) b
#define _capture3(a, b, c) typeof(a) a, typeof(b) b, typeof(c) c
#define _capture4(a, b, c, d) typeof(a) a, typeof(b) b, typeof(c) c, typeof(d) d

#define args(...) __VA_ARGS__

#define _concat_macro_(a, b) a##b
#define _concat_macro(a, b) _concat_macro_(a, b)
#define _combine_args(...) __VA_ARGS__
#define _lambda_func(ret, def,... ) ret _concat_macro(lambda_func_, __LINE__)(__VA_ARGS__) def

#define _struct_of_lambda(_lambda_func_name, ...) \
    struct _concat_macro(_lambda_struct_,__LINE__)                                 \
    {                                             \
        _struct_field2(__VA_ARGS__);              \
        ret (*func) (struct _concat_macro(_lambda_struct_,__LINE__)* lmd,_declare2(arguements));           \
    }
#define _struct_field2(_1, b) \
  typeof(_1) _1;            \
   typeof(b) b
#define _assign2(var,a,b) typeof(a)a=var->a;typeof(b)b=var->b
#define _rassign2(var,a,b) var->a=a;var->b=b

#define lambda(name,ret, arguements, captures,def) \
    \
    _lambda_func(ret,def,_declare2(arguements),_capture2(captures)) \
    \
    struct _concat_macro(_lambda_struct_,__LINE__)                                 \
    {                                             \
        _struct_field2(captures);              \
        ret (*func) (struct _concat_macro(_lambda_struct_,__LINE__)* lmd,_declare2(arguements));           \
    } \
     ; \
    \
    ret _concat_macro(lambda_func_wrapper_, __LINE__)(struct _concat_macro(_lambda_struct_,__LINE__)* lmd,_declare2(arguements)) \
        {_assign2(lmd,captures);return _concat_macro(lambda_func_, __LINE__)(_combine_args(_passing2(arguements),captures));} \
    struct _concat_macro(_lambda_struct_,__LINE__) *name =alloc(struct _concat_macro(_lambda_struct_,__LINE__)); \
    _rassign2(name,captures);name->func=&_concat_macro(lambda_func_wrapper_, __LINE__)

#define capture(...) __VA_ARGS__
#define _declare2(t1,a1,t2,a2) t1 a1,t2 a2
#define _passing2(t1,a1,t2,a2) a1,a2

#define call(lmd,...) lmd->func(lmd,__VA_ARGS__)

int main(int argc, char **argv)
{
    int a = 4;
    int *b = &a;

    lambda(lmd, int, args(int, c, int, d), capture(a, b), {
        *b=2;
        return a + *b + c + d; });

    int c=call(lmd,3,4);
    printf("%d\n",c);
}