#include "stdafx.h"
#include "CameraObserver.h"


CCameraObserver::CCameraObserver()
{
}


CCameraObserver::~CCameraObserver()
{
}

const D3DXVECTOR3 & CCameraObserver::GetCameraPos() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vCameraPos;
}

const D3DXMATRIX& CCameraObserver::GetViewMatrix() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matView;
}

const D3DXMATRIX& CCameraObserver::GetProjMatrix() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matProj;
}

const D3DXVECTOR3 & CCameraObserver::GetCameraLook() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vCameraLook;
}

void CCameraObserver::Update(int iMessage, void* pData)
{
	switch (iMessage)
	{
	case DATA_CAMERAPOS:
		m_vCameraPos = *reinterpret_cast<D3DXVECTOR3*>(pData);
		break;
	case DATA_VIEW:
		m_matView = *reinterpret_cast<D3DXMATRIX*>(pData);
		break;
	case DATA_PROJECTION:
		m_matProj = *reinterpret_cast<D3DXMATRIX*>(pData);
		break;
	case DATA_CAMERALOOK:
		m_vCameraLook = *reinterpret_cast<D3DXVECTOR3*>(pData);
		break;
	default:
		break;
	}
}

CCameraObserver* CCameraObserver::Create()
{
	return new CCameraObserver;
}
