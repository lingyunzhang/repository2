#include <iostream>
using namespace std;
class PieceData
{
private:
    FILE *fp;     //存储目标文件
	long startPos;//读取文件开始位置
	long endPos; //读取文件结束位置

public:
	void setFile(FILE *_fp);
	FILE* getFile();
	void setStartPos(long _startPos);
	long  getStartPos();
	void setEndPos(long _endPos);
	long  getEndPos();
	
};

