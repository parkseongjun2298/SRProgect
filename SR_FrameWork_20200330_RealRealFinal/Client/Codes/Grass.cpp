#include "stdafx.h"
#include "Grass.h"
#include "RcTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "PipeLine.h"

CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),	
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr)
{
	
}

CGrass::~CGrass()
{
	Release();
}

int CGrass::Update()
{
	ENGINE::CGameObject::Update();

	return NO_EVENT;
}

void CGrass::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();
	IsBillboarding();
}

void CGrass::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);	

	for (int i = 0; i < 4; ++i)
	{
		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pOriginVtx[i].vPos,
			&pTransform->GetWorldMatrix());

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetViewMatrix());

		// ������ ����� �ܺο� �����ϴ� �������� ������ȯ���� ���� ��Ų��.
		if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
			continue;

		ENGINE::CPipeLine::MyTransformCoord(
			&m_pConvertVtx[i].vPos,
			&m_pConvertVtx[i].vPos,
			&pObserver->GetProjMatrix());
	}

	dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);


	// ���� �׽���: ���� ���� �� ������ �Ѿ �κ��� �Ұ��Ѵ�(����������).
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);	

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1); // ���� ���� �� 1�� ����.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���� ������ ���� �κ��� ������.

	m_mapComponent[L"Texture"]->Render(0);
	m_mapComponent[L"Buffer"]->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// ���� ����
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_mapComponent[L"Texture"]->Render(0);
	//m_mapComponent[L"Buffer"]->Render();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	/*Z Read(Z Test)
		- A�� �׷��� �� ������ �׷��� B�� Z�� �񱳸� �Ͽ�
		- A�� Z ���� B�� Z ������ ���� �� A�� �׸���.

		SetRenderState(D3DRS_ZENABLE, TRUE);

	Z Write
		- �ڽ��� Z ���� Z ���ۿ� ����Ѵ�.

		SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	3D �󿡼� ���ĺ��� ó���� �ϱ� ���ؼ���

		Step1.
		�������� ���� ���� �׸���. (�� ���� ������ �׸��� ���� ��� X)

		Step2.
		�����ϰų� �������� ����� �׸���. (�� ���� ������ ������ �Ǿ��־�� ��)
		* ���� ���� : ī�޶�κ��� �ָ��ִ� ������ ���� �׷�������.*/
}

HRESULT CGrass::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);	

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	return S_OK;
}

HRESULT CGrass::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Grass");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Grass Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_Grass");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Grass Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

	// Transform
	float fX = float(rand() % TERRAIN_CNTX);
	float fZ = float(rand() % TERRAIN_CNTZ);

	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { fX, 0.f, fZ };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));

	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	// Collision Terrain
	pComponent = CCollTerrain::Create(this);
	NULL_CHECK_MSG_RETURN(pComponent, L"CollTerrain Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"CollTerrain", pComponent));

	return S_OK;
}

void CGrass::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

void CGrass::IsBillboarding()
{
	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	// ī�޶� ȸ���� �ݴ�� ������ ���̸� ������ �ϼ�.
	// ������: �׻� ī�޶� ������ �ٶ󺸰� ����� ���.
	// ex) Ǯ, ����, ������, ����Ʈ ��.

	// ��ü �� ������
	D3DXMATRIX matBill;
	D3DXMatrixIdentity(&matBill);

	// �������� ������ ��� ���Ҹ� ����.
	memcpy(&matBill.m[0][0], &pObserver->GetViewMatrix().m[0][0], sizeof(D3DXVECTOR3));
	memcpy(&matBill.m[1][0], &pObserver->GetViewMatrix().m[1][0], sizeof(D3DXVECTOR3));
	memcpy(&matBill.m[2][0], &pObserver->GetViewMatrix().m[2][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	D3DXMATRIX matWorld = pTransform->GetWorldMatrix();

	// �� * �� * �� * �� * ��
	pTransform->SetWorldMatrix(matBill * matWorld);

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//// Y�� ������
	//D3DXMATRIX matBillY;
	//D3DXMatrixIdentity(&matBillY);

	//// ������� Y�� ȸ�� ���� ����.
	//matBillY._11 = pObserver->GetViewMatrix()._11;
	//matBillY._13 = pObserver->GetViewMatrix()._13;
	//matBillY._31 = pObserver->GetViewMatrix()._31;
	//matBillY._33 = pObserver->GetViewMatrix()._33;	

	//D3DXMatrixInverse(&matBillY, nullptr, &matBillY);

	//ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	//NULL_CHECK(pTransform);

	//D3DXMATRIX matWorld = pTransform->GetWorldMatrix();

	//// �� * �� * �� * �� * ��
	//pTransform->SetWorldMatrix(matBillY * matWorld);
}

CGrass* CGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CGrass* pInstance = new CGrass(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
