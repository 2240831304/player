
#include "httprequest.h"
#include "tcpconnectserver.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>


class HttpRequestPrivate {
    public:
        TcpConnectServer *tcpConnectRequest;
        int clientSocketFd;
        std::string sendData;
        std::string receiveData;
        std::string finishedFlagStr;
        std::string postData;
        std::string responseHeader;
        std::string responseBody;
        std::string hostAddress;
        FILE *file;
        int totalDataSize;

};

HttpRequest::HttpRequest()
    :d (new HttpRequestPrivate)
{
    d->tcpConnectRequest = new TcpConnectServer;

}


HttpRequest::~HttpRequest()
{
    if(d->tcpConnectRequest){
        delete d->tcpConnectRequest;
        d->tcpConnectRequest = NULL;
    }

    if(d){
        delete d;
        d = NULL;
    }
}

void HttpRequest::InitEvn()
{
    d->clientSocketFd = -1;
    d->sendData.clear();
    d->receiveData.clear();
    d->finishedFlagStr.clear();
    d->postData.clear();
    d->responseBody.clear();
    d->responseHeader.clear();
    d->hostAddress.clear();
    d->totalDataSize = 0;
}

void HttpRequest::setFinishedFlagStr(const std::string &str)
{
    d->finishedFlagStr = str;
}

void HttpRequest::setRequestAddress(const std::string &dress,Request_Type type,bool flag)
{
    switch(type){
        case REQUEST_POST :{
            d->sendData += "POST ";
            break;
        }
        default:
            d->sendData += "GET ";
            break;
    }

    std::string url = dress;
    int pos = url.find_first_of("//",0);
    if( pos >= 0){
        url.replace(pos,2,"");
    }

    pos = url.find_first_of("/",0);
    std::string hostStr = url.substr(0,pos);
    std::string ServerPath = url.substr(pos);
    std::cout << "HttpRequest::setRequestAddress host=" << hostStr
                    << " request server path=" << ServerPath << std::endl;

    pos = hostStr.find_first_of(":",0);
    std::string host = hostStr.substr(pos+1);
    d->hostAddress = host;
    std::cout << "HttpRequest::setRequestAddress host=" << host << std::endl;
    d->tcpConnectRequest->setAddress(host);

    if(flag) {
        d->sendData += ServerPath;
    } else {
        d->sendData += dress;
    }
    d->sendData += " HTTP/1.1\r\n";
    d->sendData += "Host:";
    d->sendData += host;
    d->sendData += "\r\n";

}

void HttpRequest::setPort(int port)
{
    d->tcpConnectRequest->setPort(port);
}

void HttpRequest::setRequestHeader(const std::string &key,const std::string &value)
{
    std::string temp = key + ":" + value + "\r\n";
    d->sendData += temp;
}

void HttpRequest::setGetRequestInfo()
{
	d->sendData += "Accept: */*\r\n";
	d->sendData += "Accept-Language: en-US,zh-CN;q=0.5\r\n";
	d->sendData += "User-Agent: Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.1;"\
	            "Trident/4.0; qdesk 2.4.1265.203; SLCC2; .NET CLR 2.0.50727;"\
	            ".NET CLR 3.5.30729; .NET CLR 3.0.30729; InfoPath.3)\r\n";
	d->sendData += "Accept-Encoding: gzip, deflate,sdch\r\n";
	d->sendData += "Connection: Keep-Alive\r\n";
	//d->sendData += "Host:39.96.126.153\r\n"; //服务器地址
	d->sendData += "Cookie: JSESSIONID=5386A9443729D7EB0B61E38A9C7CF52F\r\n";
	d->sendData += "\r\n";
}

void HttpRequest::sendGetRequest()
{
    bool flag = d->tcpConnectRequest->create_socket();
    if(!flag) {
        std::cout << "HttpRequest::sendGetRequest create socket is failed!!!" << std::endl;
        return;
    }

    d->clientSocketFd = d->tcpConnectRequest->connect_server();
    if(d->clientSocketFd < 0) {
        std::cout << "HttpRequest::sendGetRequest connect server is failed!!" << std::endl;
        return;
    }

    setGetRequestInfo();
    std::cout << "\nHttpRequest::sendGetRequest send data===\n" << d->sendData << std::endl <<
            "d->clientSocketFd======" << d->clientSocketFd << std::endl;

    int ret = send(d->clientSocketFd, d->sendData.c_str(), d->sendData.size(), 0);
	if(ret < 0 )
	{
	    std::cout << "HttpRequest::sendGetRequest send data is failed!!!!!!!" << std::endl;
		close(d->clientSocketFd);
		return;
	}

	blockIOreceiveData();
	//SignalDriveIOReceiveData();
}

void HttpRequest::blockIOreceiveData()
{
    char recv_buf[1024];
    bool finished = false;

    struct timeval timeout = {5,0};//5s
    int ret = setsockopt(d->clientSocketFd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));

	while(1) {
        memset(recv_buf,0,1024);
	    int num = recv(d->clientSocketFd, recv_buf, sizeof(recv_buf)-1, 0);
	    if( finished && (num <= 0))
	        break;
	    d->receiveData += recv_buf;
	    if(d->receiveData.find(d->finishedFlagStr) != std::string::npos)
	        finished = true;
	}
	close(d->clientSocketFd);

	std::cout << "HttpRequest::receiveData ==\n" << d->receiveData << std::endl
	        << "recevie data size===" << d->receiveData.size() << std::endl;
	spliteResponseData();
}

