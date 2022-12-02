#include "stdafx.h"
#include "ShootingOver.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "PipeLine.h"

CShootingOver::CShootingOver(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUiPlate(pGraphicDev),
	m_pFontMgr(ENGINE::CFontMgr::GetInstance()),
	m_iCount(0), m_fTime(30.f), m_bCheck(false), m_bRendFont(false)
{
}


CShootingOver::~CShootingOver()
{
	Release();
}

int CShootingOver::Update()
{
	CUiPlate::Update();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	if (ENGINE::CKeyMgr::GetInstance()->KeyPressing(ENGINE::KEY_RBUTTON))
		m_bCheck = true;
	if (m_bCheck)
	{
		m_fTime -= m_pTimeMgr->GetDeltaTime();

		if (m_fTime <= 0)
		{
			if (pTransform->GetPos().y > -20.f)
				pTransform->MoveShift(D3DXVECTOR3{ 0.f,-600.f,0.f }*m_pTimeMgr->GetDeltaTime());


			if (pTransform->GetPos().y <= -20.f)
			{
				pTransform->SetPos(D3DXVECTOR3{ 0.f,-20.f,0.f });
				m_bRendFont = true;
			}


		}
	}
	return NO_EVENT;
}

void CShootingOver::LateUpdate()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXVECTOR3 vSize = pTransform->GetSize();
	pTransform->SetTransformForUi({ vSize.x / WINCX, vSize.y / WINCY, 1.f }, D3DXVECTOR3(0.f, 0.f, 0.f));

}

void CShootingOver::Render()
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

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 10); // 알파 기준 값 1로 설정.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // 기준 값보다 작은 부분은 날려라.
	m_mapComponent[L"Texture"]->Render((WORD)m_iTextureIndex);
	m_mapComponent[L"Buffer"]->Render();

	Render_Count();
	if (m_bRendFont)
	{
		m_pFontMgr->Render(L"둥근모꼴두꺼움", D3DXVECTOR3(600, 350.f, 0.f), m_szTextureCount, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CShootingOver::Initialize()
{
	CUiPlate::Initialize();
	return S_OK;
}

HRESULT CShootingOver::AddComponent()
{
	CUiPlate::AddComponent();
	return S_OK;
}

void CShootingOver::Release()
{
	CUiPlate::Release();
}

void CShootingOver::Render_Count()
{
	ZeroMemory(&m_szTextureCount, sizeof(m_szTextureCount));
	swprintf_s(m_szTextureCount, L"%d", m_iCount);
}

void CShootingOver::Notify_Count(int iCount)
{
	m_iCount = iCount;
}

CShootingOver * CShootingOver::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CShootingOver* pInstance = new CShootingOver(pGraphicDev);
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
