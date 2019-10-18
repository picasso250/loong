// #include <stdio.h>
// #include <stdlib.h>
// #include <stdlib.h>

#include "util.h"

#define GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define capture(...)                                                              \
    GET_MACRO(__VA_ARGS__, _capture4, _capture3, _capture2, _capture1, _capture0) \
    (__VA_ARGS__)
#define _capture0() (void)
#define _capture1(a) typeof(a) a
#define _capture2(a, b) typeof(a) a, typeof(b) b
#define _capture3(a, b, c) typeof(a) a, typeof(b) b, typeof(c) c
#define _capture4(a, b, c, d) typeof(a) a, typeof(b) b, typeof(c) c, typeof(d) d







#define do_lambda(lmd,...) (lmd->func)(lmd,__VA_ARGS__)



int main(int argc, char **argv){
    int a=4;int *b=&a;
    
    // lambda int lambda_func (int c,int d) capture(a,b){
    //     *b=2;
    //     return a+*b+c+d;
    // }

    typedef int (*_lambda_func_wrap_type_)(struct _lambda_struct*lmd,int c, int d);
    typedef int (*_lambda_func_inner_type_)(int c, int d,int a,int*b);
    struct _lambda_struct {int a;int *b; _lambda_func_wrap_type_ func; _lambda_func_inner_type_ func_inner};
    int _lambda_func_inner_(int c, int d,int a,int*b){body}
    int _lambda_func_wrap_(struct _lambda_struct*lmd,int c, int d){
        int a=lmd->a;int* b=lmd->b;
        return (*lmd->func_inner)(c,d,a,b);
    }
    lambda_name=alloc(struct _lambda_struct);
    lambda_name->a=a;lambda_name->b=b;
    lambda_name->func=&_lambda_func_wrap;
    lambda_name->func_inner=&_lambda_func_inner_;

    (*lmd->func)(lmd,3,4);

    // #define _make_lambda6(body,lambda_arg_list,lambda_arg_name_list,labmda_capture_list,lambda_capture_name_list,...) \

    
    // #define lambda_arg_list int c,int d
    // #define lambda_arg_name_list c,d
    // #define lambda_capture_list int a, int*b
    // #define lambda_capture_name_list a,b
    // #define lambda_capture_var_list int a;int*b;
    // #define return_type int

    #define AS(...) __VA_ARGS__
    #define lambda_inner_assgin(var) typeof(var)var=lmd->var ; ;
    #define lambda_inner_assgin2(a,b) typeof(a)a=lmd->a;typeof(b)b=lmd->b;
    #define lambda_assgin(lambda_name,var) lambda_name->var=var;
    #define lambda_assgin2(lambda_name,a,b) lambda_name->a=a;lambda_name->b=b;
    #define _concat_macro(a,b) a##b
    #define CONCAT_M(a, b) _concat_macro(a,b)

    #define _make_lambda_wrap_type(wrap_type_name,struct_name,...) \
        typedef int (*wrap_type_name)(struct struct_name*lmd,__VA_ARGS__)
    #define    _make_lambda_inner_type(inner_type_name,arg_list,capture_list) \
        typedef int (*inner_type_name)(arg_list,capture_list);
    #define _make_lambda_struct(struct_name,wrap_type_name,inner_type_name,capture_var_list) \
        struct struct_name {capture_var_list wrap_type_name func; inner_type_name func_inner;};
    #define _make_lambda_func_inner(body,return_type,inner_type_name, arg_list,capture_list) \
        return_type inner_type_name(arg_list,capture_list)body
    #define _make_lambda_func_wrap(return_type,wrap_type_name,struct_name,arg_list,arg_name_list,capture_name_list,inner_assgin) \
        return_type wrap_type_name(struct struct_name*lmd,arg_list){inner_assgin \
            return (*lmd->func_inner)(arg_name_list,capture_name_list);        } 
    #define _make_lambda_assgin(lambda_name,struct_name,wrap_type_name,inner_type_name,assgin) \
        struct struct_name*lambda_name=alloc(struct struct_name);        assgin \
        lambda_name->func=&wrap_type_name;         lambda_name->func_inner=&inner_type_name;

    #define _make_lambda6(lambda_name,return_type,body,arg_list,arg_name_list,capture_list,capture_name_list,capture_var_list,inner_assgin,assgin) \
        _make_lambda_wrap_type(CONCAT_M(_lambda_wrap_type_,__LINE__),CONCAT_M(_lambda_struct_,__LINE__),arg_list); \
        _make_lambda_inner_type(CONCAT_M(_lambda_inner_type_,__LINE__),AS(arg_list),AS(capture_list)); \
        _make_lambda_struct(CONCAT_M(_lambda_struct_,__LINE__),CONCAT_M(_lambda_wrap_type_,__LINE__),CONCAT_M(_lambda_inner_type_,__LINE__),capture_var_list); \
        _make_lambda_func_inner(body,return_type,CONCAT_M(_lambda_func_inner_type_,__LINE__), AS(arg_list),AS(capture_list)) \
        _make_lambda_func_wrap(return_type,CONCAT_M(_lambda_func_wrap_type_,__LINE__),CONCAT_M(_lambda_struct_,__LINE__),AS(arg_list),AS(arg_name_list),AS(capture_name_list),inner_assgin) \
        _make_lambda_assgin(lambda_name,CONCAT_M(_lambda_struct_,__LINE__),CONCAT_M(_lambda_func_wrap_type_,__LINE__),CONCAT_M(_lambda_func_inner_type_,__LINE__),assgin)

_make_lambda6(lmD,int,{
        *b=2;
        return a+*b+c+d;
    },AS(int c,int d),AS(c,d),AS(int a,int *b),AS(a,b), AS(int a; int*b;),
    lambda_inner_assgin2(a,b), lambda_assgin2(lmD,a,b));
printf("%d\n",do_lambda(lmD,3,4));
}