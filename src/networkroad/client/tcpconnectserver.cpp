
#include "tcpconnectserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class TcpConnectServerPrivate{
    public:
        int sockfd;
        int port;
        std::string address;

};


TcpConnectServer::TcpConnectServer()
    :d(new TcpConnectServerPrivate)
{

}


TcpConnectServer::~TcpConnectServer()
{
    if(d){
        delete d;
        d = NULL;
    }
}

void TcpConnectServer::error_exit(const std::string &errorInfo)
{
   perror(errorInfo.c_str());
}

void TcpConnectServer::InitEvn()
{
    d->port = 8000;
    d->address = "127.0.0.1";
}

void TcpConnectServer::setPort(int num)
{
    d->port = num;
}

void TcpConnectServer::setAddress(const std::string &str)
{
    //d->address = str;
    struct hostent *host = gethostbyname(str.c_str());
    if(host == NULL){
        d->address = str;
    } else {
        struct in_addr **addr_list = (struct in_addr **)host->h_addr_list;
        char ip[65];
        for (int i = 0; addr_list[i] != NULL; i++) {
            // inet_ntoa()将long类型的IP地址转化为圆点的字符串形式，作用与inet_addr()相反
            strcpy(ip, inet_ntoa(*addr_list[0]));
        }
        d->address = ip;
    }
    std::cout << "TcpConnectServer::setAddress ip==" << d->address << std::endl;
}

void TcpConnectServer::startWork()
{
    create_socket();
    connect_server();
}

bool TcpConnectServer::create_socket()
{
    d->sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(d->sockfd < 0)
    {
        error_exit("create error");
        return false;
    }

    return true;
}

int TcpConnectServer::connect_server()
{
    //连接服务器，设置服务器的地址(ip和端口)
    struct sockaddr_in svraddr;
    memset(&svraddr,0,sizeof(svraddr));
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = inet_addr(d->address.c_str());
    svraddr.sin_port = htons(d->port);

    int ret = connect(d->sockfd,(struct sockaddr *)&svraddr,sizeof(svraddr));
    if(ret < 0) {
        error_exit("connect error");
        return -1;
    } else {
        std::cout << "TcpConnectServer::connect_server succeed!!!" << std::endl;
        return d->sockfd;
    }

}

void TcpConnectServer::break_server()
{
    close(d->sockfd);
}

void TcpConnectServer::send_data(const std::string &data)
{
    int ret = send(d->sockfd,data.c_str(),data.size(),0);
    if(ret < 0 ){
        std::cout << "TcpConnectServer::send_data failed!!!!" << std::endl;
    }
}

void TcpConnectServer::receive_data()
{
}


