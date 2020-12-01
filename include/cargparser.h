#ifndef CARGPARSER_H
#define CARGPARSER_H

#define len(T) sizeof(T) / sizeof(T[0])

#define parse_main(parser, arg, argc, argv) parse(parser, arg, argc - 1, argv + 1)

/**
 * 选项
 * */
struct option{
    /**
     * 名字, 使用 get_argument 时会对应查找该名字
     * */
    char* name;
    /**
     * 选项, 可以有一个或两个
     * */
    char* keyword[2];
    /**
     * 参数描述信息
     * */
    char* description;
    /**
     * 是否必须
     * */
    int require;
    /**
     * 默认值
     * */
    void* defaults;

    /**
     * 存储与参数值相关数据
     * */
    void* ptr;
};

/**
 * 关键字参数
 * */
typedef struct cargparser_args_{
    void* _ptr;
}cargparser_args;

/**
 * 参数解析器
 * */
typedef struct cargparser_{
    /**
     * 命令,对于有子命令来说他将作为子命令来使用
     * */
    char* cmd;
    /**
     * 选项
     * */
    struct option *options;
    /**
     * 选项的数量, 如果有必须填入选项的数量
     * */
    unsigned int op_size;
    /**
     * 子命令
     * */
    struct cargparser_ *sub;
    /**
     * 子命令数量,如果有必须填入
     * */
    unsigned int sub_size;
    /**
     * 参数解析好后,应该做的回调
     * */
    void (*handler)(cargparser_args *);
}cargparser;


/**
 *
 * 初始化解析器
 * */
extern int init_parser(cargparser* parser);

/**
 * 开始解析
 * @param argv 参数个数
 * @param argc 参数列表
 * @param dest 解析后存放的位置
 * */
extern void parse(cargparser *parser, cargparser_args *arg, int argv, char** argc);

/**
 * 用法
 * */
extern void usage(cargparser* parser);

/**
 * 获取解析好后的参数
 * */
extern void* get_argument(cargparser_args *args, char *key, void* defaults);

/**
 * 参数解析器解析完成后, 如果解析器中 cargparser->handler 如果不为空, 将会调用
 * */
extern void cargparser_call(cargparser_args *);

#endif