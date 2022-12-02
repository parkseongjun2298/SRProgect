#include "stdafx.h"
#include "UiGroup.h"
#include "UiPlate.h"
#include "LineMgr.h"
#include "Transform.h"
#include "Mouse.h"

#define BUILDINGUI_ITV		15.f
#define BUILDINGUI_WIDTH	148.f
#define BUILDINGUI_HEIGHT	102.f

CUiGroup::CUiGroup(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pKeyMgr(ENGINE::CKeyMgr::GetInstance())
	, m_pTimeMgr(ENGINE::CTimeMgr::GetInstance())
	, m_pResourceMgr(ENGINE::CResourceMgr::GetInstance())
	, m_pUiMgr(CUiMgr::GetInstance())
	, m_iBuildingIndex(-1)
{
}


CUiGroup::~CUiGroup()
{
	Release();
}

HRESULT CUiGroup::Initialize()
{
	m_vecUiPlate.resize(12);
	m_vecCollisionBox.resize(12);



	switch (m_eUiType)
	{
	case ENGINE::BUILDING_TILE:
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				int iIndex = 4 * i + j;

				wstring wstrTag = L"Texture_Tile";
				m_vecUiPlate[iIndex] = CUiPlate::Create(m_pGraphicDev, 
				{ 20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j - WINCX / 2.f,
					WINCY / 2.f - (64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i), 0.f },
					{ BUILDINGUI_WIDTH, BUILDINGUI_HEIGHT, 1.f }, 
					wstrTag, iIndex);

				m_vecCollisionBox[iIndex].left = (int)(20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j);
				m_vecCollisionBox[iIndex].top = (int)(64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i);
				m_vecCollisionBox[iIndex].right = m_vecCollisionBox[iIndex].left + (int)BUILDINGUI_WIDTH;
				m_vecCollisionBox[iIndex].bottom = m_vecCollisionBox[iIndex].top + (int)BUILDINGUI_HEIGHT;
				//cout << m_vecCollisionBox[iIndex].right - m_vecCollisionBox[iIndex].left << endl;
			}
		}
		break;
	case ENGINE::BUILDING_RIDES:
		for(int i = 0; i < 3; ++i)
			for (int j = 0; j < 4; ++j)
			{
				int iIndex = 4 * i + j;
				m_vecCollisionBox[iIndex].left = (int)(20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j);
				m_vecCollisionBox[iIndex].top = (int)(64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i);
				m_vecCollisionBox[iIndex].right = m_vecCollisionBox[iIndex].left + (int)BUILDINGUI_WIDTH;
				m_vecCollisionBox[iIndex].bottom = m_vecCollisionBox[iIndex].top + (int)BUILDINGUI_HEIGHT;
				wstring wstrTag = L"";
				switch (iIndex)
				{
				case 0:
					wstrTag = L"Texture_VikingUi";
					break;
				case 1:
					wstrTag = L"Texture_SpinHorseUi";
					break;
				case 2:
					wstrTag = L"Texture_GyroDropUi";
					break;
				case 3:
					wstrTag = L"Texture_RollerCoasterUi";
					break;
				case 4:
					wstrTag = L"Texture_TeaCupUi";
					break;
				case 5:
					wstrTag = L"Texture_FerrisUi";
					break;
				case 6:
					wstrTag = L"Texture_GyroSwingUi";
					break;
				default:
					break;
				}
				m_vecUiPlate[iIndex] = CUiPlate::Create(m_pGraphicDev,
				{ 20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j - WINCX / 2.f,
					WINCY / 2.f - (64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i), 0.f },
					{ BUILDINGUI_WIDTH, BUILDINGUI_HEIGHT, 1.f },
					wstrTag, 0);
			}
		break;
	case ENGINE::BUILDING_COMFY:
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 4; ++j)
			{
				int iIndex = 4 * i + j;
				m_vecCollisionBox[iIndex].left = (int)(20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j);
				m_vecCollisionBox[iIndex].top = (int)(64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i);
				m_vecCollisionBox[iIndex].right = m_vecCollisionBox[iIndex].left + (int)BUILDINGUI_WIDTH;
				m_vecCollisionBox[iIndex].bottom = m_vecCollisionBox[iIndex].top + (int)BUILDINGUI_HEIGHT;
				wstring wstrTag = L"";
				switch (iIndex)
				{
				case 0:
					wstrTag = L"Texture_ShopUi";
					break;
				case 1:
					wstrTag = L"Texture_CottonCandyShopUi";
					break;
				case 2:
					wstrTag = L"Texture_ChurrosShopUi";
					break;
				default:
					break;
				}
				m_vecUiPlate[iIndex] = CUiPlate::Create(m_pGraphicDev,
				{ 20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j - WINCX / 2.f,
					WINCY / 2.f - (64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i), 0.f },
					{ BUILDINGUI_WIDTH, BUILDINGUI_HEIGHT, 1.f },
					wstrTag, 0);
			}
		break;
	case ENGINE::BUILDING_DESIGN:
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 4; ++j)
			{
				int iIndex = 4 * i + j;
				m_vecCollisionBox[iIndex].left = (int)(20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j);
				m_vecCollisionBox[iIndex].top = (int)(64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i);
				m_vecCollisionBox[iIndex].right = m_vecCollisionBox[iIndex].left + (int)BUILDINGUI_WIDTH;
				m_vecCollisionBox[iIndex].bottom = m_vecCollisionBox[iIndex].top + (int)BUILDINGUI_HEIGHT;
				wstring wstrTag = L"";
				switch (iIndex)
				{
				case 0:
					wstrTag = L"Texture_PinkFlowerUi";
					break;
				case 1:
					wstrTag = L"Texture_YellowFlowerUi";
					break;
				case 2:
					wstrTag = L"Texture_RedFlowerUi";
					break;
				default:
					break;
				}
				m_vecUiPlate[iIndex] = CUiPlate::Create(m_pGraphicDev,
				{ 20.f + (BUILDINGUI_ITV + BUILDINGUI_WIDTH) * j - WINCX / 2.f,
					WINCY / 2.f - (64.f + (BUILDINGUI_ITV + BUILDINGUI_HEIGHT) * i), 0.f },
					{ BUILDINGUI_WIDTH, BUILDINGUI_HEIGHT, 1.f },
					wstrTag, 0);
			}
		break;
	default:
		break;
	}
	return S_OK;
}

