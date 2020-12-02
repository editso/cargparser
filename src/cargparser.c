//
// Created by zy on 2020/11/30.
//
#include "cargparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cargparser_array{
    unsigned int capacity;
    unsigned int size;
    void** _el;
};

struct cargparser_args{
    struct cargparser_array* options;
    struct cargparser_array* groups;
};

typedef struct cargparser_argument_{
    unsigned int argc;
    unsigned int pos;
    char **argv;
}cargparser_argument;


typedef struct {
    int type;
    union {
        struct option* op;
        cargparser* parser;
    };
}cargparser_type;


typedef struct{
    void (*handler)(cargparser_args *);
    void *ptr;
}_args;

/**
 * 添加一条关键字信息
 * */
extern int cargparser_add(struct cargparser_array *array, void *keyword);

/**
 * 获取参数与终端传过来的参数
 * 如果有返回 1 否则 0
 * */
extern int cargparser_get(cargparser *parser, cargparser_type *dest, char *keyword);

/**
 * 开始解析, 该解析包含子命令解析
 * */
extern int cargparser_parse(cargparser *parser, cargparser_argument* argument, cargparser_args* args);

/**
 * 释放内存
 * */
extern int cargparser_free(_args *args);

/**
 * array
 * */
extern struct cargparser_array* make_cargparser_array();

/**
 * usage
 * */
extern void cargparser_usage(cargparser *parser, cargparser* parent);

extern struct cargparser_array* make_cargparser_array(){
    struct cargparser_array *array = calloc(1, sizeof(struct cargparser_array));
    array->capacity = 1;
    array->_el = malloc(sizeof(void *));
    return array;
}

extern int cargparser_get(cargparser *parser, cargparser_type *dest, char *keyword){
    cargparser *cp = 0;
    for (int i = 0; i < parser->sub_size; ++i) {
        cp = parser->sub + i;
        if (strcmp(cp->cmd, keyword) == 0){
            dest->type = 1;
            dest->parser = cp;
            return 1;
        }
    }
    struct option *op = 0;
    for (int i = 0; i < parser->op_size; ++i) {
        op = parser->options + i;
        if (strcmp(op->keyword[0], keyword) == 0 ||
            strcmp(op->keyword[1], keyword) == 0){
            dest->type = 0;
            dest->op = op;
            return 1;
        }
    }
    return 0;
}

void parse(cargparser *parser, cargparser_args *arg, int argc, char** argv){
    cargparser_argument ps = {argc, 0, argv};
    _args *_as = malloc(sizeof(_args));
    _as->ptr = make_cargparser_array();
    arg->_ptr = _as;
    if (!cargparser_parse(parser, &ps, arg)){
        if (ps.pos >= argc){
            printf("arguments error\n");
        }else{
            printf("no argument: %s\n", argv[ps.pos]);
            usage(parser);
        }
        cargparser_free(arg->_ptr);
        arg->_ptr = 0;
    }
}


extern int cargparser_parse(cargparser *parser, cargparser_argument* argument, cargparser_args* args){
    _args *as = args->_ptr;
    cargparser_type type;
    char *arg = 0;
    while (argument->pos < argument->argc){
        arg = argument->argv[argument->pos];
        if(!cargparser_get(parser, &type, arg)) return 0;
        if (type.type){
            /**
             * 说明是一个子命令, 进行递归
             * */
            argument->pos++;
            as->handler = type.parser->handler;
            cargparser_parse(type.parser, argument, args);
        } else {
            /**
             * 选项参数
             * */
            argument->pos++;
            if (type.op->require && argument->pos >= argument->argc) return 0;
            type.op->ptr = argument->argv[argument->pos];
            cargparser_add(as->ptr, type.op);
            argument->pos++;
        }
    }
    return 1;
}

extern void usage(cargparser* parser){
    printf("usage: %s", parser->cmd);
    cargparser_usage(parser, 0);
    printf("\n");
}


extern void cargparser_usage(cargparser* parser, cargparser* parent){
    struct option *op;
    if (parent)
        printf("[%s", parser->cmd);
    for (int i = 0; i < parser->op_size; ++i) {
        op = parser->options + i;
        if (*op->keyword)
            printf("[%s", *op->keyword);
        if (*(op->keyword + 1))
            printf("|%s", *(op->keyword + 1));
        printf("]");
    }
    if (parent)
        printf("]");
    for (int i = 0; i < parser->sub_size; ++i) {
        cargparser_usage(parser->sub + i, parser);
    }
    if (parser->sub_size > 0)
        printf("\nkeyword:");
    cargparser *ps;
    for (int i = 0; i < parser->sub_size; ++i) {
        ps = parser->sub + i;
        printf("\n%s: %s", ps->cmd, ps->description);
    }
}


extern int cargparser_add(struct cargparser_array* array, void *keyword){
    if (! array && ! keyword) return 0;
    if (array->size >= array->capacity) {
        array->_el = realloc(array->_el, sizeof(void *) * array->size + sizeof(void *));
        array->capacity++;
    }
    array->_el[array->size++] = keyword;
    return 1;
}

extern void* get_argument(cargparser_args *args, char *key, void* defaults){
    struct cargparser_array *arr = ((_args *)args->_ptr)->ptr;
    struct option *op;
    for (int i = 0; i < arr->size; ++i) {
        op = arr->_el[i];
        if (strcmp(op->name, key) == 0)
            return op->ptr ? op->ptr : op->defaults;
    }
    return defaults;
}


extern void cargparser_call(cargparser_args *arg){
    _args *args = arg->_ptr;
    if (!args || !args->handler) return;
    args->handler(arg);
}


extern int cargparser_free(_args* args){
    if (!args) return 0;
    args->handler = 0;
    struct cargparser_array *arr = args->ptr;
    free(arr->_el);
    free(arr);
    return 1;
}