void HttpRequest::SignalDriveIOReceiveData()
{
    const int on = 1;
    signal(SIGIO,do_sigIO);
    int flag = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(d->clientSocketFd, F_SETFL, flag | O_NONBLOCK | O_ASYNC);
    fcntl(d->clientSocketFd,F_SETOWN,getpid());  //    设置套接口的属主
    ioctl(d->clientSocketFd,FIOASYNC,&on);         //设置信号驱动i/o
    ioctl(d->clientSocketFd,FIONBIO,&on);         // 设置非阻塞i/o
}

void HttpRequest::do_sigIO(int sig)
{
    std::cout << "HttpRequest::do_sigIO the sig== " << sig << std::endl;
}

void HttpRequest::spliteResponseData()
{
    int pos = d->receiveData.find("\r\n\r\n");
    d->responseHeader = d->receiveData.substr(0,pos);
    d->responseBody = d->receiveData.substr(pos+4);
    std::cout << "HttpRequest::spliteResponseData header= " << d->responseHeader
                   << std::endl << "body@@@@@@@@@ = " << d->responseBody
                   << "  @@body size===" << d->responseBody.size() << std::endl;
}


void HttpRequest::sendPostRequest()
{
    bool flag = d->tcpConnectRequest->create_socket();
    if(!flag) {
        std::cout << "HttpRequest::sendPostRequest create socket is failed!!!" << std::endl;
        return;
    }

    d->clientSocketFd = d->tcpConnectRequest->connect_server();
    if(d->clientSocketFd < 0) {
        std::cout << "HttpRequest::sendPostRequest connect server is failed!!" << std::endl;
        return;
    }

    setPostRequestInfo();
    std::cout << "HttpRequest::sendPostRequest send data===\n" << d->sendData << std::endl;

    int ret = send(d->clientSocketFd, d->sendData.c_str(), d->sendData.size(), 0);
	if(ret < 0 )
	{
	    std::cout << "HttpRequest::sendPostRequest send data is failed!!!!!!!" << std::endl;
		close(d->clientSocketFd);
		return;
	}
	blockIOreceiveData();
}


void HttpRequest::setPostRequestInfo()
{
    d->sendData += "Accept: */*\r\n";
	d->sendData += "Referer: */*\r\n";
	d->sendData += "Accept-Language: en-US,zh-CN;q=0.5\r\n";
	d->sendData += "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	d->sendData += "Content-type:application/x-www-form-urlencoded\r\n";
	d->sendData += "Accept-Encoding: gzip, deflate\r\n";
	d->sendData += "Connection: Keep-Alive\r\n";
	d->sendData += "Cache-Control: no-cache\r\n";
    d->sendData += "Content-Length:";
    std::stringstream ss;
    ss << d->postData.size();
    d->sendData += ss.str();
    d->sendData += "\r\n\r\n";
    d->sendData += d->postData;
	//d->sendData += "\r\n";
}

void HttpRequest::setPostData(const std::string &data)
{
    d->postData = data;
//test data
#if 1
    d->postData = "";
    d->postData = "<Request><PostRecentListReq><book><bookName>aff.txt</bookName>" \
            "<bookId>20190829</bookId><bookFormat>.epub</bookFormat><recentPostion><htmlPath>/swhuwu/swsws</htmlPath>" \
            "<paragraphId>56</paragraphId><elementId>89</elementId><charId>36</charId>" \
            "<pageId>56</pageId><content>jiwsjwswii</content></recentPostion></book></PostRecentListReq></Request>";
#endif
}

void HttpRequest::downFile()
{
    bool flag = d->tcpConnectRequest->create_socket();
    if(!flag) {
        std::cout << "HttpRequest::downFile create socket is failed!!!" << std::endl;
        return;
    }

    std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
    d->clientSocketFd = d->tcpConnectRequest->connect_server();
    if(d->clientSocketFd < 0) {
        std::cout << "HttpRequest::downFile connect server is failed!!" << std::endl;
        return;
    }

    setDownFileRequestInfo();
    std::cout << "HttpRequest::downFile send data===\n" << d->sendData << std::endl;

    int ret = send(d->clientSocketFd, d->sendData.c_str(), d->sendData.size(), 0);
	if(ret < 0 )
	{
	    std::cout << "HttpRequest::downFile send data is failed!!!!!!!" << std::endl;
		close(d->clientSocketFd);
		return;
	}

	receiveFileData();
}

void HttpRequest::setDownFileRequestInfo()
{
    d->sendData += "Accept: */*\r\n";
	d->sendData += "Accept-Language: en-US,zh-CN;q=0.5\r\n";
	d->sendData += "User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\n";
	d->sendData += "Accept-Encoding: gzip, deflate,sdch\r\n";
	d->sendData += "Connection: Keep-Alive\r\n";
	d->sendData += "Range: bytes=0 -\r\n";
	d->sendData += "Cookie: JSESSIONID=5386A9443729D7EB0B61E38A9C7CF52F\r\n";
	d->sendData += "\r\n";
}

