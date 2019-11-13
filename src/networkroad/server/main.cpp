
#include <iostream>
#include "waitconnect.h"

int main(int argc, char* argv[])
{
    std::cout << "this is server,start backstage work!!!" << std::endl;
    WaitConnect *server = new WaitConnect;
    server->startUpServer();
    return 0;
}
