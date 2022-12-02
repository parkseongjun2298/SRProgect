#pragma once

#ifndef __ENGINE_STRUCT_H__

BEGIN(ENGINE)

typedef struct tagVertexColor
{
	D3DXVECTOR3 vPos;
	DWORD dwColor;
}VTX_COL;

const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertexTexture
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR2 vTex; // UV좌표. 텍스처 한장의 UV좌표는 0, 0 ~ 1, 1 범위를 갖는다.
}VTX_TEX;

const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;

typedef struct tagVertexCubeTexture
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vTex; 
}VTX_CUBETEX;

const DWORD VTXFVF_CUBETEX = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagIndex16
{
	WORD _1, _2, _3;
}INDEX16;

typedef struct tagIndex32
{
	DWORD _1, _2, _3;
}INDEX32;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld;
}INFO;

typedef struct tagSaveInfo
{
	int			iRideTag;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	float		fAngleY;
	float		fSize;
	TCHAR		tcTag[256];
	int			iIdx;
}SAVE_INFO;

typedef struct tagRailInfo
{
	bool		bExist;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vUp;
	D3DXVECTOR3 vSize;
}RAIL_INFO;

typedef struct tagRollerInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	float		fAngleY;
	RAIL_INFO	tRailInfo[1000];
}ROLLER_INFO;

END

#define __ENGINE_STRUCT_H__
#endif