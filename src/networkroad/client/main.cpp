
#include <iostream>
#include "tcpconnectserver.h"
#include "httprequest.h"
#include "mqttpubmessage.h"


int main(int argc, char* argv[])
{
    std::cout << "this is client,start connect server!!!" << std::endl;

//连接服务器测试
#if 0
    TcpConnectServer *tcpClient = new TcpConnectServer;
    tcpClient->startWork();
#endif

//get 测试
#if 0
    std::cout << " 111111111111" << std::endl;
    HttpRequest http;
    http.InitEvn();
    //http.setRequestAddress("http://127.0.0.1/readrecord/readrecordapi.aspx/",REQUEST_GET,true);
    http.setRequestAddress("http://127.0.0.1/readrecord/readrecordapi.aspx?bookName=围城&bookId=0",REQUEST_GET,true);
    http.setRequestHeader("action","getAnnotationList");
    http.setRequestHeader("serial","OF6GE30123456");
    http.setFinishedFlagStr("</Response>");
    http.setPort(8000);
    http.sendGetRequest();
#endif

//post 测试
#if 0
    HttpRequest httpPost;
    httpPost.InitEvn();
    httpPost.setRequestAddress("http://127.0.0.1/readrecord/readrecordapi.aspx/",REQUEST_POST,true);
    httpPost.setRequestHeader("action","postRecentList");
    httpPost.setRequestHeader("serial","OF6GE30123456");
    httpPost.setPort(8000);
    httpPost.setPostData("");
    httpPost.sendPostRequest();
#endif

// 测试获取哪些天阅读了书，
#if 0
    HttpRequest http;
    http.InitEvn();
    http.setRequestAddress("http://127.0.0.1/readrecord/readrecordapi.aspx/",REQUEST_GET,true);
    http.setRequestHeader("action","getReadDateBookList");
    http.setRequestHeader("serial","OS6BB11727B00997");
    http.setFinishedFlagStr("</Response>");
    http.setPort(8000);
    http.sendGetRequest();
#endif

// 测试今天，本周。。。阅读字数，时长
#if 1
    HttpRequest http;
    http.InitEvn();
    http.setRequestAddress("http://127.0.0.1/readrecord/readrecordapi.aspx/",REQUEST_GET,true);
    http.setRequestHeader("action","getBookDataList");
    http.setRequestHeader("serial","234234231234455");
    http.setFinishedFlagStr("</Response>");
    http.setPort(8000);
    http.sendGetRequest();
#endif

//获取其他网站
#if 0
    std::cout << " 2222222222222" << std::endl;
    HttpRequest http;
    http.InitEvn();
    http.setRequestAddress("http://blog.csdn.net/qq_26437925/article/details/50946025",REQUEST_GET,false);
    http.setPort(8000);
    http.sendGetRequest();
#endif

//下载文件测试
#if 0
    HttpRequest httpFile;
    httpFile.InitEvn();
    //http://geolite.maxmind.com/download/geoip/database/GeoLite2-Country.tar.gz
    //http://hukai.me/eBooks/AndroidTrainingCHS.pdf
    httpFile.setRequestAddress("http://hukai.me/eBooks/AndroidTrainingCHS.pdf"
                        ,REQUEST_GET,false);
    httpFile.setPort(80);
    httpFile.downFile();
#endif

#if 0
    MqttPubMessage mqtt;
    mqtt.testPubMes();
#endif


#if 0
    HttpRequest http;
    http.sendTestGetRequest();
#endif

    return 0;
}
