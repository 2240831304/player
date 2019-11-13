
#ifndef WAITCONNECT_H
#define WAITCONNECT_H

#include <iostream>

class WaitConnectPrivate;
class WaitConnect{
    public:
        WaitConnect();
        ~WaitConnect();

        void startUpServer();

    private:
        void InitEvn();
        void error_exit(const std::string &);
        void create_socket();
        void bind_listen();
        void obtain_client();

    private:
        static void *thread_fun(void *);

    private:
        WaitConnectPrivate *d;

};


#endif
