#pragma once
#ifndef __MYSTRCMP_H__

class CMyStrCmp
{
public:
	CMyStrCmp() {};
	CMyStrCmp(const TCHAR* pKey)
		:m_pString(pKey)
	{}
	~CMyStrCmp() {}; 
public:
	template<typename T>
	bool operator()(T& rPair)
	{
		return !lstrcmp(rPair.first, m_pString);
	}

private:
	const TCHAR* m_pString; 
};

#define __MYSTRCMP_H__
#endif // !__MYSTRCMP_H__
