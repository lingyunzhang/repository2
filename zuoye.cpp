#include <iostream>
#include <thread>
#include <mutex>
#include <curl/curl.h>
using namespace std;

struct tNode
{
	FILE *fp;     //下载文件指针
	long startPos;//读取文件开始位置
	long endPos; //读取文件结束位置
	void *curl;
	pthread_t tid; //线程id
};

volatile int threadCount = 0; //线程计数器
mutex mut;//锁

/* 获取要下载的远程文件的大小 */
long getDownloadFileLenth (const char *url)
{
	double downloadFileLenth = 0;
	CURL *handle = curl_easy_init ();
	curl_easy_setopt (handle, CURLOPT_URL, url);
	curl_easy_setopt (handle, CURLOPT_HEADER, 1);	//只需要header头
	curl_easy_setopt (handle, CURLOPT_NOBODY, 1);	//不需要body
	if (curl_easy_perform (handle) == CURLE_OK)
	{
		curl_easy_getinfo (handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
	}
	else
	{
		downloadFileLenth = -1;
	}
	return downloadFileLenth;
}
//size_t function( void *ptr, size_t size, size_t nmemb, void *stream); 
//将下载的数据写入文件
static size_t writeFunc (void *ptr, size_t size, size_t nmemb, void *userdata)
{
	tNode *node = (tNode *) userdata;
	size_t written = 0;
	mut.lock();
	if (node->startPos + size * nmemb <= node->endPos)
	{
		fseek (node->fp, node->startPos, SEEK_SET);
		written = fwrite (ptr, size, nmemb, node->fp);
		node->startPos += size * nmemb;
	}
	else
	{
		fseek (node->fp, node->startPos, SEEK_SET);
		written = fwrite (ptr, 1, node->endPos - node->startPos + 1, node->fp);
		node->startPos = node->endPos;
	}
    mut.unlock();
	return written;
}
void downLoadFile();
void downLoadThread();
//获取下载进度
int progressFunc (void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);

const int threadNums; //线程数量
const string Url; //下载链接
const string Path; //文件保存路径
const string fileName; //文件保存名

int main(){

    long fileLength = getDownloadFileLenth (Url.c_str ());
    //判断文件是否存在
    if (fileLength <= 0)
	{
		printf ("get the file length error...");
		return false;
	}
    //创建文件保存
    const string outFileName = Path + fileName;
	FILE *fp = fopen (outFileName.c_str (), "wb");
	if (!fp)
	{
		return false;
	}
    for (int i = 0; i <= threadNum; i++)
	{
        //计算每次下载大小，存入pNode
        tNode *pNode = new tNode ();
		if (i < threadNum)
		{
			pNode->startPos = i * partSize;
			pNode->endPos = (i + 1) * partSize - 1;
		}
		else
		{
			if (fileLength % threadNum != 0)
			{
				pNode->startPos = i * partSize;
				pNode->endPos = fileLength - 1;
			}
			else
				break;
		}
        //开始下载文件
        downLoadFile(fp,pNode);
    }

    //等待所有线程执行完毕
    while (threadCount > 0)
	{
		usleep (1000000L);
	}
 
 //关闭文件流
	fclose (fp);

    return 0;
}
void downLoadFile(FILE *fp,tNode *pNode){
    CURL *curl = curl_easy_init ();
 
		pNode->curl = curl;
		pNode->fp = fp;
 
		char range[64] = { 0 };
		snprintf (range, sizeof (range), "%ld-%ld", pNode->startPos, pNode->endPos);
 
		// libcurl 下载插件
		curl_easy_setopt (curl, CURLOPT_URL, Url.c_str ());
		curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, writeFunc);
		curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *) pNode);
		curl_easy_setopt (curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt (curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
		curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt (curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
		curl_easy_setopt (curl, CURLOPT_LOW_SPEED_TIME, 5L);
		curl_easy_setopt (curl, CURLOPT_RANGE, range);
 
        
		mut.lock;
		threadCount++;
		mut.unlock;
        //启用线程，下载文件
        thread t = thread(downLoadThread,pNode);
        t.join();
}

//显示下载进度
int progressFunc (void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	int percent = 0;
	if (totalToDownload > 0)
	{
		percent = (int) (nowDownloaded / totalToDownload * 100);
	}
 
    if(percent % 20 == 0)
	    printf ("下载进度%0d%%\n", percent);
	return 0;
}

/*
*正式下载线程
*/
void downLoadThread(tNode pNode){
 
	int res = curl_easy_perform (pNode->curl);
 
	if (res != 0)
	{
 
	}
 
	curl_easy_cleanup (pNode->curl);
 
	mut.lock;
	threadCount--;
	printf ("thred %ld exit\n", pNode->tid);
	mut.unlock;
	
}

