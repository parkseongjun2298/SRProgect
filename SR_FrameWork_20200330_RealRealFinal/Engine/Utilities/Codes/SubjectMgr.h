#pragma once

#ifndef __SUBJECTMGR_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CSubject;
class CObserver;
class ENGINE_DLL CSubjectMgr
{
	DECLARE_SINGLETON(CSubjectMgr)

private:
	CSubjectMgr();
	virtual ~CSubjectMgr();

public:
	HRESULT AddSubject(const wstring& wstrSubjectKey, CSubject* pSubject);
	void Subscribe(const wstring& wstrSubjectKey, CObserver* pObserver);
	void UnSubscribe(const wstring& wstrSubjectKey, CObserver* pObserver);
	void Notify(const wstring& wstrSubjectKey, int iMessage, void* pData);

private:
	void Release();

private:
	map<wstring, CSubject*>	m_mapSubject;
};

END

#define __SUBJECTMGR_H__
#endif