void HttpRequest::receiveFileData()
{
    char recv_buf[4096];
    int receiveDataSum = 0;
    int num = 0;
    std::string tempReceiveStr;
    d->file = fopen("test.pdf","w+");

    for(; ;) {
        memset(recv_buf,0,4096);
	    num = recv(d->clientSocketFd, recv_buf, 4096, 0);
	    receiveDataSum += num;

        tempReceiveStr += recv_buf;
        int pos = tempReceiveStr.find("\r\n\r\n");
        if(pos != std::string::npos) {
            num = parseReponseHeader(tempReceiveStr.c_str(),receiveDataSum);
            receiveDataSum = 0;
            receiveDataSum += num;
            break;
        }
	}

	for(; ;) {
        memset(recv_buf,0,4096);
	    num = recv(d->clientSocketFd, recv_buf, 4096, 0);

        receiveDataSum += num;
        fwrite(recv_buf,1,strlen(recv_buf),d->file);
        std::cout << "###############file totle=== " << d->totalDataSize << "receive sum==" << receiveDataSum << std::endl;

        if(receiveDataSum >= d->totalDataSize)
            break;
	}

	fclose(d->file);
	close(d->clientSocketFd);
}

int HttpRequest::parseReponseHeader(const char *headerData,int receiveDataNum)
{
    std::string data = headerData;
    std::cout << data;
    int pos = data.find("\r\n\r\n");
    d->responseHeader = data.substr(0,pos);
    int size = receiveDataNum - pos -4;
    fwrite(headerData+pos+4,1,size,d->file);

    std::string str = d->responseHeader;
    for(;;) {
        pos = str.find_first_of("\r\n",0);
        if(pos > 0) {
            std::string keyValue = str.substr(0,pos);
            if(keyValue.find("Content-Length") != std::string::npos){
                int keyIndex = keyValue.find(":");
                d->totalDataSize = atoi(keyValue.substr(keyIndex+1).c_str());
            }
            str = str.substr(pos+2);
        } else {
            break;
        }
    }

    return size;
}



void HttpRequest::sendTestGetRequest()
{
    d->tcpConnectRequest->create_socket();
    d->clientSocketFd = d->tcpConnectRequest->connect_server();

    char str1[1024] = "";

#if 1
    //服务端接收数据处理文件地址,并带参数
    std::string sendContext = "GET /readrecord/readrecordapi.aspx/ HTTP/1.1\r\n";
    sendContext += "action:getRecentList\r\n";
	sendContext += "serial:OF6GE30123456\r\n";
	sendContext += "Accept: */*\r\n";
	sendContext += "Accept-Language: en-US,zh-CN;q=0.5\\r\\n";
	sendContext += "User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0; qdesk 2.4.1265.203; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; InfoPath.3)\r\n";
	sendContext += "Accept-Encoding: gzip, deflate\r\n";
	//sendContext += "Host: 127.0.0.1 "; //服务器地址
	sendContext += "Connection: Keep-Alive\r\n";
	sendContext += "Cookie: JSESSIONID=5386A9443729D7EB0B61E38A9C7CF52F\r\n";
	sendContext += "\r\n";

    strcpy(str1,sendContext.c_str());
#else
    sprintf(str1, "%s\r\n","GET /readrecord/readrecordapi.aspx/ HTTP/1.1");
	sprintf(str1, "%s%s\r\n",str1,"Accept: */*");
	sprintf(str1, "%s%s\r\n",str1,"Accept-Language: en-US,zh-CN;q=0.5");
	sprintf(str1, "%s%s\r\n",str1,"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0; qdesk 2.4.1265.203; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; InfoPath.3)");
	sprintf(str1, "%s%s\r\n",str1,"Accept-Encoding: gzip, deflate");
    //sprintf(str1, "%s%s\r\n",str1,"Host: 127.0.0.1"); //服务器地址
	sprintf(str1, "%s%s\r\n",str1,"Connection: Keep-Alive");
	sprintf(str1, "%s%s\r\n",str1,"action:getRecentList");
	sprintf(str1, "%s%s\r\n",str1,"serial:OF6GE30123456");
	sprintf(str1,"%s%s\r\n",str1,"Cookie: JSESSIONID=5386A9443729D7EB0B61E38A9C7CF52F");
	sprintf(str1, "%s\r\n",str1);

#endif
    std::cout << sendContext << " size =" << sendContext.size() << std::endl;

    int ret = send(d->clientSocketFd, sendContext.c_str(), sendContext.size(), 0);   // 向服务器发送信息
	if(ret < 0 )
	{
	    std::cout << "HttpRequest::sendRequst is failed!!!!!!!" << std::endl;
		close(d->clientSocketFd);
	}

    d->finishedFlagStr = "</Response>";
	blockIOreceiveData();
}

void HttpRequest::sendTestPostRequest()
{
}