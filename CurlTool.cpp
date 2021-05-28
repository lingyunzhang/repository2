#include <iostream>
#include "PieceData.h"
#include "CommonDownLoadTool.h"
#include <mutex>
#include <curl/curl.h>
using namespace std;
class CurlTool : public CommonDownLoadTool
{
private:
    CURL *curl;//libcurl初始化指针
    mutex mut;//锁

    void init()
    {
        curl = curl_easy_init();
        PieceData * _pieceData = &getPieceData();
        // _pieceData->setCurl(curl);
        char range[64] = {0};
        snprintf(range, sizeof(range), "%ld-%ld", _pieceData->getStartPos(), _pieceData->getEndPos());
        // libcurl 下载插件
        curl_easy_setopt(curl, CURLOPT_URL, getUrl().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)_pieceData);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5L);
        curl_easy_setopt(curl, CURLOPT_RANGE, range);
    }
    //下载文件
    void downLoadFile(int* threadCount)
    {
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        mut.lock;
        *threadCount++;
        mut.unlock;
    }
    //将下载的内容写入目标文件
    size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
    {
        PieceData *node = (PieceData *)userdata;
        size_t written = 0;
        mut.lock();
        if (node->getStartPos() + size * nmemb <= node->getEndPos())
        {
            fseek(node->getFile(), node->getStartPos(), SEEK_SET);
            written = fwrite(ptr, size, nmemb, node->getFile());
            node->startPos += size * nmemb;
        }
        else
        {
            fseek(node->getFile(), node->getStartPos(), SEEK_SET);
            written = fwrite(ptr, 1, node-getEndPos() - node->getStartPos() + 1, node->getFile());
            node->setEndPos(node->getEndPos());
        }
        mut.unlock();
        return written;
    }
    //显示下载进度
    int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
    {
        int percent = 0;
        if (totalToDownload > 0)
        {
            percent = (int)(nowDownloaded / totalToDownload * 100);
        }

        if (percent % 20 == 0)
            printf("下载进度%0d%%\n", percent);
        return 0;
    }
    //获取文件大小
    long getDownloadFileLenth()
    {
        const char *url = getUrl();
        double downloadFileLenth = 0;
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_URL, url);
        curl_easy_setopt(handle, CURLOPT_HEADER, 1); //只需要header头
        curl_easy_setopt(handle, CURLOPT_NOBODY, 1); //不需要body
        if (curl_easy_perform(handle) == CURLE_OK)
        {
            curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
        }
        else
        {
            downloadFileLenth = -1;
        }
        return downloadFileLenth;
    }
};
