#pragma once

#ifndef __TRANSFORM_H__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();

public:
	virtual ~CTransform();

public:
	const D3DXVECTOR3&	GetPos() const;
	const D3DXVECTOR3&	GetDir() const;
	const D3DXMATRIX&	GetWorldMatrix() const;
	const D3DXVECTOR3&	GetSize() { return m_vSize; }
	const float&		GetAngle(ANGLE_TYPE eType) { return m_fAngle[eType]; }
public:
	void SetPos(const D3DXVECTOR3& vPos);
	void SetSize(const D3DXVECTOR3& vSize) { m_vSize = vSize; }
	void SetAngle(ANGLE_TYPE eType, float fAngle) { m_fAngle[eType] = fAngle; }
	void SetWorldMatrix(const D3DXMATRIX& matWorld);
	void SetWorldMatrixForRightUpLookPos(const D3DXVECTOR3& vRight, const D3DXVECTOR3& vUp,
		const D3DXVECTOR3& vLook, const D3DXVECTOR3& vPos);
	void SetTransformForUi(D3DXVECTOR3 vSize, D3DXVECTOR3 vParentPos);
	void SetParentWorldMatrix(const D3DXMATRIX& matParent) { m_matWorld = m_matWorld * matParent; }

public:
	virtual void LateUpdate() override;

public:
	void MovePos(float fMoveSpeed);	//바라보고 있는 방향
	void MoveShift(const D3DXVECTOR3& vShift) { m_vPos += vShift; }	//인자로 준 벡터만큼 이동
	void MoveAngle(ANGLE_TYPE eAxis, float fMoveSpeed);	//원하는 각도, 움직일 양

public:
	static CTransform* Create(	
		const D3DXVECTOR3& vLook,
		const D3DXVECTOR3& vPos,
		const D3DXVECTOR3& vSize);

private:
	D3DXMATRIX			m_matWorld;
	D3DXVECTOR3			m_vPos;
	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vLook;
	D3DXVECTOR3			m_vSize;

	float				m_fAngle[ANGLE_END];
};

END

#define __TRANSFORM_H__
#endif

