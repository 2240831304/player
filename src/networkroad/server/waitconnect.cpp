
#include "waitconnect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

class WaitConnectPrivate{
    public:
        bool isServerWorking;
        int sockfd;
};


WaitConnect::WaitConnect()
    : d(new WaitConnectPrivate)
{
    InitEvn();
}

WaitConnect::~WaitConnect()
{
    if(d){
        delete d;
        d = NULL;
    }

}

void WaitConnect::error_exit(const std::string &errorInfo)
{
    perror(errorInfo.c_str());
}

void WaitConnect::InitEvn()
{
    d->isServerWorking = true;
    d->sockfd = -1;
}

void WaitConnect::startUpServer()
{
    create_socket();
    bind_listen();
    obtain_client();
}

void WaitConnect::create_socket()
{
    d->sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(d->sockfd < 0)
    {
        error_exit("create error");
    }
}

void WaitConnect::bind_listen()
{
    struct sockaddr_in svraddr;
    memset(&svraddr,0,sizeof(svraddr));
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    svraddr.sin_port = htons(8080);

    int ret = bind(d->sockfd,(struct sockaddr *)&svraddr,sizeof(svraddr));
    if(ret < 0)
    {
        error_exit("bind error");
    }

    //设置监听参数back login,半连接数的最大
    ret=listen(d->sockfd,10);
    if(ret < 0)
    {
        error_exit("listen error");
    }
}

void WaitConnect::obtain_client()
{
    while (d->isServerWorking) {
        int acceptClientFd = accept(d->sockfd,(struct sockaddr*)NULL,NULL);
        if(acceptClientFd < 0){
            error_exit("WaitConnect::obtain_client accept client connect faile!!!");
        } else {
            pthread_t threadPt;
            int ret =  pthread_create(&threadPt, NULL,thread_fun, &acceptClientFd);
            if(ret != 0) {
                std::cout << "WaitConnect::obtain_client() create thread failed!!!" << std::endl;
            }
        }
    }

}

void *WaitConnect::thread_fun(void *arg)
{
    int *clientFd = (int *)arg;
    int clientSocketFd = *clientFd;
    for(int i = 0;i < 3;i++) {
        std::cout << "This is a pthread." << clientSocketFd << std::endl;
    }
    return 0;
}
