#include "CubeCol.h"

USING(ENGINE)

CCubeCol::CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


CCubeCol::~CCubeCol()
{
	Release();
}

void CCubeCol::Render(WORD wIndex/* = 0*/)
{
	CVIBuffer::Render();
}

HRESULT CCubeCol::CreateBuffer(D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, DWORD dwColor, PIVOT_TYPE eType)
{
	m_dwVtxSize = sizeof(VTX_COL);
	m_dwVtxFVF = VTXFVF_COL;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	m_dwVtxCount = 8;
	m_dwTriCount = 12;

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTX_COL* pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 로컬 스페이스 

	// 0	1
	// 3	2
	/*
	-+-
	++-
	+--
	---
	-++
	+++
	+-+
	--+
	*/
	switch (eType)
	{
	case ENGINE::PIVOT_BOTTOM:
		pVtxTex[0].vPos = { vPos.x - vSize.x * 0.5f, vPos.y + vSize.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[1].vPos = { vPos.x + vSize.x * 0.5f, vPos.y + vSize.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[2].vPos = { vPos.x + vSize.x * 0.5f, vPos.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[3].vPos = { vPos.x - vSize.x * 0.5f, vPos.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[4].vPos = { vPos.x - vSize.x * 0.5f, vPos.y + vSize.y, vPos.z + vSize.z * 0.5f };
		pVtxTex[5].vPos = { vPos.x + vSize.x * 0.5f, vPos.y + vSize.y, vPos.z + vSize.z * 0.5f };
		pVtxTex[6].vPos = { vPos.x + vSize.x * 0.5f, vPos.y, vPos.z + vSize.z * 0.5f };
		pVtxTex[7].vPos = { vPos.x - vSize.x * 0.5f, vPos.y, vPos.z + vSize.z * 0.5f };
		break;
	case ENGINE::PIVOT_MIDDLE:
		pVtxTex[0].vPos = { vPos.x - vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z - vSize.z * 0.5f };
		pVtxTex[1].vPos = { vPos.x + vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z - vSize.z * 0.5f };
		pVtxTex[2].vPos = { vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z - vSize.z * 0.5f };
		pVtxTex[3].vPos = { vPos.x - vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z - vSize.z * 0.5f };
		pVtxTex[4].vPos = { vPos.x - vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z + vSize.z * 0.5f };
		pVtxTex[5].vPos = { vPos.x + vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z + vSize.z * 0.5f };
		pVtxTex[6].vPos = { vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z + vSize.z * 0.5f };
		pVtxTex[7].vPos = { vPos.x - vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z + vSize.z * 0.5f };
		break;
	case ENGINE::PIVOT_TOP:
		pVtxTex[0].vPos = { vPos.x - vSize.x * 0.5f, vPos.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[1].vPos = { vPos.x + vSize.x * 0.5f, vPos.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[2].vPos = { vPos.x + vSize.x * 0.5f, vPos.y - vSize.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[3].vPos = { vPos.x - vSize.x * 0.5f, vPos.y - vSize.y, vPos.z - vSize.z * 0.5f };
		pVtxTex[4].vPos = { vPos.x - vSize.x * 0.5f, vPos.y, vPos.z + vSize.z * 0.5f };
		pVtxTex[5].vPos = { vPos.x + vSize.x * 0.5f, vPos.y, vPos.z + vSize.z * 0.5f };
		pVtxTex[6].vPos = { vPos.x + vSize.x * 0.5f, vPos.y - vSize.y, vPos.z + vSize.z * 0.5f };
		pVtxTex[7].vPos = { vPos.x - vSize.x * 0.5f, vPos.y - vSize.y, vPos.z + vSize.z * 0.5f };
		break;
	case ENGINE::PIVOT_FRONT:
		pVtxTex[0].vPos = { vPos.x - vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z};
		pVtxTex[1].vPos = { vPos.x + vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z};
		pVtxTex[2].vPos = { vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z};
		pVtxTex[3].vPos = { vPos.x - vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z};
		pVtxTex[4].vPos = { vPos.x - vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z + vSize.z};
		pVtxTex[5].vPos = { vPos.x + vSize.x * 0.5f, vPos.y + vSize.y * 0.5f, vPos.z + vSize.z};
		pVtxTex[6].vPos = { vPos.x + vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z + vSize.z};
		pVtxTex[7].vPos = { vPos.x - vSize.x * 0.5f, vPos.y - vSize.y * 0.5f, vPos.z + vSize.z};
	default:
		break;
	}
	

	pVtxTex[0].dwColor = dwColor;
	pVtxTex[1].dwColor = dwColor;
	pVtxTex[2].dwColor = dwColor;
	pVtxTex[3].dwColor = dwColor;
	pVtxTex[4].dwColor = dwColor;
	pVtxTex[5].dwColor = dwColor;
	pVtxTex[6].dwColor = dwColor;
	pVtxTex[7].dwColor = dwColor;

	// Origin Vtx
	m_pOriginVtx = new VTX_COL[m_dwVtxCount];
	memcpy(m_pOriginVtx, pVtxTex, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	
	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;
	
	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 6;
	pIndex[6]._2 = 7;
	pIndex[6]._3 = 3;

	pIndex[7]._1 = 6;
	pIndex[7]._2 = 3;
	pIndex[7]._3 = 2;

	// +z
	pIndex[8]._1 = 0;
	pIndex[8]._2 = 1;
	pIndex[8]._3 = 2;

	pIndex[9]._1 = 0;
	pIndex[9]._2 = 2;
	pIndex[9]._3 = 3;

	// -z
	pIndex[10]._1 = 5;
	pIndex[10]._2 = 4;
	pIndex[10]._3 = 7;

	pIndex[11]._1 = 5;
	pIndex[11]._2 = 7;
	pIndex[11]._3 = 6;

	m_pIB->Unlock();

	return S_OK;
}

void CCubeCol::Release()
{
}

CCubeCol* CCubeCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, DWORD dwColor, PIVOT_TYPE eType)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CCubeCol* pInstance = new CCubeCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(vPos, vSize, dwColor, eType)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
