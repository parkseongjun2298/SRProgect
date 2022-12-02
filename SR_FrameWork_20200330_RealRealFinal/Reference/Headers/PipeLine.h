#pragma once

#ifndef __PIPELINE_H__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CPipeLine
{
private:
	CPipeLine();
	~CPipeLine();

public:
	static void MakeWorldMatrix(
		D3DXMATRIX* pOut,
		const D3DXVECTOR3& vSize,
		const float* pAngle,
		const D3DXVECTOR3& vPos);
	static void MakeViewMatrix(
		D3DXMATRIX* pOut,
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR3& vAt,
		const D3DXVECTOR3& vUp);
	static void MakeProjectionMatrix(
		D3DXMATRIX* pOut,
		const float& fFovY,
		const float& fAspect,
		const float& fNear,
		const float& fFar);
	static void MakeTransformMatrix(
		D3DXMATRIX* pOut,
		const D3DXVECTOR3& vRight,
		const D3DXVECTOR3& vUp,
		const D3DXVECTOR3& vLook,
		const D3DXVECTOR3& vPos);
	static void MyTransformCoord(
		D3DXVECTOR3* pOut,
		const D3DXVECTOR3* pIn,
		const D3DXMATRIX* pMatrix);
};

END

#define __PIPELINE_H__
#endif