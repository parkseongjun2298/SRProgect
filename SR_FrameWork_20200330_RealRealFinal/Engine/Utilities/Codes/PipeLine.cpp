#include "PipeLine.h"
#include "MathMgr.h"

USING(ENGINE)

CPipeLine::CPipeLine()
{
}


CPipeLine::~CPipeLine()
{
}

void CPipeLine::MakeWorldMatrix(
	D3DXMATRIX* pOut, 
	const D3DXVECTOR3& vSize, 
	const float* pAngle, 
	const D3DXVECTOR3& vPos)
{
	D3DXMatrixIdentity(pOut);

	D3DXVECTOR3 vRight	= { 1.f, 0.f, 0.f };
	D3DXVECTOR3 vUp		= { 0.f, 1.f, 0.f };
	D3DXVECTOR3 vLook	= { 0.f, 0.f, 1.f };

	// 스케일
	vRight.x *= vSize.x;
	vUp.y *= vSize.y;
	vLook.z *= vSize.z;

	// 자전
	CMathMgr::MyRotationX(&vRight, &vRight, D3DXToRadian(pAngle[ANGLE_X]));
	CMathMgr::MyRotationX(&vUp, &vUp, D3DXToRadian(pAngle[ANGLE_X]));
	CMathMgr::MyRotationX(&vLook, &vLook, D3DXToRadian(pAngle[ANGLE_X]));

	CMathMgr::MyRotationY(&vRight, &vRight, D3DXToRadian(pAngle[ANGLE_Y]));
	CMathMgr::MyRotationY(&vUp, &vUp, D3DXToRadian(pAngle[ANGLE_Y]));
	CMathMgr::MyRotationY(&vLook, &vLook, D3DXToRadian(pAngle[ANGLE_Y]));

	CMathMgr::MyRotationZ(&vRight, &vRight, D3DXToRadian(pAngle[ANGLE_Z]));
	CMathMgr::MyRotationZ(&vUp, &vUp, D3DXToRadian(pAngle[ANGLE_Z]));
	CMathMgr::MyRotationZ(&vLook, &vLook, D3DXToRadian(pAngle[ANGLE_Z]));

	MakeTransformMatrix(pOut, vRight, vUp, vLook, vPos);
}

void CPipeLine::MakeViewMatrix(
	D3DXMATRIX* pOut, 
	const D3DXVECTOR3& vEye, 
	const D3DXVECTOR3& vAt, 
	const D3DXVECTOR3& vUp)
{
	D3DXVECTOR3 vR = {};
	D3DXVECTOR3 vU = {};
	D3DXVECTOR3 vL = vAt - vEye;

	D3DXVec3Cross(&vR, &vUp, &vL);
	D3DXVec3Cross(&vU, &vL, &vR);

	D3DXVec3Normalize(&vR, &vR);
	D3DXVec3Normalize(&vU, &vU);
	D3DXVec3Normalize(&vL, &vL);

	// 카메라의 월드행렬
	MakeTransformMatrix(pOut, vR, vU, vL, vEye);

	// 뷰행렬 == 카메라의 역행렬
	D3DXMatrixInverse(pOut, nullptr, pOut); // 역행렬 만드는 함수.
}

void CPipeLine::MakeProjectionMatrix(
	D3DXMATRIX* pOut, 
	const float& fFovY, 
	const float& fAspect, 
	const float& fNear, 
	const float& fFar)
{
	D3DXMatrixIdentity(pOut);

	// 투영 스페이스: (-1, -1, 0) ~ (1, 1, 1)

	// 우리가 설정한 시야각에 맞게 뷰스페이스 물체들이 투영 스페이스 범위 안으로 들어오게 하기 위함.
	float fScale = 1.f / tanf(fFovY / 2.f);

	pOut->_11 = fScale / fAspect; // 뷰포트로 변환했을 때 왜곡현상을 방지하기 위해 fAspect를 나눈다.
	pOut->_22 = fScale;

	// near ~ far를 0 ~ far 범위로 변환. 나중에 Z나누기했을 때 0 ~ 1 범위가 된다.
	pOut->_33 = fFar / (fFar - fNear);
	pOut->_43 = -(fFar * fNear) / (fFar - fNear);

	// 나중에 Z나누기를 하기 위해 기존 Z 값을 보존한다.
	pOut->_34 = 1.f;
	pOut->_44 = 0.f;
}

void CPipeLine::MakeTransformMatrix(
	D3DXMATRIX* pOut, 
	const D3DXVECTOR3& vRight, 
	const D3DXVECTOR3& vUp, 
	const D3DXVECTOR3& vLook, 
	const D3DXVECTOR3& vPos)
{
	D3DXMatrixIdentity(pOut);

	memcpy(&pOut->m[0][0], &vRight, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[1][0], &vUp, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[2][0], &vLook, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[3][0], &vPos, sizeof(D3DXVECTOR3));
}

void CPipeLine::MyTransformCoord(
	D3DXVECTOR3* pOut, 
	const D3DXVECTOR3* pIn, 
	const D3DXMATRIX* pMatrix)
{
	D3DXVECTOR4 vTemp = { pIn->x, pIn->y, pIn->z, 1.f };

	pOut->x = vTemp.x * pMatrix->_11 + vTemp.y * pMatrix->_21 
		+ vTemp.z * pMatrix->_31 + vTemp.w * pMatrix->_41;
	pOut->y = vTemp.x * pMatrix->_12 + vTemp.y * pMatrix->_22
		+ vTemp.z * pMatrix->_32 + vTemp.w * pMatrix->_42;
	pOut->z = vTemp.x * pMatrix->_13 + vTemp.y * pMatrix->_23
		+ vTemp.z * pMatrix->_33 + vTemp.w * pMatrix->_43;

	// Z나누기
	float fZ = vTemp.x * pMatrix->_14 + vTemp.y * pMatrix->_24
		+ vTemp.z * pMatrix->_34 + vTemp.w * pMatrix->_44;

	(*pOut) /= fZ;
}
