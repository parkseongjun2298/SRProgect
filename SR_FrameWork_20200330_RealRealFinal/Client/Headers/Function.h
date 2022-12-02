#pragma once

#ifndef __FUNCTION_H__
#include "stdafx.h"
#include "VIBuffer.h"

//inline void	ChangeScene(ENGINE::SCENE_TYPE eSceneType, bool bCameraClear = false)
//{
//	ENGINE::CResourceMgr::GetInstance()->ChangeSceneType(eSceneType);
//	CCameraMgr::GetInstance()->ChangeSceneCamera(eSceneType, bCameraClear);
//}

inline float ReflectionLight(D3DXVECTOR3 vNormal)
{
	D3DXVECTOR3 vLight = { -1.f, -1.f, -1.f };
	D3DXVec3Normalize(&vLight, &vLight);

	float fDot = D3DXVec3Dot(&vLight, &vNormal);

	fDot = fDot - 1.f;
	fDot /= 2.f;
	fDot *= -1.f;

	return fDot;
}

inline bool	IsZero(float fValue) { return ((fabsf(fValue) < EPSILON)); }
inline bool	IsEqual(float fValue1, float fValue2) { return(::IsZero(fValue1 - fValue2)); }
inline void Swap(float* pfSource, float* pfResult) { float fTemp = *pfSource; *pfSource = *pfResult; *pfResult = fTemp; }

BEGIN(VEC3)

inline D3DXVECTOR3	Multiply(const Vec3& vValue1, const Vec3& vValue2) {
	Vec3 vResult;
	vResult = { vValue1.x * vValue2.x, vValue1.y * vValue2.y, vValue1.z * vValue2.z };
	return vResult;
}
inline D3DXVECTOR3	Normalize(const D3DXVECTOR3& vValue)
{
	D3DXVECTOR3 vNormal;
	D3DXVec3Normalize(&vNormal, &vValue);
	return vNormal;
}
inline D3DXVECTOR3	CrossProduct(const D3DXVECTOR3& vValue1, const D3DXVECTOR3& vValue2, bool bNormal = true)
{
	D3DXVECTOR3 vResult;
	if (bNormal)
	{
		D3DXVec3Cross(&vResult, &vValue1, &vValue2);
		vResult = Normalize(vResult);
	}
	else
		D3DXVec3Cross(&vResult, &vValue1, &vValue2);

	return vResult;
}
inline float		DotProduct(const D3DXVECTOR3& vValue1, const D3DXVECTOR3& vValue2)
{
	return D3DXVec3Dot(&vValue1, &vValue2);
}
inline float		Length(const D3DXVECTOR3& vValue)
{
	return D3DXVec3Length(&vValue);
}
inline float		Angle(const D3DXVECTOR3& vValue1, const D3DXVECTOR3& vValue2, bool bRadian = true)
{
	D3DXVECTOR3 value1 = Normalize(vValue1);
	D3DXVECTOR3 value2 = Normalize(vValue2);

	float fResult = DotProduct(value1, value2);
	fResult = acosf(fResult);
	if (!bRadian)
		return D3DXToDegree(fResult);
	return fResult;
}
inline D3DXVECTOR3	TransformNormal(const D3DXVECTOR3& vValue, const D3DXMATRIX& vMatrix)
{
	D3DXVECTOR3	vResult;
	D3DXVec3TransformNormal(&vResult, &vValue, &vMatrix);
	return vResult;
}
inline D3DXVECTOR3	TransformCoord(const D3DXVECTOR3& vValue, const D3DXMATRIX& vMatrix)
{
	D3DXVECTOR3	vResult;
	D3DXVec3TransformCoord(&vResult, &vValue, &vMatrix);
	return vResult;
}
END

BEGIN(MATRIX)

inline D3DXMATRIX Identity()
{
	D3DXMATRIX matResult;
	D3DXMatrixIdentity(&matResult);
	return matResult;
}

inline D3DXMATRIX LookAtLH(const D3DXVECTOR3& vEyePosition, const D3DXVECTOR3& vLookAt, 
	const D3DXVECTOR3& vUpDirection)
{
	D3DXMATRIX matResult;
	D3DXMatrixLookAtLH(&matResult, &vEyePosition, &vLookAt, &vLookAt);
	return matResult;
}

inline D3DXMATRIX Scaling(const Vec3& vSize)
{
	D3DXMATRIX matResult;
	D3DXMatrixScaling(&matResult, vSize.x, vSize.y, vSize.z);
	return matResult;
}

inline D3DXMATRIX RotateX(float fRadian)
{
	D3DXMATRIX matResult;
	D3DXMatrixRotationX(&matResult, fRadian);
	return matResult;
}
inline D3DXMATRIX RotateY(float fRadian)
{
	D3DXMATRIX matResult;
	D3DXMatrixRotationY(&matResult, fRadian);
	return matResult;
}
inline D3DXMATRIX RotateZ(float fRadian)
{
	D3DXMATRIX matResult;
	D3DXMatrixRotationZ(&matResult, fRadian);
	return matResult;
}
inline D3DXMATRIX RotateAxis(D3DXVECTOR3 vAxis, float fRadian)
{
	D3DXMATRIX matResult;
	D3DXMatrixRotationAxis(&matResult, &vAxis, fRadian);
	return matResult;
}
inline D3DXMATRIX Translation(const Vec3& vPos)
{
	D3DXMATRIX matResult;
	D3DXMatrixTranslation(&matResult, vPos.x, vPos.y, vPos.z);
	return matResult;
}

END

#define __FUNCTION_H__
#endif