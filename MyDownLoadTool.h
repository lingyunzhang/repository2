#include <iostream>
#include "PieceData.h"
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
    void downLoadFile(int threadNums, string url, string path, string fileName);