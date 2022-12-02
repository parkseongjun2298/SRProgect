#pragma once

#ifndef __ENGINE_FUNCTION_H__

BEGIN(ENGINE)

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
DWORD SafeRelease(T& ptr)
{
	DWORD dwRefCnt = 0;

	if (ptr)
	{
		dwRefCnt = ptr->Release();

		if (0 == dwRefCnt)
			ptr = nullptr;
	}

	return dwRefCnt;
}


class CDeleteMap
{
public:
	explicit CDeleteMap(void) {}
	~CDeleteMap(void) {}
public: // operator	
	template <typename T> void operator () (T& Pair)
	{
		if (NULL != Pair.second)
		{
			delete Pair.second;
			Pair.second = NULL;
		}
	}
};

class CTagFinder
{
public:
	explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
	~CTagFinder(void) {}
public:
	template <typename T> bool operator () (T& Pair)
	{
		int iResult = lstrcmp(m_pTag, Pair.first);

		if (0 == iResult)
			return true;
		return false;
	}
private:
	const TCHAR*				m_pTag;
};

END

#define __ENGINE_FUNCTION_H__
#endif