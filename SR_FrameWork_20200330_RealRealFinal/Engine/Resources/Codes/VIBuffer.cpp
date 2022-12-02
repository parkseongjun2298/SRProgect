#include "VIBuffer.h"

USING(ENGINE)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev),
	m_pVB(nullptr), m_pIB(nullptr), 
	m_dwVtxSize(0), m_dwVtxFVF(0), 
	m_dwVtxCount(0), m_dwTriCount(0),
	m_dwIdxSize(0), m_IdxFmt(D3DFMT_UNKNOWN),
	m_pOriginVtx(nullptr)
{
}


CVIBuffer::~CVIBuffer()
{
	Release();
}

void* CVIBuffer::GetOriginVtx() const
{
	return m_pOriginVtx;
}

void CVIBuffer::SetConvertVtx(void* pConvertVtx)
{
	NULL_CHECK(pConvertVtx);

	void* pVtx = nullptr;

	m_pVB->Lock(0, 0, &pVtx, 0);

	memcpy(pVtx, pConvertVtx, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();
}

CResources* CVIBuffer::Clone()
{
	// ������Ÿ�� ������ ����
	// ������Ʈ ������ ������ �ʿ��� ���ҽ��� ���� ������ �� �߻��ϴ� ����� ���̵��� ������ ������ ����.
	++(*m_pRefCnt);

	return new CVIBuffer(*this); // ���� ����
}

void CVIBuffer::Render(WORD wIndex/* = 0*/)
{
	// ���ؽ� ���۸� ��ġ���� ����.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// �ε��� ���۸� ��ġ���� ����.
	m_pGraphicDev->SetIndices(m_pIB);

	// ����ڰ� ������ FVF�� ��ġ���� ����.
	m_pGraphicDev->SetFVF(m_dwVtxFVF);	

	// ��ġ�� �ε��� ���۸� �̿��ؼ� ������ �׸���.
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
		m_dwVtxCount, 0, m_dwTriCount);
}

HRESULT CVIBuffer::CreateBuffer()
{
	// ���ؽ� ���� ���� �Լ�.
	HRESULT hr = m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxSize * m_dwVtxCount, /* ���ؽ� �迭�� ��ü �޸� ũ�� */
		0, /* 0: ��������, D3DUSAGE_DYNAMIC: �������� */
		m_dwVtxFVF, /* FVF */
		D3DPOOL_MANAGED, /* �޸� ���� ��� */
		&m_pVB,
		nullptr);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	// �ε��� ���� ����
	hr = m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxSize * m_dwTriCount, /* �ε��� �迭�� ��ü �޸� ũ�� */
		0,
		m_IdxFmt, /* �ε��� ���� */
		D3DPOOL_MANAGED, /* �޸� ���� ��� */
		&m_pIB,
		nullptr);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	return S_OK;
}

void CVIBuffer::Release()
{
	if (0 == --(*m_pRefCnt))
	{
		SafeRelease(m_pVB);
		SafeRelease(m_pIB);
		SafeDeleteArray(m_pOriginVtx);
	}
}
