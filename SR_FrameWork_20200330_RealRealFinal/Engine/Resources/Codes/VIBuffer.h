#pragma once

#ifndef __VIBUFFER_H__

#include "Resources.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CVIBuffer();

public:
	void* GetOriginVtx() const;

public:
	void SetConvertVtx(void* pConvertVtx);

public:
	virtual CResources* Clone() override;
	virtual void Render(WORD wIndex = 0) override;

protected:
	virtual HRESULT CreateBuffer();
	virtual void Release() override;

protected:
	// IDirect3DVertexBuffer9: 버텍스를 배열 형식으로 관리하는 COM객체.
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	// IDirect3DIndexBuffer9: 인덱스를 배열 형식으로 관리하는 COM객체.
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	DWORD		m_dwVtxSize;	// 버텍스 자료형 크기
	DWORD		m_dwVtxFVF;		// FVF(Flexible Vertex Format, 유연한 정점 포맷)

	DWORD		m_dwVtxCount;	// 정점 개수
	DWORD		m_dwTriCount;	// 삼각형 개수

	DWORD		m_dwIdxSize;	// 인덱스 자료형 크기
	D3DFORMAT	m_IdxFmt;		// 인덱스 포맷 (16비트 or 32비트)

	void*		m_pOriginVtx;
	
};

END

#define __VIBUFFER_H__
#endif