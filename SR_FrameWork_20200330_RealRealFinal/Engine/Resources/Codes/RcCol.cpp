#include "RcCol.h"

USING(ENGINE)

CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


CRcCol::~CRcCol()
{
	Release();
}

void CRcCol::Render(WORD wIndex/* = 0*/)
{
	CVIBuffer::Render();
}

HRESULT CRcCol::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTX_COL);
	m_dwVtxFVF = VTXFVF_COL;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_dwVtxCount = 4;
	m_dwTriCount = 2;

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTX_COL* pVtxCol = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	// 로컬 스페이스 

	// 0	1
	// 3	2

	pVtxCol[0].vPos = { -1.f, 1.f, 0.f };
	pVtxCol[0].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVtxCol[1].vPos = { 1.f, 1.f, 0.f };
	pVtxCol[1].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVtxCol[2].vPos = { 1.f, -1.f, 0.f };
	pVtxCol[2].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);	

	pVtxCol[3].vPos = { -1.f, -1.f, 0.f };
	pVtxCol[3].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	// Origin Vtx
	m_pOriginVtx = new VTX_COL[m_dwVtxCount];
	memcpy(m_pOriginVtx, pVtxCol, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	/*
	0	1
		2
	*/
	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	/*
	0	
	3	2
	*/
	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRcCol::Release()
{
}

CRcCol* CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CRcCol* pInstance = new CRcCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
