#include "stdafx.h"
#include "Customer.h"

#include "RcTex.h"
#include "Texture.h"

#include "Transform.h"
#include "CameraObserver.h"
#include "CollTerrain.h"
#include "CollMouse.h"
#include "Frame.h"

#include "PipeLine.h"
#include "LineMgr.h"
#include "FpsCamera.h"

#include "CustomerUi.h"
#include "UiStateBar.h"

#include "SoundMgr.h"
#include "UiStateBar.h"
#include "DeadObj.h"

CCustomer::CCustomer(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),	
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance()),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_pStateBar(dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())),
	m_pOriginVtx(nullptr),
	m_pConvertVtx(nullptr),
	m_fSpeed(5.f)
{
	m_pStateBar->PlusCustomer();
}

CCustomer::~CCustomer()
{
	Release();
}

int CCustomer::Update()
{
	if (m_lstRideTag.empty())
		dynamic_cast<ENGINE::CDeadObj*>(m_mapComponent[L"DeadObj"])->SetDead();
	KeyInput();
	FindPath();
	MoveGoalPos();

	m_pCustomerUi->Update();

	return ENGINE::CGameObject::Update();
}

void CCustomer::LateUpdate()
{
	ENGINE::CGameObject::LateUpdate();	

	m_pCustomerUi->LateUpdate();
}

void CCustomer::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 4; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos, 
	//		&m_pOriginVtx[i].vPos, 
	//		&pTransform->GetWorldMatrix());
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());
	//
	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if(m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}
	//
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	if(!m_bMove)
		m_mapComponent[L"Texture_Idle"]->Render(dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->GetCurrentFrame());
	else
		m_mapComponent[L"Texture_Run"]->Render(dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->GetCurrentFrame());
	m_mapComponent[L"Buffer"]->Render();

	if (CCameraMgr::GetInstance()->GetCurrentIdx() == 2)
	{
		if (dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->GetSetCustomer() != this)
			if (m_bUiRender)
				m_pCustomerUi->Render();
	}
	else
		if (m_bUiRender)
			m_pCustomerUi->Render();
}

void CCustomer::Render(Matrix matTransform)
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(pTransform->GetWorldMatrix() * matTransform));
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());

	//for (int i = 0; i < 4; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&(pTransform->GetWorldMatrix() * matTransform));
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());
	//
	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;
	//
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}
	//
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"])->SetConvertVtx(m_pConvertVtx);

	if (!m_bMove)
		m_mapComponent[L"Texture_Idle"]->Render(dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->GetCurrentFrame());
	else
		m_mapComponent[L"Texture_Run"]->Render(dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->GetCurrentFrame());
	m_mapComponent[L"Buffer"]->Render();
}

HRESULT CCustomer::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_TEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_TEX[4];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_TEX) * 4);

	// CustomerUi
	m_pCustomerUi = CCustomerUi::Create(m_pGraphicDev, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, this);
	NULL_CHECK_RETURN(m_pCustomerUi, E_FAIL);

	//2-4
	int iRide = rand() % 3 + 2;
	for (int i = 0; i < iRide; ++i)
	{
		ENGINE::RIDE_TYPE eRideType = (ENGINE::RIDE_TYPE)(rand() % (int)ENGINE::RIDE_END);
		wstring wstrRideTag;
		while (true)
		{
	
			switch (eRideType)
			{
			case ENGINE::RIDE_VIKING:
				wstrRideTag = L"Viking";
				break;
			case ENGINE::RIDE_SPINHORSE:
				wstrRideTag = L"SpinHorse";
				break;
			case ENGINE::RIDE_FERRIS:
				wstrRideTag = L"Ferris";
				break;
			case ENGINE::RIDE_GYRODROP:
				wstrRideTag = L"GyroDrop";
				break;
			case ENGINE::RIDE_ROLLERCOASTER:
				wstrRideTag = L"RollerCoaster";
				break;
			case ENGINE::RIDE_SPINTEACUP:
				wstrRideTag = L"SpinTeaCup";
				break;
			case ENGINE::RIDE_GYROSWING:
				wstrRideTag = L"GyroSwing";
				break;
			default:
				break;
			}
			if (ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_OBJECT, wstrRideTag) != nullptr)
				break;
			else
				eRideType = (ENGINE::RIDE_TYPE)(rand() % (int)ENGINE::RIDE_END);
		}
		m_lstRideTag.push_back(wstrRideTag);
	}

	//m_lstRideTag.push_back(L"RollerCoaster");

	return S_OK;
}

