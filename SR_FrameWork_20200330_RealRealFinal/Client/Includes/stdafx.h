// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <ctime>

using namespace std;

#include "fmod.h"
#pragma comment(lib,"fmodex_vc.lib")
// ������ ���� ������� ���� �ش� 
#include <io.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Define.h"
#include "Extern.h"
#include "Enum.h"
#include "Function.h"
#include "Functor.h"		// SoungMgr���� ���� ���.

#include "DeviceMgr.h"
#include "Management.h"
#include "ResourceMgr.h"
#include "SubjectMgr.h"
#include "CameraMgr.h"
#include "UiMgr.h"
#include "EventMgr.h"
#include "FontMgr.h"
#include "LineMgr.h"

#pragma comment(lib, "Systems.lib")
#pragma comment(lib, "Utilities.lib")
#pragma comment(lib, "Resources.lib")

//#ifdef _DEBUG
//
//#include <iostream>
//using namespace std;
//
//// console
//#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
//
//#include "vld.h"
//
//#endif
