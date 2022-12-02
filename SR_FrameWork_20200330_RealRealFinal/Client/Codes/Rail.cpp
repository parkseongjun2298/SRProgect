#include "stdafx.h"
#include "Rail.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"


CRail::CRail(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_fTimeCount(0.f), m_iRailIndex(1), m_iRailNum(0),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance())
{
}


CRail::~CRail()
{
	Release();
}

int CRail::Update()
{
	ENGINE::CGameObject::Update();
	return NO_EVENT;
}

void CRail::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetWorldMatrixForRightUpLookPos(m_vRight, m_vUp, m_vLook, pTransform->GetPos());
}

void CRail::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	Matrix matParentMatrix = m_pParent->GetWorldMatrix();
	Matrix matWorld = pTransform->GetWorldMatrix() * matParentMatrix;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 8; ++j)
	//	{
	//		ENGINE::CPipeLine::MyTransformCoord(
	//			&m_pConvertVtx[i][j].vPos,
	//			&m_pOriginVtx[i][j].vPos,
	//			&matWorld);

	//		ENGINE::CPipeLine::MyTransformCoord(
	//			&m_pConvertVtx[i][j].vPos,
	//			&m_pConvertVtx[i][j].vPos,
	//			&pObserver->GetViewMatrix());

	//		// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//		if (m_pConvertVtx[i][j].vPos.z < 1.f || m_pConvertVtx[i][j].vPos.z > 1000.f)
	//			continue;

	//		ENGINE::CPipeLine::MyTransformCoord(
	//			&m_pConvertVtx[i][j].vPos,
	//			&m_pConvertVtx[i][j].vPos,
	//			&pObserver->GetProjMatrix());
	//	}
	//}

	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_RailLeft"])->SetConvertVtx(m_pConvertVtx[0]);
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_RailRight"])->SetConvertVtx(m_pConvertVtx[1]);
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_RailMiddle"])->SetConvertVtx(m_pConvertVtx[2]);

	m_mapComponent[L"Texture_Rail"]->Render(0);
	m_mapComponent[L"Buffer_Rail"]->Render();
}

HRESULT CRail::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_Rail"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);


	this->SetLook(m_vLook, m_vUp);

	m_vStart = m_vPos - (m_vLook * (RAIL_ITV * 0.5f));
	m_vEnd = m_vPos + (m_vLook * (RAIL_ITV * 0.5f));

	return S_OK;
}

HRESULT CRail::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Rail");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Rail Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_Rail", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Rail");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Rail Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Rail", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 5.f, 15.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	return S_OK;
}

void CRail::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDelete(m_pConvertVtx);
}

void CRail::KeyInput()
{
}

void CRail::SetLook(Vec3 vLook, Vec3 vUp)
{
	m_vLook = vLook;
	m_vLook = VEC3::Normalize(m_vLook);
	m_vRight = VEC3::Normalize(VEC3::CrossProduct(vUp, m_vLook));
	m_vUp = VEC3::Normalize(VEC3::CrossProduct(m_vLook, m_vRight));
}

void CRail::SetUp(Vec3 vUp, Vec3 vRight)
{
	m_vUp = vUp;
	m_vUp = VEC3::Normalize(m_vUp);
	m_vLook = VEC3::Normalize(VEC3::CrossProduct(vRight, m_vUp));
	m_vRight = VEC3::Normalize(VEC3::CrossProduct(m_vUp, m_vLook));
}

void CRail::SetRight(Vec3 vRight, Vec3 vLook)
{
	m_vRight = vRight;
	m_vRight = VEC3::Normalize(m_vRight);
	m_vUp = VEC3::Normalize(VEC3::CrossProduct(vLook, m_vRight));
	m_vLook = VEC3::Normalize(VEC3::CrossProduct(m_vRight, m_vUp));
}

ENGINE::RAIL_INFO CRail::GetRailInfo()
{
	ENGINE::RAIL_INFO tInfo;
	tInfo.bExist = true;
	tInfo.vPos = m_vPos;
	tInfo.vSize = m_vSize;
	tInfo.vLook = m_vLook;
	tInfo.vUp = m_vUp;
	return tInfo;
}

CRail * CRail::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos,
	D3DXVECTOR3 vLook, D3DXVECTOR3 vUp, Vec3 vSize, ENGINE::CGameObject* pParent)
{
	CRail* pInstance = new CRail(pGraphicDev);
	pInstance->m_vLook = vLook;
	pInstance->m_vUp = vUp;
	pInstance->m_vPos = vPos;
	pInstance->m_pParent = pParent;
	pInstance->m_vSize = vSize;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, 0.f);
	return pInstance;
}
