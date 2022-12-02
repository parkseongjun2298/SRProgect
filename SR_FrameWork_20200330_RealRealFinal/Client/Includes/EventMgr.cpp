#include "stdafx.h"
#include "EventMgr.h"
#include "UiTexture.h"
#include "UiNpc.h"

IMPLEMENT_SINGLETON(CEventMgr)

CEventMgr::CEventMgr()
	:m_vecbQuest(vector<bool>(1, false)),
	m_iCurQuest(0),
	m_pKeyMgr(ENGINE::CKeyMgr::GetInstance())
{
}


CEventMgr::~CEventMgr()
{
	Release();
}

int CEventMgr::Update()
{
	if (m_pTexture != nullptr)
	{
		if (DEAD_OBJ == m_pTexture->Update())
			ENGINE::SafeDelete(m_pTexture);
		m_pNpc->Update();
	}
	
	//Ư�� �̺�Ʈ �߻� �� ���� �� �߰�
	if (m_vecbQuest[m_iCurQuest] == true && m_iCurQuest + 1 >= m_vecbQuest.size())
	{
		if (m_pTexture != nullptr)
			ENGINE::SafeDelete(m_pTexture);
		vector<wstring> vecTalk;
		switch (m_iCurQuest)
		{
		case 0:
			if (m_mapQuest[QUEST_CONDITION::PUSH_TAB])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"���ϼ̾��. ���� Ÿ���� ��ƺ���.");
				vecTalk.push_back(L"���߳�. ���� Ÿ���� ��ƺ���.");
				//vecTalk.push_back(L"���ϴ� Ÿ���� Ŭ���� �� TABŰ�� ����\n\n������ �� �ٴڿ� ��ƺ�����.");
				vecTalk.push_back(L"���ϴ� Ÿ���� Ŭ���� �� TABŰ�� ����\n\n������ �� �ٴڿ� ��ƺ��Գ�.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 1:
			if (m_mapQuest[QUEST_CONDITION::TILE_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"���ϼ̾��. �׷��� ���� ���̱ⱸ��\n\n��ġ�غ����?");
				vecTalk.push_back(L"�����ֱ�. �׷��� ���� ���̱ⱸ��\n\n��ġ�غ��ڳ�?");
				//vecTalk.push_back(L"��/��Ű�� ���� ���� �ű� �� �־��.\n\n���� ȸ���񸶸� ���ƺ���.");
				vecTalk.push_back(L"��/��Ű�� ���� ���� �ű� �� �ִٳ�.\n\n���� ȸ���񸶸� ���ƺ��ð�.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 2:
			if (m_mapQuest[QUEST_CONDITION::SPINHORSE_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"���� ���̱ⱸ�� ��ƺ���.\n\n�̹��� ����ŷ�� ��ġ�غ���.");
				vecTalk.push_back(L"���� ���̱ⱸ�� ��ƺ��ڱ�.\n\n�̹��� ����ŷ�� ��ġ�غ���������.");
				//vecTalk.push_back(L"�Ʊ�� ���������� ��ġ�ϸ� �ſ�.");
				vecTalk.push_back(L"�Ʊ�� ���������� ��ġ�ϸ� �ȴٳ�.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 3:
			if (m_mapQuest[QUEST_CONDITION::VIKING_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"���߿��� �� �������� �̷� ����ϱ��?");
				vecTalk.push_back(L"���߿��� �� �������� �̷� ����̰ڱ�?");
				//vecTalk.push_back(L"�̹��� ���̷ε���� ��ġ�غ� ���ʿ���.");
				vecTalk.push_back(L"�̹��� ���̷ε���� ��ġ�غ� ���ʶ��.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 4:
			if (m_mapQuest[QUEST_CONDITION::GYRODROP_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"���ϼ̽��ϴ�. ���� �ӵ��� \n\n���ۺ��� ���ư��� �� �����غ���!");
				vecTalk.push_back(L"�Ǹ��ϳ�. ���� �ӵ��� \n\n���ۺ��� ���ư��� �� �����غ��ڱ�!");
				//vecTalk.push_back(L"�̹��� �ѷ��ڽ��͸� ��ġ�غ���.");
				vecTalk.push_back(L"�̹��� �ѷ��ڽ��͸� ��ġ�غ��ð�.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 5:
			if (m_mapQuest[QUEST_CONDITION::ROLLERCOASTER_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"�Ǹ��ؿ�. Ÿ���� ���� \n\n�ﷷ�Ÿ� ȸ�� ���� ���ʿ���!");
				vecTalk.push_back(L"�Ϻ��ϳ�. Ÿ���� ���� \n\n�ﷷ�Ÿ� ȸ�� ���� ���ʶ��!");
				//vecTalk.push_back(L"ȸ�������� ��ġ�غ����?");
				vecTalk.push_back(L"ȸ�������� ��ġ�غ��ڳ�?");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 6:
			if (m_mapQuest[QUEST_CONDITION::SPINHORSE_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"���� �������Ű���? ���θ�����. \n\n���� ���̱ⱸ�� �����־��!");
				vecTalk.push_back(L"���� ��������? ���θ���. \n\n���� ���̱ⱸ�� �����ִٳ�!");
				//vecTalk.push_back(L"���̰����� �Ѵ��� ����ʹٱ���? \n\n�׷� �������� ��ġ�غ���.");
				vecTalk.push_back(L"���̰����� �Ѵ��� ����ʹٰ�? \n\n�׷� �������� ��ġ�غ���.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 7:
			if (m_mapQuest[QUEST_CONDITION::FERRIS_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"�̰����� �⺻���� Ʃ�丮���� �������.");
				vecTalk.push_back(L"�̰����� �⺻���� Ʃ�丮���� ������.");
				//vecTalk.push_back(L"���ݱ��� �������� ������� ���� ��غ�����!");
				vecTalk.push_back(L"���ݱ��� �������� �ڳװ� ���� ��غ��ðԳ�!");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		default:
			break;
		}
	}

	return NO_EVENT;
}

void CEventMgr::LateUpdate()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->LateUpdate();
		m_pNpc->LateUpdate();
	}
}

void CEventMgr::Render()
{
	if (m_pTexture != nullptr)
	{
		m_pNpc->Render();
		m_pTexture->Render();
	}
}

void CEventMgr::Initialize(CPlayer * pPlayer, LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pPlayer = pPlayer;
	m_pGraphicDev = pGraphicDev;
	vector<wstring> vecTalk;
	//vecTalk.push_back(L"�ȳ��ϼ���. ���� �� ���̰��� \n\n��� ������ �θ��θ� �븶���̿���.");
	vecTalk.push_back(L"�ȳ��ϰԳ�. ���� �� ���̰��� \n\n��� ������ �θ��θ� �븶���̶��.");
	//vecTalk.push_back(L"���ݺ���, Ʃ�丮���� �����غ����? \n\n���� TABŰ�� ���� ����â�� �������.");
	vecTalk.push_back(L"���ݺ���, Ʃ�丮���� �����غ��ڳ�? \n\n���� TABŰ�� ���� ����â�� �����.");
	this->CreateUiTexture(vecTalk, vecTalk.size());

	// 130, 163
	m_pNpc = CUiNpc::Create(m_pGraphicDev, { WINCX * 0.5f - 330.f, -30.f, 0.f }, { 130 * 3.f, 163.f * 3.f, 0.f }, L"Texture_UiNpc");
}

void CEventMgr::Release()
{
	ENGINE::SafeDelete(m_pTexture);
	ENGINE::SafeDelete(m_pNpc);
}

void CEventMgr::CreateUiTexture(vector<wstring> vecwstrTalk, size_t iSize)
{
	m_pTexture = CUiTexture::Create(m_pGraphicDev, { -WINCX * 0.5f, 30.f - (64.f * 3.f), 0.f }, { 320 * 3.f, 64 * 3.f, 0.f }, L"Texture_UiTalk");
	for (size_t i = 0; i < iSize; ++i)
		m_pTexture->SetTexture(vecwstrTalk[i]);
}
