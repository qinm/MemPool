#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "PoolManager.h"
#include <windows.h>

using namespace std;


class CTimer
{
public:
	CTimer(){
		QueryPerformanceFrequency(&m_Frequency);
		Start();
	}
	void Start(){
		QueryPerformanceCounter(&m_StartCount);
	}
	double End(){
		LARGE_INTEGER CurrentCount;
		QueryPerformanceCounter(&CurrentCount);
		return double(CurrentCount.LowPart - m_StartCount.LowPart) / (double)m_Frequency.LowPart;
	}
	void ShowNow(){
		LARGE_INTEGER CurrentCount;
		QueryPerformanceCounter(&CurrentCount);
		cout<<"Timer Count is:"<<double(CurrentCount.LowPart - m_StartCount.LowPart) / (double)m_Frequency.LowPart<<endl;
	}
private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_StartCount;
};

int main(int argc,char **argv){
	int *p[1000];
	size_t size=1024;

	CTimer PoolTim;
	
	MemPool *mPool=new MemPool();
	PoolManager *pMger;
	for(int j=0;j<10;++j){
		for(int i=0;i<1000;++i)
			p[i]=(int *)pMger->Create(mPool,size);
		for(int i=0;i<1000;++i)
			pMger->Delete(mPool,p[i]);
	}
	
	delete mPool;
	cout<<"end pool alloc"<<endl;
	cout<<PoolTim.End()<<endl<<endl;

	CTimer Tim;
	for(int j=0;j<10;++j){
		for(int i=0;i<1000;++i)
			p[i]=(int *)::malloc(size);
		for(int i=0;i<1000;++i){
			::free(p[i]);
			p[i]=NULL;
		}
	}
	cout<<"end alloc"<<endl;
	cout<<Tim.End()<<endl;

	system("pause");
	return 0;
}