HRESULT CCustomer::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Customer");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_Customer Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer", pComponent));


	int iRand = rand() % 4;
	wstring wstrTag1 = L"";
	wstring wstrTag2 = L"";
	switch (iRand)
	{
	case 0:
		wstrTag1 = L"Texture_Customer_Run1";
		wstrTag2 = L"Texture_Customer_Idle1";
		break;
	case 1:
		wstrTag1 = L"Texture_Customer_Run2";
		wstrTag2 = L"Texture_Customer_Idle2";
		break;
	case 2:
		wstrTag1 = L"Texture_Customer_Run2";
		wstrTag2 = L"Texture_Customer_Idle2";
		break;
	case 3:
		wstrTag1 = L"Texture_Customer_Run3";
		wstrTag2 = L"Texture_Customer_Idle3";
		break;
	default:
		break;
	}
	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, wstrTag1);
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Customer_Run Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Run", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, wstrTag2);
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_Customer_Idle Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture_Idle", pComponent));

	// Transform
	D3DXVECTOR3 vLook = { 0.f, 0.f, 1.f };
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vSize = { 1.f, 1.f, 1.f };
	pComponent = ENGINE::CTransform::Create(vLook, vPos, vSize);
	NULL_CHECK_MSG_RETURN(pComponent, L"Transform Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Transform", pComponent));

	// Observer
	pComponent = CCameraObserver::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"Observer Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Observer", pComponent));	
	m_pSubjectMgr->Subscribe(L"Camera", dynamic_cast<CCameraObserver*>(pComponent));

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(pComponent);
	NULL_CHECK_RETURN(pObserver, E_FAIL);

	pComponent = ENGINE::CFrame::Create(0.f, 4, 1.f, true);
	NULL_CHECK_MSG_RETURN(pComponent, L"CollTerrain Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Frame", pComponent));

	//Collision Mouse
	pComponent = CCollMouse::Create(pObserver);
	NULL_CHECK_MSG_RETURN(pComponent, L"CollMouse Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"CollMouse", pComponent));

	//DeadObj
	pComponent = ENGINE::CDeadObj::Create();
	NULL_CHECK_MSG_RETURN(pComponent, L"DeadObj Create Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"DeadObj", pComponent));

	m_bMove = false;
	m_vDir = { 0.f, 0.f, 1.f };

	return S_OK;
}

void CCustomer::Release()
{
	m_pStateBar->MinusCustomer();
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));
	ENGINE::SafeDeleteArray(m_pConvertVtx);
	ENGINE::SafeDelete(m_pCustomerUi);
}

void CCustomer::MoveGoalPos()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	float fMoveValue = m_fSpeed * m_pTimeMgr->GetDeltaTime();
	if (m_bMove)
	{
		m_vDir = VEC3::Normalize(m_lstGoalPos.front()- pTransform->GetPos());
		if (m_vDir.x >= 0)
			pTransform->SetSize(Vec3(1.f, 1.f, 1.f));
		else
			pTransform->SetSize(Vec3(-1.f, 1.f, 1.f));

		if (fMoveValue > VEC3::Length(m_lstGoalPos.front() - pTransform->GetPos()))
			fMoveValue = VEC3::Length(m_lstGoalPos.front() - pTransform->GetPos());
		pTransform->MoveShift(m_vDir * fMoveValue);
		if (VEC3::Length(pTransform->GetPos() - m_lstGoalPos.front()) <= EPSILON)
		{
			m_lstGoalPos.pop_front();
			if (m_lstGoalPos.empty())
			{
				m_bMove = false;
				dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->ChangeFrame(0.f, 4, 1.f);
			}
		}
	}
}

void CCustomer::KeyInput()
{

	CCollMouse* pCollMouse = dynamic_cast<CCollMouse*>(m_mapComponent[L"CollMouse"]);
	NULL_CHECK(pCollMouse);

	D3DXVECTOR3 vPickPos = {};
	bool		bPick = false;

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

	bPick = pCollMouse->PickTile(&vPickPos, ENGINE::LAYER_OBJECT, L"Terrain", m_pOriginVtx, pTransform->GetWorldMatrix());

	if (bPick)
	{
		if (m_pKeyMgr->KeyPressing(ENGINE::KEY_RBUTTON))
		{
			CCameraMgr::GetInstance()->SetCameraIdx(1);
			dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->SetCustomer(this);
			m_bUiRender = false;
		}
		m_bUiRender = true;
	}
	else
		m_bUiRender = false;
	
	//if (m_pKeyMgr->KeyDown(ENGINE::KEY_NUMPAD8))
	//	m_pCustomerUi->Set_HogamdoGage(1);
	//if (m_pKeyMgr->KeyDown(ENGINE::KEY_NUMPAD2))
	//	m_pCustomerUi->Set_HogamdoGage(-1);
}

