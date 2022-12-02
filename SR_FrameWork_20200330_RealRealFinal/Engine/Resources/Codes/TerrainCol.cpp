#include "TerrainCol.h"

USING(ENGINE)

CTerrainCol::CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


CTerrainCol::~CTerrainCol()
{
	Release();
}

void CTerrainCol::Render(WORD wIndex/* = 0*/)
{
	CVIBuffer::Render();
}

HRESULT CTerrainCol::CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv/* = 1*/)
{
	m_dwVtxSize = sizeof(VTX_COL);
	m_dwVtxFVF = VTXFVF_COL;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_dwVtxCount = wCntX * wCntZ;
	m_dwTriCount = (wCntX - 1) * (wCntZ - 1) * 2;

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTX_COL* pVtxCol = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	// 로컬 스페이스 
	WORD wIndex = 0;

	for (WORD i = 0; i < wCntZ; ++i)
	{
		for (WORD j = 0; j < wCntX; ++j)
		{
			wIndex = i * wCntX + j;

			pVtxCol[wIndex].vPos = { float(j * wItv), 0.f, float(i * wItv) };
			pVtxCol[wIndex].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	// Origin Vtx
	m_pOriginVtx = new VTX_COL[m_dwVtxCount];
	memcpy(m_pOriginVtx, pVtxCol, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	WORD wTriIndex = 0;

	for (WORD i = 0; i < wCntZ - 1; ++i)
	{
		for (WORD j = 0; j < wCntX - 1; ++j)
		{
			wIndex = i * wCntX + j;

			/*
			0	1
				2
			*/
			pIndex[wTriIndex]._1 = wIndex + wCntX;
			pIndex[wTriIndex]._2 = wIndex + wCntX + 1;
			pIndex[wTriIndex++]._3 = wIndex + 1;

			/*
			0
			3	2
			*/
			pIndex[wTriIndex]._1 = wIndex + wCntX;
			pIndex[wTriIndex]._2 = wIndex + 1;
			pIndex[wTriIndex++]._3 = wIndex;
		}
	}

	

	m_pIB->Unlock();

	return S_OK;
}

void CTerrainCol::Release()
{
}

CTerrainCol* CTerrainCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv/* = 1*/)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CTerrainCol* pInstance = new CTerrainCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(wCntX, wCntZ, wItv)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
