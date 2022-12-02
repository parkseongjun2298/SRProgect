#include "stdafx.h"
#include "GyroSwingDevice.h"
#include "CubeTex.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PipeLine.h"

#define GYROSWING_PLAYING_TIME 60.f

CGyroSwingDevice::CGyroSwingDevice(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CGameObject(pGraphicDev),
	m_pTimeMgr(ENGINE::CTimeMgr::GetInstance()),
	m_pResourceMgr(ENGINE::CResourceMgr::GetInstance()),
	m_pSubjectMgr(ENGINE::CSubjectMgr::GetInstance()),
	m_fAngle(0), m_fSpeed(1.f), m_fAngle2(0),
	m_fPlayTime(0), m_fAngleTime(0), m_fValue(0),
	m_bDirCheck(false), m_bFinCheck(false), m_fTime(0.f)
{
}


CGyroSwingDevice::~CGyroSwingDevice()
{
	Release();
}

int CGyroSwingDevice::Update()
{


	ENGINE::CGameObject::Update();
	int iEvent = MoveDevice();

	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	/*float fRotateSpeed = 90.f * m_pTimeMgr->GetDeltaTime();
	pTransform->SetAngle(ENGINE::ANGLE_Y, pTransform->GetAngle(ENGINE::ANGLE_Y) + fRotateSpeed);*/

	if (pTransform->GetAngle(ENGINE::ANGLE_Z) >= 165)
	{
		pTransform->SetAngle(ENGINE::ANGLE_Z, 165);
	}

	if (pTransform->GetAngle(ENGINE::ANGLE_Z) <= -165)
	{
		pTransform->SetAngle(ENGINE::ANGLE_Z, -165);
	}

	m_Matworld = pTransform->GetWorldMatrix();

	if (iEvent == -1)
		return -1;

	return NO_EVENT;
}

void CGyroSwingDevice::LateUpdate()
{

	ENGINE::CGameObject::LateUpdate();



}

void CGyroSwingDevice::Render()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	NULL_CHECK(pTransform);

	CCameraObserver* pObserver = dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]);
	NULL_CHECK(pObserver);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pTransform->GetWorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &pObserver->GetViewMatrix());
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pObserver->GetProjMatrix());
	//for (int i = 0; i < 8; ++i)
	//{
	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pOriginVtx[i].vPos,
	//		&pTransform->GetWorldMatrix());

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetViewMatrix());

	//	// 근평면과 원평면 외부에 존재하는 정점들은 투영변환에서 제외 시킨다.
	//	if (m_pConvertVtx[i].vPos.z < 1.f || m_pConvertVtx[i].vPos.z > 1000.f)
	//		continue;

	//	ENGINE::CPipeLine::MyTransformCoord(
	//		&m_pConvertVtx[i].vPos,
	//		&m_pConvertVtx[i].vPos,
	//		&pObserver->GetProjMatrix());
	//}

	m_pGraphicDev->SetTexture(0, nullptr);
	//dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_GyroSwingDevice"])->SetConvertVtx(m_pConvertVtx);


	m_mapComponent[L"Texture"]->Render(0);

	m_mapComponent[L"Buffer_GyroSwingDevice"]->Render();
}

HRESULT CGyroSwingDevice::Initialize()
{
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);


	ENGINE::CVIBuffer* pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(m_mapComponent[L"Buffer_GyroSwingDevice"]);
	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pOriginVtx = reinterpret_cast<ENGINE::VTX_CUBETEX*>(pBuffer->GetOriginVtx());
	NULL_CHECK_RETURN(m_pOriginVtx, E_FAIL);

	m_pConvertVtx = new ENGINE::VTX_CUBETEX[8];
	memcpy(m_pConvertVtx, m_pOriginVtx, sizeof(ENGINE::VTX_CUBETEX) * 8);

	m_eRideType = ENGINE::RIDE_GYROSWING;

	return S_OK;
}

HRESULT CGyroSwingDevice::AddComponent()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Buffer_GyroSwingDevice");
	NULL_CHECK_MSG_RETURN(pComponent, L"Buffer_GyroSwingDevice Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Buffer_GyroSwingDevice", pComponent));



	// Texture
	pComponent = m_pResourceMgr->CloneResources(ENGINE::RESOURCE_DYNAMIC, L"Texture_GyroSwingDevice");
	NULL_CHECK_MSG_RETURN(pComponent, L"Texture_GyroSwingDevice Clone Failed", E_FAIL);
	m_mapComponent.insert(make_pair(L"Texture", pComponent));

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

	return S_OK;
}

void CGyroSwingDevice::Release()
{
	m_pSubjectMgr->UnSubscribe(L"Camera", dynamic_cast<CCameraObserver*>(m_mapComponent[L"Observer"]));


	ENGINE::SafeDeleteArray(m_pConvertVtx);
}

CGyroSwingDevice * CGyroSwingDevice::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, float fAngleY)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CGyroSwingDevice* pInstance = new CGyroSwingDevice(pGraphicDev);
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetCreatePosSize(vPos, vSize, fAngleY);


	return pInstance;
}

D3DXMATRIX CGyroSwingDevice::Get_Matrix()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	return pTransform->GetWorldMatrix();
}

int CGyroSwingDevice::MoveDevice()
{
	ENGINE::CTransform* pTransform = dynamic_cast<ENGINE::CTransform*>(m_mapComponent[L"Transform"]);
	if (m_bDirCheck)
	{

		if (m_fPlayTime >= GYROSWING_PLAYING_TIME)
		{
			pTransform->SetAngle(ENGINE::ANGLE_X, pTransform->GetAngle(ENGINE::ANGLE_X) + m_fAngle * m_pTimeMgr->GetDeltaTime());
			if (fabsf(pTransform->GetAngle(ENGINE::ANGLE_X)) < 0.1f)
			{
				m_fPlayTime = 0.f;
				m_bDirCheck = false;
				m_fAngle = 0.f;
				pTransform->SetAngle(ENGINE::ANGLE_X, 0.f);


				m_bFinCheck = true;
				return -1;
			}
		}
		else
		{
			m_fPlayTime += m_pTimeMgr->GetDeltaTime();
			float fCosTime = cosf(D3DX_PI * m_fPlayTime);	//-1, 1사이의 값

			if (m_fPlayTime <= GYROSWING_PLAYING_TIME / 2.f)
				m_fAngle = fCosTime * (m_fPlayTime * m_pTimeMgr->GetDeltaTime() * 10.f);/// 7.5f);
			else
				m_fAngle = fCosTime * ((GYROSWING_PLAYING_TIME - m_fPlayTime) * m_pTimeMgr->GetDeltaTime() * 10.f); /// 7.5f);
			pTransform->SetAngle(ENGINE::ANGLE_X, pTransform->GetAngle(ENGINE::ANGLE_X) + m_fAngle);
			if (m_fPlayTime >= GYROSWING_PLAYING_TIME)
			{
				m_fAngle = -pTransform->GetAngle(ENGINE::ANGLE_X);
			}
		}
	}
	return NO_EVENT;
}

void CGyroSwingDevice::PlayGyroSwing()
{

	if (!m_bDirCheck)
	{
		m_fTime += m_pTimeMgr->GetDeltaTime();



		if (m_fTime >= 1.f)
		{
			m_bDirCheck = true;
			m_bFinCheck = false;
			m_fTime = 0;


		}
		//CSoundMgr::GetInstance()->PlaySound(L"바이킹소리.mp3", CSoundMgr::CHANNELID::RIDES);
		//CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::RIDES, 0.2f);
	}


}

