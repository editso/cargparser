#include <stdio.h>
#include "cargparser.h"


/**解析完成后的回调*/
extern void server_cmd(cargparser_args* args);
extern void client_cmd(cargparser_args* args);

struct option options[] = {
/**      选项参数名             命令            描述信息      是否必须       默认值    */
        {"port",        {"-p", "--port"},       0,           0,        "8080"},
        {"host",        {"-h", "--host"},       0,           0,        "3306"}
};

struct option server_options[] = {
        {"server",      {"-s","--server"} ,    0,            0,          0}
};

struct option client_options[] = {
        {"client",      {"-c","--client"} ,    0,            0,          0}
};


// 子命令
cargparser sub_cmd[] ={
/**       命令             选项          选项数量               子命令    子命令数量      处理命令        描述信息*/
        {"server", server_options, len(server_options),         0,      0,       server_cmd, "创建一个服务端"},
        {"client", client_options, len(client_options),         0,      0,       client_cmd, "创建一个客户端"},
};

/**
 * test server|client|stop
 * */
cargparser parser = {"test", options, len(options), sub_cmd, len(sub_cmd), 0};


int main(int argv, char **args){
    cargparser_args arg;
    parse_main(&parser, &arg, argv, args);
    cargparser_call(&arg);
}


extern void server_cmd(cargparser_args* args){
    char *port = get_argument(args, "port", "9999");
    char *host = get_argument(args, "host", "255.255.255.255");
    char *server = get_argument(args, "server","");
    printf("port: %s\n", port);
    printf("host: %s\n", host);
    printf("server: %s\n", server);
}


extern void client_cmd(cargparser_args* args){
    char *port = get_argument(args, "port", "9999");
    char *host = get_argument(args, "host", "255.255.255.255");
    char *client = get_argument(args, "client","");
    printf("port: %s\n", port);
    printf("host: %s\n", host);
    printf("client: %s\n", client);
}

