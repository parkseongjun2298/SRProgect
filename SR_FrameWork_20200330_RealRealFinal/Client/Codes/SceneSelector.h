#pragma once

#ifndef __SCENE_SELECTOR_H__

#include "Logo.h"
#include "Stage.h"
#include "SubStageExample.h"
#include "ShootingGame.h"
#include "MoleCatchGame.h"
#include "BumperCarGame.h"

class CSceneSelector
{
public:
	explicit CSceneSelector(SCENE_TYPE eCurScene, SCENE_TYPE eNextScene)
		: m_eCurScene(eCurScene), m_eNextScene(eNextScene)
	{

	}

public:
	HRESULT operator()(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::CScene** ppScene)
	{
		if (m_eCurScene != m_eNextScene)
		{
			ENGINE::SafeDelete(*ppScene);

			switch (m_eNextScene)
			{
			case SCENE_LOGO:
				*ppScene = CLogo::Create(pGraphicDev);
				break;
			case SCENE_STAGE:
				*ppScene = CStage::Create(pGraphicDev);
				break;
			case SCENE_SUBSTAGEEXAMPLE:
				*ppScene = CSubStageExample::Create(pGraphicDev);
				break;
			case SCENE_SHOOTINGGAME:
				*ppScene = CShootingGame::Create(pGraphicDev);
				break;
			case SCENE_MOLECATCHGAME:
				*ppScene = CMoleCatchGame::Create(pGraphicDev);
				break;
			case SCENE_BUMPERCARGAME:
				*ppScene = CBumperCarGame::Create(pGraphicDev);
				break;
			}

			NULL_CHECK_RETURN(*ppScene, E_FAIL);
		}

		return S_OK;
	}

private:
	SCENE_TYPE m_eCurScene;
	SCENE_TYPE m_eNextScene;

};

#define __SCENE_SELECTOR_H__
#endif