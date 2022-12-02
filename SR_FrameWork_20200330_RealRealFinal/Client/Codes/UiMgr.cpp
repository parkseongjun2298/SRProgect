#include "stdafx.h"
#include "UiMgr.h"
#include "UiGroup.h"
#include "UiPlate.h"
#include "UiBase.h"
#include "Transform.h"
#include "UiTexture.h"

IMPLEMENT_SINGLETON(CUiMgr)
CUiMgr::CUiMgr()
	: m_bOpen(false),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_iGroupIdx(0),
	m_iSelectIdx(-1),
	m_wstrSelect(L"")
{
}


CUiMgr::~CUiMgr()
{
	Release();
}

bool CUiMgr::Update()
{
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_TAB) && !m_bFirstFrame)
		m_bOpen = false;
	m_bFirstFrame = false;
	if (m_bOpen)
	{
		m_pTransform->MoveShift(D3DXVECTOR3(676.f * m_pTimeMgr->GetDeltaTime() * 2.f, 0.f, 0.f));
		if (m_pTransform->GetPos().x >= 0)
		{
			m_pTransform->SetPos({ 0.f, -40.f, 0.f });
		}
	}
	else
	{
		m_pTransform->MoveShift(D3DXVECTOR3(-676.f * m_pTimeMgr->GetDeltaTime() * 4.f, 0.f, 0.f));
		if (m_pTransform->GetPos().x <= -676.f)
		{
			m_pTransform->SetPos({ -676.f, -40.f, 0.f });
			return false;
		}
	}
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_LEFT))
		--m_iGroupIdx;
	if (m_pKeyMgr->KeyDown(ENGINE::KEY_RIGHT))
		++m_iGroupIdx;

	if (m_iGroupIdx < 0)
		m_iGroupIdx = 0;
	if (m_iGroupIdx >= m_vecUiGroup.size())
		m_iGroupIdx = m_vecUiGroup.size() - 1;

	m_pBackGround->Update();
	//m_pUiTexture->Update();

	m_vecUiGroup[m_iGroupIdx]->Update(&m_wstrSelect, &m_iSelectIdx);
	return true;
}

void CUiMgr::LateUpdate()
{
	m_pTransform->LateUpdate();
	m_pBackGround->LateUpdate();
	//m_pUiTexture->LateUpdate();
	m_vecUiGroup[m_iGroupIdx]->LateUpdate();
}

void CUiMgr::Render()
{
	m_pBackGround->Render();
	//m_pUiTexture->Render();
	m_vecUiGroup[m_iGroupIdx]->Render();
}

HRESULT CUiMgr::SetGraphicDev(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_vecUiGroup.push_back(CUiGroup::Create(m_pGraphicDev, ENGINE::BUILDING_TILE));
	m_vecUiGroup.push_back(CUiGroup::Create(m_pGraphicDev, ENGINE::BUILDING_RIDES));
	m_vecUiGroup.push_back(CUiGroup::Create(m_pGraphicDev, ENGINE::BUILDING_COMFY));
	m_vecUiGroup.push_back(CUiGroup::Create(m_pGraphicDev, ENGINE::BUILDING_DESIGN));
	m_pBackGround = (CUiBase::Create(m_pGraphicDev, { -WINCX * 0.5f, WINCY * 0.5f, 0.f }, { 676.f, 414.f, 0.f }, L"Texture_UiBase"));
	m_pTransform = ENGINE::CTransform::Create({ 0.f, 0.f, -1.f }, { -676.f, -40.f, 0.f }, { 0.f, 0.f, 0.f });
	//m_pUiTexture = CUiTexture::Create(m_pGraphicDev, { -WINCX * 0.5f, 30.f - (64.f * 3.f), 0.f }, { 320 * 3.f, 64 * 3.f, 0.f }, L"Texture_UiTalk");


	return E_NOTIMPL;
}

wstring CUiMgr::GetwstrSelect()
{
	return m_wstrSelect;
}

size_t CUiMgr::GetiSelect()
{
	return m_iSelectIdx;
}

void CUiMgr::Release()
{
	ENGINE::SafeDelete(m_pBackGround);
	//ENGINE::SafeDelete(m_pUiTexture);
	for (auto& Plate : m_vecUiGroup)
		ENGINE::SafeDelete(Plate);
	m_vecUiGroup.clear();
	ENGINE::SafeDelete(m_pTransform);
}