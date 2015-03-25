#ifndef POOL_MANAGER_H
#define POOL_MANAGR_H

#include "MemPool.h"

#define MEM_POOL_TEST

#ifdef MEM_POOL_TEST

class PoolManager
{
public:

	static void* Create(MemPool* pool,size_t MemSize)
	{
		void* pObj=pool->GetBlock(MemSize);
		return pObj;
	}
	static void Delete(MemPool* pool, void* pObj)
	{
		pool->FreeBlock(pObj);
	}
};

#else

template<typename T>
class PoolManager
{
public:
	typedef T ObjType;

	static ObjType* Create(MemPool* pool)
	{
		void* pObj=pool->GetBlock(sizeof(T));
		return static_cast<ObjType*>(pObj);
	}
	static void Delete(MemPool* pool, ObjType* pObj)
	{
		pObj->~ObjType();
		pool->FreeBlock(pObj);
	}
};
#endif

#endif