void CUiGroup::Release()
{
	for (auto& Plate : m_vecUiPlate)
		ENGINE::SafeDelete(Plate);
	m_vecUiPlate.clear();
}

int CUiGroup::Update(wstring* pwstrSelect, int* piSelect)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	for (auto& Plate : m_vecUiPlate)
		if(Plate != nullptr)
			Plate->Update();

	for (int i = 0; i < 12; ++i)
	{
		if (PtInRect(&m_vecCollisionBox[i], pt))
		{
			if (m_pKeyMgr->KeyDown(ENGINE::KEY_LBUTTON))
			{
				if (m_vecUiPlate[i] != nullptr)
				{
					m_wstrBuilding = m_vecUiPlate[i]->GetwstrTag();
					*pwstrSelect = m_wstrBuilding;
					m_iBuildingIndex = m_vecUiPlate[i]->GetIndex();
					*piSelect = m_iBuildingIndex;
					//CMouse::GetInstance()->CreateMouse(m_wstrBuilding, m_iBuildingIndex);
				}
				else
				{
					m_wstrBuilding = L"Delete";
					*pwstrSelect = L"Delete";
					*piSelect = 0;
				}
				//cout << *m_wstrBuilding.c_str() << endl;
			}
		}
	}
	return NO_EVENT;
}

void CUiGroup::LateUpdate()
{
	for (auto& Plate : m_vecUiPlate)
		if (Plate != nullptr)
			Plate->LateUpdate();
}

void CUiGroup::Render()
{
	for (int i = 0; i < 12; ++i)
		if (m_vecUiPlate[i] != nullptr)
		{
			m_vecUiPlate[i]->Render();
			if (m_vecUiPlate[i]->GetwstrTag() == m_wstrBuilding && m_vecUiPlate[i]->GetIndex() == m_iBuildingIndex)
			{
				RECT rc = {
					m_vecCollisionBox[i].left + (LONG)m_pUiMgr->GetTransform()->GetPos().x,
					m_vecCollisionBox[i].top - (LONG)m_pUiMgr->GetTransform()->GetPos().y,
					m_vecCollisionBox[i].right + (LONG)m_pUiMgr->GetTransform()->GetPos().x,
					m_vecCollisionBox[i].bottom - (LONG)m_pUiMgr->GetTransform()->GetPos().y
				};
				CLineMgr::DrawBoxNoTransform(rc, 0xff00ff00, 5.f);
			}

		}
}

CUiGroup * CUiGroup::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::BUILDING_UI_TYPE eType)
{
	CUiGroup* pInstance = new CUiGroup(pGraphicDev);
	pInstance->m_eUiType = eType;
	if (FAILED(pInstance->Initialize()))
	{
		ENGINE::SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}
