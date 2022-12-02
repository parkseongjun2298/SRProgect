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
	// IDirect3DVertexBuffer9: ���ؽ��� �迭 �������� �����ϴ� COM��ü.
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	// IDirect3DIndexBuffer9: �ε����� �迭 �������� �����ϴ� COM��ü.
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	DWORD		m_dwVtxSize;	// ���ؽ� �ڷ��� ũ��
	DWORD		m_dwVtxFVF;		// FVF(Flexible Vertex Format, ������ ���� ����)

	DWORD		m_dwVtxCount;	// ���� ����
	DWORD		m_dwTriCount;	// �ﰢ�� ����

	DWORD		m_dwIdxSize;	// �ε��� �ڷ��� ũ��
	D3DFORMAT	m_IdxFmt;		// �ε��� ���� (16��Ʈ or 32��Ʈ)

	void*		m_pOriginVtx;
	
};

END

#define __VIBUFFER_H__
#endif