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
	
	//특정 이벤트 발생 시 생성 후 추가
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
				//vecTalk.push_back(L"잘하셨어요. 이제 타일을 깔아봐요.");
				vecTalk.push_back(L"잘했네. 이제 타일을 깔아보게.");
				//vecTalk.push_back(L"원하는 타일을 클릭한 뒤 TAB키를 눌러\n\n종료한 후 바닥에 깔아보세요.");
				vecTalk.push_back(L"원하는 타일을 클릭한 뒤 TAB키를 눌러\n\n종료한 후 바닥에 깔아보게나.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 1:
			if (m_mapQuest[QUEST_CONDITION::TILE_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"잘하셨어요. 그러면 이제 놀이기구를\n\n설치해볼까요?");
				vecTalk.push_back(L"소질있군. 그러면 이제 놀이기구를\n\n설치해보겠나?");
				//vecTalk.push_back(L"좌/우키를 통해 탭을 옮길 수 있어요.\n\n먼저 회전목마를 놓아보죠.");
				vecTalk.push_back(L"좌/우키를 통해 탭을 옮길 수 있다네.\n\n먼저 회전목마를 놓아보시게.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 2:
			if (m_mapQuest[QUEST_CONDITION::SPINHORSE_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"다음 놀이기구를 깔아봐요.\n\n이번엔 바이킹을 설치해보죠.");
				vecTalk.push_back(L"다음 놀이기구를 깔아보자구.\n\n이번엔 바이킹을 설치해보도록하지.");
				//vecTalk.push_back(L"아까와 마찬가지로 설치하면 돼요.");
				vecTalk.push_back(L"아까와 마찬가지로 설치하면 된다네.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 3:
			if (m_mapQuest[QUEST_CONDITION::VIKING_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"공중에서 뚝 떨어지면 이런 기분일까요?");
				vecTalk.push_back(L"공중에서 뚝 떨어지면 이런 기분이겠군?");
				//vecTalk.push_back(L"이번엔 자이로드롭을 설치해볼 차례에요.");
				vecTalk.push_back(L"이번엔 자이로드롭을 설치해볼 차례라네.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 4:
			if (m_mapQuest[QUEST_CONDITION::GYRODROP_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"잘하셨습니다. 빠른 속도와 \n\n빙글빙글 돌아가는 걸 경험해봐요!");
				vecTalk.push_back(L"훌륭하네. 빠른 속도와 \n\n빙글빙글 돌아가는 걸 경험해보자구!");
				//vecTalk.push_back(L"이번엔 롤러코스터를 설치해봐요.");
				vecTalk.push_back(L"이번엔 롤러코스터를 설치해보시게.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 5:
			if (m_mapQuest[QUEST_CONDITION::ROLLERCOASTER_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"훌륭해요. 타고나면 속이 \n\n울렁거릴 회전 찻잔 차례에요!");
				vecTalk.push_back(L"완벽하네. 타고나면 속이 \n\n울렁거릴 회전 찻잔 차례라네!");
				//vecTalk.push_back(L"회전찻잔을 설치해볼까요?");
				vecTalk.push_back(L"회전찻잔을 설치해보겠나?");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 6:
			if (m_mapQuest[QUEST_CONDITION::SPINHORSE_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"속은 괜찮으신가요? 서두르세요. \n\n다음 놀이기구가 남아있어요!");
				vecTalk.push_back(L"속은 괜찮은가? 서두르게. \n\n다음 놀이기구가 남아있다네!");
				//vecTalk.push_back(L"놀이공원을 한눈에 보고싶다구요? \n\n그럼 관람차를 설치해보죠.");
				vecTalk.push_back(L"놀이공원을 한눈에 보고싶다고? \n\n그럼 관람차를 설치해보자.");
				this->CreateUiTexture(vecTalk, vecTalk.size());
			}
			break;
		case 7:
			if (m_mapQuest[QUEST_CONDITION::FERRIS_CREATE])
			{
				++m_iCurQuest;
				m_vecbQuest.push_back(false);
				//vecTalk.push_back(L"이것으로 기본적인 튜토리얼은 끝났어요.");
				vecTalk.push_back(L"이것으로 기본적인 튜토리얼은 끝났네.");
				//vecTalk.push_back(L"지금까지 배운것으로 점장님이 직접 운영해보세요!");
				vecTalk.push_back(L"지금까지 배운것으로 자네가 직접 운영해보시게나!");
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
	//vecTalk.push_back(L"안녕하세요. 저는 이 놀이공원 \n\n운영을 도와줄 부리부리 대마왕이에요.");
	vecTalk.push_back(L"안녕하게나. 나는 이 놀이공원 \n\n운영을 도와줄 부리부리 대마왕이라네.");
	//vecTalk.push_back(L"지금부터, 튜토리얼을 시작해볼까요? \n\n먼저 TAB키를 눌러 빌딩창을 열어봐요.");
	vecTalk.push_back(L"지금부터, 튜토리얼을 시작해보겠나? \n\n먼저 TAB키를 눌러 빌딩창을 열어보게.");
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
