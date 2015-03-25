#ifndef MEM_LIST_H
#define MEM_LIST_H

#include "MemChunk.h"
#include <vector>

struct HeapHeader{
	size_t size;
};
typedef struct HeapBlock{
	HeapHeader hh;
	char buff;
}HeapBlock,*pHeapBlock;

class MemPool{
public:	
	typedef enum{MAX_SIZE=1024,MIN_SIZE=sizeof(MemChunk*)};
	MemPool(){
		for(size_t index=MIN_SIZE;index<=MAX_SIZE;index*=2){
			MemChunk *mChunk=new MemChunk(index,count);
			if(mChunk)
				_MemPool.push_back(mChunk);
		}
	}
	~MemPool(){
		size_t i=0;
		while(i<_MemPool.size()){
			_MemPool[i]->~MemChunk();
			++i;
		}
//		std::cout<<"MemPool has been free"<<std::endl;
	}
	void *GetBlock(size_t size){
		if(size>1024)
			return MallocHeap(size);
		else{
			size_t i=0;
			for(;i<_MemPool.size();++i){
				if(_MemPool[i]->getSize()>=size&&_MemPool[i]->hasFree())
					break;
			}
			return _MemPool[i]->GetChunk();
		}
	}
	void FreeBlock(void *pChunk){	
		if(!(FreeHeap(pChunk))){
			pMemBlock pBlock=(pMemBlock)((char *)pChunk-sizeof(BlockHeader));
			int size=pBlock->header.size;
			size_t i=0;
			for(;i<_MemPool.size();++i){
				if(_MemPool[i]->getSize()==size)
					break;
			}
			_MemPool[i]->FreeChunk(pBlock);		
		}
	}
private:
	void *MallocHeap(size_t size){
		pHeapBlock hb=(pHeapBlock)::malloc(sizeof(HeapHeader)+size);
		hb->hh.size=size;
		return &hb->buff;
	}
	bool FreeHeap(void *pChunk){
		pHeapBlock pBlock=(pHeapBlock)((char *)pChunk-sizeof(HeapHeader));
		if(pBlock->hh.size>1024){
			free(pBlock);
			pBlock=NULL;
			return true;
		}
		else
			return false;
	}

private:
	static const int count = 1000;
	std::vector<MemChunk *>	_MemPool;
};

#endif