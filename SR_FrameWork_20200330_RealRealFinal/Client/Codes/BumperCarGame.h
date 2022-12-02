#pragma once
#ifndef __BUMPERCARGAME_H__

#include "Scene.h"
class CCollisionMgr;
class CBumperCarGame :
	public ENGINE::CScene
{
private:
	explicit CBumperCarGame(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBumperCarGame();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	virtual HRESULT Add_Environment_Layer() override;
	virtual HRESULT Add_Object_Layer() override;
	virtual HRESULT Add_Effect_Layer() override;
	virtual HRESULT Add_UI_Layer() override;

public:
	static CBumperCarGame* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	ENGINE::CResourceMgr*	m_pResourceMgr;
	CUiMgr*					m_pUiMgr;
	CCameraMgr*				m_pCameraMgr;
	CCollisionMgr*			m_pCollisionMgr;
};

#define __BUMPERCARGAME_H__
#endif // !__BUMPERCARGAME_H__
