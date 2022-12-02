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
	// 프로토타입 디자인 패턴
	// 오브젝트 생성할 때마다 필요한 리소스를 새로 생성할 때 발생하는 비용을 줄이도록 설계한 디자인 패턴.
	++(*m_pRefCnt);

	return new CVIBuffer(*this); // 복사 생성
}

void CVIBuffer::Render(WORD wIndex/* = 0*/)
{
	// 버텍스 버퍼를 장치에게 전달.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 인덱스 버퍼를 장치에게 전달.
	m_pGraphicDev->SetIndices(m_pIB);

	// 사용자가 구성한 FVF를 장치에게 전달.
	m_pGraphicDev->SetFVF(m_dwVtxFVF);	

	// 장치가 인덱스 버퍼를 이용해서 도형을 그린다.
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
		m_dwVtxCount, 0, m_dwTriCount);
}

HRESULT CVIBuffer::CreateBuffer()
{
	// 버텍스 버퍼 생성 함수.
	HRESULT hr = m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxSize * m_dwVtxCount, /* 버텍스 배열의 전체 메모리 크기 */
		0, /* 0: 정적버퍼, D3DUSAGE_DYNAMIC: 동적버퍼 */
		m_dwVtxFVF, /* FVF */
		D3DPOOL_MANAGED, /* 메모리 보관 방식 */
		&m_pVB,
		nullptr);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	// 인덱스 버퍼 생성
	hr = m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxSize * m_dwTriCount, /* 인덱스 배열의 전체 메모리 크기 */
		0,
		m_IdxFmt, /* 인덱스 포맷 */
		D3DPOOL_MANAGED, /* 메모리 보관 방식 */
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
