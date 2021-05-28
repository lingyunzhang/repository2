#include <iostream>
#include "PieceData.h"
#include <curl/curl.h>
using namespace std;
class CurlTool
{
private:
    PieceData readContent;

public:
    void setUrl(string _url);
    string getUrl();
    void setPieceData(PieceData _targetFile);
    PieceData getPieceData();
    /**
     * 初始化
     * */
    void init();
    //下载文件
    void downLoadFile(int* threadCount);
    //将下载的内容写入目标文件
    static size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata);
    //显示下载进度
    int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);
    long getDownloadFileLenth();
};