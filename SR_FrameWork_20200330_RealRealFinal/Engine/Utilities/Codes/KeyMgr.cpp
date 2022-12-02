#include "KeyMgr.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwCurrentKey(0), m_dwKeyDown(0), m_dwKeyUp(0)
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Update()
{
	m_dwCurrentKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurrentKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurrentKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurrentKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurrentKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurrentKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurrentKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurrentKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurrentKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		m_dwCurrentKey |= KEY_NUMPAD2;
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		m_dwCurrentKey |= KEY_NUMPAD8;
	if (GetAsyncKeyState(VK_ADD) & 0x8000)
		m_dwCurrentKey |= KEY_ADD;
	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
		m_dwCurrentKey |= KEY_MIN;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurrentKey |= KEY_W;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurrentKey |= KEY_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurrentKey |= KEY_S;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurrentKey |= KEY_D;

	if (GetAsyncKeyState(VK_F1) & 0x8000)
		m_dwCurrentKey |= KEY_F1;
	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_dwCurrentKey |= KEY_F2;
	if (GetAsyncKeyState(VK_F3) & 0x8000)
		m_dwCurrentKey |= KEY_F3;
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_dwCurrentKey |= KEY_TAB;

	if (GetAsyncKeyState('Z') & 0x8000)
		m_dwCurrentKey |= KEY_Z;
	if (GetAsyncKeyState('X') & 0x8000)
		m_dwCurrentKey |= KEY_X;

	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		m_dwCurrentKey |= KEY_NUMPAD4;
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
		m_dwCurrentKey |= KEY_NUMPAD5;
	if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
		m_dwCurrentKey |= KEY_NUMPAD6;
	if (GetAsyncKeyState('C') & 0x8000)
		m_dwCurrentKey |= KEY_C;

	if (GetAsyncKeyState(VK_BACK) & 0x8000)
		m_dwCurrentKey |= KEY_BACKSPACE;
	if (GetAsyncKeyState('O') & 0x8000)
		m_dwCurrentKey |= KEY_O;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwCurrentKey |= KEY_P;

}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 전에 누른적 없고 현재 눌렸을 때
	if (!(m_dwKeyDown & dwKey) && (m_dwCurrentKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}

	// 전에 누른적 있고 현재 누르지 않았을 때
	if ((m_dwKeyDown & dwKey) && !(m_dwCurrentKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// 전에 누른적 있고 현재 누르지 않았을 때
	if ((m_dwKeyUp & dwKey) && !(m_dwCurrentKey & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}

	// 전에 누른적 없고 현재 눌렀을 때
	if (!(m_dwKeyUp & dwKey) && (m_dwCurrentKey & dwKey))
	{
		m_dwKeyUp |= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurrentKey & dwKey)
		return true;

	return false;
}
