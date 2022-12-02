#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

USING(ENGINE)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
}


CGameObject::~CGameObject()
{
	Release();
}

CComponent* CGameObject::GetComponent(const wstring& wstrComponentKey)
{
	auto iter_find = m_mapComponent.find(wstrComponentKey);

	if(m_mapComponent.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

D3DXVECTOR3 CGameObject::GetPos()
{
	auto iter_find = m_mapComponent.find(L"Transform");
	if (m_mapComponent.end() == iter_find)
		return D3DXVECTOR3(-999.f, -999.f, -999.f);
	return dynamic_cast<ENGINE::CTransform*>(iter_find->second)->GetPos();
}

ENGINE::SAVE_INFO CGameObject::GetSaveInfo()
{
	auto iter_find = m_mapComponent.find(L"Transform");
	if (m_mapComponent.end() == iter_find)
		return ENGINE::SAVE_INFO();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(iter_find->second);

	SAVE_INFO tInfo;
	tInfo.vPos = pTransform->GetPos();
	tInfo.vSize = pTransform->GetSize();
	tInfo.fAngleY = pTransform->GetAngle(ENGINE::ANGLE_Y);
	return tInfo;
}

D3DXMATRIX CGameObject::GetWorldMatrix()
{
	auto iter_find = m_mapComponent.find(L"Transform");
	if (m_mapComponent.end() == iter_find)
		return D3DXMATRIX();
	return dynamic_cast<ENGINE::CTransform*>(iter_find->second)->GetWorldMatrix();
}

void CGameObject::SetCreatePosSize(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	auto iter_find = m_mapComponent.find(L"Transform");
	if (m_mapComponent.end() == iter_find)
		return;
	dynamic_cast<ENGINE::CTransform*>(iter_find->second)->SetPos(vPos);
	dynamic_cast<ENGINE::CTransform*>(iter_find->second)->SetSize(vSize);
	dynamic_cast<ENGINE::CTransform*>(iter_find->second)->SetAngle(ENGINE::ANGLE_Y, fAngleY);
}

int CGameObject::Update()
{
	for (auto& MyPair : m_mapComponent)
		if (DEAD_OBJ == MyPair.second->Update())
			return DEAD_OBJ;

	return NO_EVENT;
}

void CGameObject::LateUpdate()
{
	for (auto& MyPair : m_mapComponent)
		MyPair.second->LateUpdate();
}

void CGameObject::Render()
{
}

HRESULT CGameObject::Initialize()
{
	return S_OK;
}

void CGameObject::Release()
{
	for (auto& MyPair : m_mapComponent)
		SafeDelete(MyPair.second);

	m_mapComponent.clear();
}
