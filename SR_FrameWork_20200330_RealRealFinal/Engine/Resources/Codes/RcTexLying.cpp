#include "RcTexLying.h"

USING(ENGINE)

CRcTexLying::CRcTexLying(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


CRcTexLying::~CRcTexLying()
{
	Release();
}

void CRcTexLying::Render(WORD wIndex/* = 0*/)
{
	CVIBuffer::Render();
}

HRESULT CRcTexLying::CreateBuffer(D3DXVECTOR3 vPivotPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType)
{
	m_dwVtxSize = sizeof(VTX_TEX);
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_dwVtxCount = 4;
	m_dwTriCount = 2;

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTX_TEX* pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 로컬 스페이스 

	// 0-+	1++
	// 3--	2+-
	if (eType == ENGINE::PIVOT_FRONT)
	{
		pVtxTex[0].vPos = { vPivotPos.x - vSize.x * 0.5f, vPivotPos.y,	vPivotPos.z + vSize.z };
		pVtxTex[1].vPos = { vPivotPos.x + vSize.x * 0.5f, vPivotPos.y,	vPivotPos.z + vSize.z };
		pVtxTex[2].vPos = { vPivotPos.x + vSize.x * 0.5f, vPivotPos.y,	vPivotPos.z };
		pVtxTex[3].vPos = { vPivotPos.x - vSize.x * 0.5f, vPivotPos.y,	vPivotPos.z };
	}
	else
	{
		pVtxTex[0].vPos = { vPivotPos.x, vPivotPos.y,	vPivotPos.z + vSize.z };
		pVtxTex[1].vPos = { vPivotPos.x + vSize.x, vPivotPos.y,	vPivotPos.z + vSize.z };
		pVtxTex[2].vPos = { vPivotPos.x + vSize.x, vPivotPos.y,	vPivotPos.z };
		pVtxTex[3].vPos = { vPivotPos.x, vPivotPos.y,	vPivotPos.z };
	}

	pVtxTex[0].vTex = { 0.f, 0.f };
	pVtxTex[1].vTex = { 1.f, 0.f };
	pVtxTex[2].vTex = { 1.f, 1.f };
	pVtxTex[3].vTex = { 0.f, 1.f };	

	// Origin Vtx
	m_pOriginVtx = new VTX_TEX[m_dwVtxCount];
	memcpy(m_pOriginVtx, pVtxTex, m_dwVtxSize * m_dwVtxCount);

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

void CRcTexLying::Release()
{
}

CRcTexLying* CRcTexLying::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	D3DXVECTOR3 vPivotPos, D3DXVECTOR3 vSize, PIVOT_TYPE eType)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CRcTexLying* pInstance = new CRcTexLying(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(vPivotPos, vSize, eType)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}	

	return pInstance;
}
