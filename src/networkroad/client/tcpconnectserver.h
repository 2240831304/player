
#ifndef TCPCONNECTSERVER_H
#define TCPCONNECTSERVER_H

#include <iostream>

class TcpConnectServerPrivate;
class TcpConnectServer {
    public:
        TcpConnectServer();
        ~TcpConnectServer();

        void startWork();
        void break_server();
        void setPort(int );
        void setAddress(const std::string &);
        bool create_socket();
        int connect_server();

    private:
        void InitEvn();
        void send_data(const std::string &);
        void receive_data();
        void error_exit(const std::string &);


    private:
        TcpConnectServerPrivate *d;

};


#endif
