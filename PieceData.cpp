#include <iostream>
using namespace std;
class PieceData
{
private:
    FILE *fp;     //存储目标文件
	long startPos;//读取文件开始位置
	long endPos; //读取文件结束位置

public:
	void setFile(FILE *_fp){
        fp = _fp;
    }
	FILE* getFile(){
        return fp;
    }
	void setStartPos(long _startPos){
        startPos = _startPos;
    }
	long  getStartPos(){
        return startPos;
    }
	void setEndPos(long _endPos){
        startPos = _endPos;
    }
	long  getEndPos(){
        return endPos;
    }
};

