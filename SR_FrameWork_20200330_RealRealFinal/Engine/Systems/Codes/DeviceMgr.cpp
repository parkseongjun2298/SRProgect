#include "DeviceMgr.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
{
}


CDeviceMgr::~CDeviceMgr()
{
	Release();
}

LPDIRECT3DDEVICE9 CDeviceMgr::GetDevice() const
{
	return m_pGraphicDev;
}

LPD3DXSPRITE CDeviceMgr::GetSprite() const
{
	return m_pSprite;
}

LPD3DXFONT CDeviceMgr::GetFont() const
{
	return m_pFont;
}

LPD3DXLINE CDeviceMgr::GetLine() const
{
	return m_pLine;
}

HRESULT CDeviceMgr::InitDevice(HWND hWnd, const WORD& wCX, const WORD& wCY, DISPLAY_MODE eMode)
{
	// ��ġ �ʱ�ȭ ����.
	// 1. SDK ��ü ���� (IDirect3D9)
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_MSG_RETURN(m_pSDK, L"Direct3DCreate9 Failed", E_FAIL);

	// 2. SDK ��ü�� ���� ���� ����� ��ġ�� ����.
	D3DCAPS9 d3dcaps; // ��ġ ����
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// HAL(Hardware Abstraction Layer, �ϵ���� �߻� ����)
	// ���� ��ġ ������ HAL���� ���� �Լ�.
	HRESULT hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 2-1. ������ �� ��ġ ������ ���� ���� ��ġ�� "���ؽ� ���μ���"�� �����ϴ��� Ȯ��.
	// ���ؽ� ���μ���: ���� ��ȯ + ���� ����

	DWORD vp = 0; // ��ġ �ʱ�ȭ �ɼ�

				  // ��ġ�� ���ؽ� ���μ����� �����ϴ���?
	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING; // �����Ѵٸ� ��ġ�� ���ؽ� ���μ����� ���.
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	// 3. SDK ��ü�� ���� ��ġ ��ü(IDirect3DDevice9) ����.
	D3DPRESENT_PARAMETERS d3dpp; // ������ ȯ�� ����.
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = wCX;	// �ĸ���� ����ũ��
	d3dpp.BackBufferHeight = wCY;	// �ĸ���� ����ũ��
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // �ĸ���� �ȼ�����
	d3dpp.BackBufferCount = 1; // �ĸ���� ����

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // ��Ƽ ���ø�
	d3dpp.MultiSampleQuality = 0;	// ǰ��

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ����ü�� ����� ����Ѵ�.
	d3dpp.hDeviceWindow = hWnd; // �⺻ ��� ���.
	d3dpp.Windowed = eMode; // TRUE: â ���, FALSE: ��üȭ�� ���
	d3dpp.EnableAutoDepthStencil = TRUE; // ��ġ�� ���̹���(Z����)�� ���ٽǹ��۸� �ڵ����� �����Ѵ�.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ��üȭ���� �� ����� �ֻ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, vp, &d3dpp, &m_pGraphicDev);
	FAILED_CHECK_MSG_RETURN(hr, L"CreateDevice Failed", E_FAIL);	

	D3DXFONT_DESC	tFontInfo;	// ��Ʈ ����
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 20;
	tFontInfo.Height = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ձٸ��"); // ��ü

	hr = D3DXCreateSprite(m_pGraphicDev, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	hr = D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	hr = D3DXCreateLine(m_pGraphicDev, &m_pLine);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateLine Failed", E_FAIL);

	m_pLine->SetWidth(2.f);

	return S_OK;
}

void CDeviceMgr::DrawFont(D3DXMATRIX matTrans, TCHAR* strOut, DWORD dwARGB)
{
	m_pSprite->SetTransform(&matTrans);

	// ���̷�Ʈ ��Ʈ�� ���ڿ� ����� ������ �� �� �´� �ؽ�ó�� ���� ����Ѵ�. �̳� ������.
	m_pFont->DrawText(
		m_pSprite,
		strOut, /* ����� ���ڿ� */
		lstrlen(strOut), /* ���� */
		nullptr, /* RECT ������ */
		0,
		dwARGB);
}

void CDeviceMgr::Release()
{	
	SafeRelease(m_pLine);
	SafeRelease(m_pFont);
	SafeRelease(m_pSprite);
	SafeRelease(m_pGraphicDev);
	SafeRelease(m_pSDK);
}