
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <iostream>

enum Request_Type{
    REQUEST_GET,
    REQUEST_POST
};

class HttpRequestPrivate;
class HttpRequest {
    public:
        HttpRequest();
        ~HttpRequest();

        void InitEvn();
        void setFinishedFlagStr(const std::string &);
        void setRequestAddress(const std::string &,Request_Type,bool);
        void setPort(int );
        void setRequestHeader(const std::string &,const std::string &);

        void sendGetRequest();

        void sendPostRequest();
        void setPostData(const std::string &);

        void sendTestGetRequest();
        void sendTestPostRequest();

        //下载文件
        void downFile();

    private:
        void setGetRequestInfo();
        void setPostRequestInfo();
        void spliteResponseData();
        void blockIOreceiveData();
        void SignalDriveIOReceiveData();
        static void do_sigIO(int sig);
        void setDownFileRequestInfo();
        void receiveFileData();

        int parseReponseHeader(const char *headerData,int receiveDataNum);

    private:
        HttpRequestPrivate *d;

};


#endif