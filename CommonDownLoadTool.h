#include <iostream>
#include "PieceData.h"
#include <mutex>
using namespace std;
class CommonDownLoadTool
{
private:
    string url;//下载路径
    PieceData pieceData; //每次写入文件的大小

public:
    void setUrl(string _url);
    string getUrl();
    void setPieceData(PieceData _targetFile);
    PieceData getPieceData();

    //初始化准备
    virtual void init();
    //下载文件，参数：线程计数
    virtual void downLoadFile(int *threadCount);
    //写入文件
    virtual size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata);
    //显示下载进度
    virtual int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);
    //获取要下载文件的大小
    virtual long getDownloadFileLenth();

};