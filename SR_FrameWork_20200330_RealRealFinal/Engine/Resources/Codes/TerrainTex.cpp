#include "TerrainTex.h"

USING(ENGINE)

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


CTerrainTex::~CTerrainTex()
{
	Release();
}

void CTerrainTex::Render(WORD wIndex/* = 0*/)
{
	CVIBuffer::Render();
}

HRESULT CTerrainTex::CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv/* = 1*/)
{
	m_dwVtxSize = sizeof(VTX_TEX);
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_dwVtxCount = wCntX * wCntZ;
	m_dwTriCount = (wCntX - 1) * (wCntZ - 1) * 2;

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTX_TEX* pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 로컬 스페이스 
	WORD wIndex = 0;
	float fTerrainY = 0.f;

	for (WORD i = 0; i < wCntZ; ++i)
	{
		for (WORD j = 0; j < wCntX; ++j)
		{

			wIndex = i * wCntX + j;

			pVtxTex[wIndex].vPos = { float(j * wItv), 0.f, float(i * wItv) };
			pVtxTex[wIndex].vTex.x = (j & 1) ? 1.f : 0.f;
			pVtxTex[wIndex].vTex.y = (i & 1) ? 1.f : 0.f;
			//{ pVtxTex[wIndex].vPos.x / (wCntX - 1.f), 1.f - pVtxTex[wIndex].vPos.z / (wCntZ - 1.f) };
		}
	}

	// Origin Vtx
	m_pOriginVtx = new VTX_TEX[m_dwVtxCount];
	memcpy(m_pOriginVtx, pVtxTex, m_dwVtxSize * m_dwVtxCount);

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

void CTerrainTex::Release()
{
}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv/* = 1*/)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(wCntX, wCntZ, wItv)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
