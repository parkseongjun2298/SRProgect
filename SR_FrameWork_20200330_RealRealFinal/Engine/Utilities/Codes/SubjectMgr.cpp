#include "SubjectMgr.h"
#include "Subject.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CSubjectMgr)

CSubjectMgr::CSubjectMgr()
{
}


CSubjectMgr::~CSubjectMgr()
{
	Release();
}

HRESULT CSubjectMgr::AddSubject(const wstring& wstrSubjectKey, CSubject* pSubject)
{
	NULL_CHECK_RETURN(pSubject, E_FAIL);

	auto iter_find = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() != iter_find)
		return E_FAIL;

	m_mapSubject.insert(make_pair(wstrSubjectKey, pSubject));

	return S_OK;
}

void CSubjectMgr::Subscribe(const wstring& wstrSubjectKey, CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	auto iter_find = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter_find)
		return;

	m_mapSubject[wstrSubjectKey]->Subscribe(pObserver);
}

void CSubjectMgr::UnSubscribe(const wstring& wstrSubjectKey, CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	auto iter_find = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter_find)
		return;

	m_mapSubject[wstrSubjectKey]->UnSubscribe(pObserver);
}

void CSubjectMgr::Notify(const wstring& wstrSubjectKey, int iMessage, void* pData)
{
	NULL_CHECK(pData);

	auto iter_find = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter_find)
		return;

	m_mapSubject[wstrSubjectKey]->Notify(iMessage, pData);
}

void CSubjectMgr::Release()
{
	for (auto& MyPair : m_mapSubject)
		SafeDelete(MyPair.second);

	m_mapSubject.clear();
}
