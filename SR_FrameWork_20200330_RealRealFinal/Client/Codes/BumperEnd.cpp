#include "stdafx.h"
#include "BumperEnd.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "PipeLine.h"

CBumperEnd::CBumperEnd(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev)
{
}


CBumperEnd::~CBumperEnd()
{
	Release();
}

int CBumperEnd::Update()
{
	CUiPlate::Update();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	pTransform->SetPos(D3DXVECTOR3{ 0.f,20,0.f });

	//m_vSize.x = 134.f;
	//m_vSize.y = 68.f;
	//pTransform->SetSize({ m_vSize.x, m_vSize.y, 1.f });
	//if (m_vSize.x < 156.f && m_vSize.y < 204.f)
	//{
	//	m_vSize.x += 156.f * m_pTimeMgr->GetDeltaTime();
	//	m_v
	//}

	return 0;
}

void CBumperEnd::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, D3DXVECTOR3(0.f, 0.f, 0.f));
}

void CBumperEnd::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);


	for (int i = 0; i < 4; ++i)
	{
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&pTransform->GetWorldMatrix());

		//ENGINE::CPipeLine::MyTransformCoord(
		//	&m_pConvertVtx[i].vPos,
		//	&m_pConvertVtx[i].vPos,
		//	&pObserver->GetViewMatrix());

		// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		D3DXMATRIX matOrtho;
		D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1000.f);

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&matOrtho);
	}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	// 알파 테스팅: 일정 알파 값 기준을 넘어선 부분은 소거한다(날려버린다).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.

	if(m_bIsFail)
		m_mapComponent[L"Texture_Fail"]->Render((WORD)m_iTextureIndex);
	else
		m_mapComponent[L"Texture"]->Render((WORD)m_iTextureIndex);

	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CBumperEnd::Initialize()
{
	CUiPlate::Initialize();
	return S_OK;
}

HRESULT CBumperEnd::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	if (m_iTextureIndex >= m_pResourceMgr->GetInstance()->GetTextureSize(ENGINE::RESOURCE_DYNAMIC, m_wstrTextureTag))
		return E_FAIL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_UiBase");
	if (pComponent == nullptr)
	{
		pComponent = nullptr;
	}
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_UiBase Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, m_wstrTextureTag);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_BumperCarFail");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Fail", pComponent));


	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, -1.f };
	D3DXVECTOR3 vPos = m_vPos;
	D3DXVECTOR3 vSize = m_vSize;
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));
	return S_OK;
}

void CBumperEnd::Release()
{
	CUiPlate::Release();
}

void CBumperEnd::Set_Victory(bool bCheck)
{
	m_bIsVictory = bCheck;
}

void CBumperEnd::Set_Fail(bool bCheck)
{
	m_bIsFail = bCheck;
}

CBumperEnd * CBumperEnd::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CBumperEnd* pInstance = new CBumperEnd(pGraphicDev);
	pInstance->m_vPos = D3DXVECTOR3(vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z);
	pInstance->m_vSize = vSize;
	pInstance->m_wstrTextureTag = wstrTextureTag;
	pInstance->m_iTextureIndex = iIndex;

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