void CCustomer::FindPath()
{
	if (m_bMove)
		return;
	if (m_lstRideTag.empty())
		return;
	list<ENGINE::CGameObject*>* lstRide = ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_OBJECT, m_lstRideTag.front());
	if (lstRide == nullptr)
	{
		//cout << "찾으시는 놀이기구가 없습니다." << endl;
		m_lstRideTag.pop_front();
		return;
	}
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	float fCriteriaDist = 9999.f;
	ENGINE::CGameObject* pRide = nullptr;
	//가장 가까운 놀이기구 찾기
	for (auto Ride : *lstRide)
	{
		if (VEC3::Length(Ride->GetPos() - pTransform->GetPos()) < fCriteriaDist)
		{
			fCriteriaDist = VEC3::Length(Ride->GetPos() - pTransform->GetPos());
			pRide = Ride;
		}
	}
	if (pRide == nullptr)
	{
		//cout << "찾으시는 놀이기구가 없습니다." << endl;
		m_lstRideTag.pop_front();
		return;
	}

	m_bMove = true;
	Vec3 vGoalPos = pRide->GetPos();
	vGoalPos.y = 0.f;
	m_lstGoalPos.push_back(vGoalPos);
	dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->ChangeFrame(0.f, 8, 1.f);
}

void CCustomer::KeyInput(ENGINE::CGameObject* pRide, Matrix matParentWorld, Matrix matSeat)
{
	if (m_pKeyMgr->KeyPressing(ENGINE::KEY_RBUTTON))
	{
		CCollMouse* pCollMouse = dynamic_cast<CCollMouse*>(m_mapComponent[L"CollMouse"]);
		NULL_CHECK(pCollMouse);

		D3DXVECTOR3 vPickPos = {};
		bool		bPick = false;

		ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);

		bPick = pCollMouse->PickTile(&vPickPos, ENGINE::LAYER_OBJECT, L"Terrain", m_pOriginVtx, pTransform->GetWorldMatrix() * matSeat * pRide->GetWorldMatrix()* matParentWorld);

		if (bPick)
		{
			CCameraMgr::GetInstance()->SetCameraIdx(1);
			dynamic_cast<CFpsCamera*>(CCameraMgr::GetInstance()->GetCurrentCamera())->SetCustomer(this, pRide, matParentWorld, matSeat);
		}
	}
}

void CCustomer::RideSetting(Vec3 vDir)
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	if (VEC3::Length(CCameraMgr::GetInstance()->GetCurrentCamera()->GetEye() - pTransform->GetPos()) <= 50.f)
	{
		CSoundMgr::GetInstance()->PlaySound(L"17.치링.mp3", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::EFFECT, 0.1f);
	}

	pTransform->SetPos({ 0.f, 0.f, 0.f });
	pTransform->LateUpdate();
	m_vDir = vDir;
	m_lstGoalPos.clear();
	m_bMove = false;
	dynamic_cast<ENGINE::CFrame*>(m_mapComponent[L"Frame"])->ChangeFrame(0.f, 4, 1.f);
	m_lstRideTag.pop_front();
	m_pCustomerUi->Set_HogamdoGage(1);

	dynamic_cast<CUiStateBar*>(ENGINE::CManagement::GetInstance()->GetObjectLst(ENGINE::LAYER_UI, L"UiStateBar")->front())->PlusCoin(100);

}

void CCustomer::SetGoalPos(list<Vec3> lstPos)
{
	if (m_bMove)
		return;
	m_bMove = true;
	m_lstGoalPos = lstPos;
}

CCustomer* CCustomer::Create(LPDIRECT3DDEVICE9 pGraphicDev, Vec3 vPos)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CCustomer* pInstance = new CCustomer(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, { 1.f, 1.f, 1.f }, 0.f);

	return pInstance;
}
