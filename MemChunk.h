#ifndef MEM_CHUNK_H
#define MEM_CHUNK_H

#include <iostream>
#include <windows.h>
typedef CRITICAL_SECTION MUTEXTYPE;
#define INITMUTEX(hMutex) InitializeCriticalSection(&hMutex)
#define DELMUTEX(hMutex) DeleteCriticalSection(&hMutex)
#define LOCK(hMutex) EnterCriticalSection(&hMutex)
#define UNLOCK(hMutex) LeaveCriticalSection(&hMutex)

struct MemBlock;
struct BlockHeader{	
	MemBlock *next;
	int size;
};

typedef struct MemBlock{
	BlockHeader header;
	char pBuff;
}MemBlock,*pMemBlock;


class MemChunk{
public:
	typedef  MemChunk * pMemChunk;
	MemChunk(size_t size,int count):_size(size),_count(count){
		INITMUTEX(_hMutex);
		pFreeList=CreatBlock();
		pFreeList->header.size=_size;
		pFreeList->header.next=NULL;
		pMemBlock pBlock=pFreeList;
		for(int i=0;i<count-1;++i){
			pBlock=CreatBlock();
			if(pBlock){
				pBlock->header.next=pFreeList->header.next;
				pFreeList->header.next=pBlock;
			}
		}
	}
	~MemChunk(){
		size_t tmpCnt=0;
		pMemBlock pBlock;
		while(pFreeList!=NULL){
			pBlock=pFreeList;
			pFreeList=pFreeList->header.next;
			FreeBlock(pBlock);
			++tmpCnt;
		}
		DELMUTEX(_hMutex);
		if(tmpCnt!=_count)
			std::cout<<"there are still "<<_count-tmpCnt<<"blocks with size  "<<_size<<"	bytes need to free"<<std::endl;
//		std::cout<<"MemChunk with size	"<<_size<<"	bytes has been free"<<std::endl;	
	}
	size_t getSize(){
		return _size;
	}
	bool hasFree(){
		return pFreeList!=NULL;
	}
	void *GetChunk(){
		LOCK(_hMutex);
		pMemBlock pBlock=pFreeList;
		pFreeList=pFreeList->header.next;
		UNLOCK(_hMutex);
		return &pBlock->pBuff;
	}
	void FreeChunk(pMemBlock pBlock){
		LOCK(_hMutex);
		pMemBlock pTmp=pFreeList;
		pFreeList=pBlock;
		pFreeList->header.next=pTmp;
		UNLOCK(_hMutex);
		return;
	}

private:
	pMemBlock CreatBlock(){
		pMemBlock pBlock=(pMemBlock)::malloc(sizeof(BlockHeader)+_size);
		pBlock->header.size=_size;
		pBlock->header.next=NULL;
		return pBlock;
	}
	void FreeBlock(pMemBlock pBlock){
		::free(pBlock);
		pBlock=NULL;
	}

private:
	size_t _size;	//chunk size
	size_t _count;	//chunk numbers
	MUTEXTYPE _hMutex;	//mutex;
	pMemBlock pFreeList;	//real block list
};


#endif