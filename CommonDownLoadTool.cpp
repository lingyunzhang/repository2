#include <iostream>
#include "PieceData.h"
#include <mutex>
using namespace std;
class CommonDownLoadTool
{
private:
    string url;
    PieceData pieceData; //每次写入文件的大小

public:
    void setUrl(string _url)
    {
        url = _url;
    }

    string getUrl()
    {
        return url;
    }

    void setPieceData(PieceData _targetFile)
    {
        pieceData = _targetFile;
    }

    PieceData getPieceData()
    {
        return pieceData;
    }

    virtual void init()
    {
    }

    virtual void downLoadFile(mutex *mut, int *threadCount)
    {
    }

    virtual size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
    {

    }
    virtual int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
    {

    }

    virtual long getDownloadFileLenth(const char *url)
    {

    }
};