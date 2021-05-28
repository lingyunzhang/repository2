#include <iostream>
#include "PieceData.h"
#include "CurlTool.h"
#include <curl/curl.h>
#include <thread>
#include <mutex>
using namespace std;
class MyDownLoadTool
{
private:
    volatile int threadCount = 0; //线程计数器
    mutex mut;//锁
public:
    //参数依次为：线程数量，下载的url, 存储文件路径，文件名
    void downLoadFile(int threadNums, string url, string path, string fileName)
    {
        CurlTool curlTool;
        curlTool.setUrl(url);
        //获取目标文件大小
        long fileLength = curlTool.getDownloadFileLenth();
        if (fileLength <= 0)
        {
            printf("get the file length error...");
            return false;
        }
        //创建文件保存
        const string outFileName = path + fileName;
        FILE *fp = fopen(outFileName.c_str(), "wb");
        if (!fp)
        {
            return false;
        }
        //切分目标文件
        for (int i = 0; i <= threadNum; i++)
        {
            //计算每次下载大小，存入pNode
            PieceData *pNode = new PieceData();
            pNode->setFile(fp);
            if (i < threadNum)
            {
                pNode->setStartPos(i * partSize);
                pNode->setEndPos((i + 1) * partSize - 1);
            }
            else
            {
                if (fileLength % threadNum != 0)
                {
                    // pNode->startPos = i * partSize;
                    // pNode->endPos = fileLength - 1;
                    pNode->setStartPos(i * partSize);
                    pNode->setEndPos(fileLength - 1);
                }
                else
                    break;
            }
            
            curlTool.setPieceData(*pNode);
           //初始化下载工具
            curlTool.init();
            //线程数加一
            mut.lock;
		    threadCount++;
		    mut.unlock;
            //开启多线程
            int *threadNum = &threadCount;
            thread t = thread(curlTool.downLoadFile,threadNum);
            t.join();
        }
        //等待所有线程执行完毕
        while (threadCount > 0)
        {
            usleep(1000000L);
        }

        //关闭文件流
        fclose(fp);

        return 0;
    }
};