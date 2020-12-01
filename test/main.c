#include "cargparser.h"
#include <stdio.h>

extern void server_cmd(cargparser_args* args);

struct option server_options[] = {
        {"port", {"-p","--port"},  0, 0, "8080"},
        {"host", {"-h","--host"},  0, 0, "3306"},
};


struct option client_options[] = {
        {"global",{"-c","--c"} , "",0,"12"},
};

// 子命令
cargparser sub_cmd[] ={
/**       命令             选项               选项数量          子命令     子命令数量         处理命令   */
        {"server", server_options, len(server_options), 0, 0, server_cmd},
        {"client", client_options, len(client_options), 0, 0, server_cmd},
        {"stop", client_options, len(client_options), 0, 0, server_cmd},
};

/**
 * test server|client|stop
 * */
cargparser parser = {"test", client_options, len(client_options), sub_cmd, len(sub_cmd)};

int main(int argv, char **args){
    cargparser_args arg;
    parse_main(&parser, &arg, argv, args);
    cargparser_call(&arg);
}

extern void server_cmd(cargparser_args* args){
    char * port = get_argument(args, "port", "9999");
    char *host = get_argument(args, "host", "255.255.255.255");
    char *global = get_argument(args, "global","");
    printf("port: %s\n", port);
    printf("host: %s\n", host);
    printf("global: %s\n", global);
}
