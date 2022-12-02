#pragma once
#ifndef __BUMPEREND_H__

#include "UiPlate.h"
class CBumperEnd :
	public CUiPlate
{
public:
	explicit CBumperEnd(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperEnd();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	// Set
	void Set_Victory(bool bCheck);
	void Set_Fail(bool bCheck);

public:
	static CBumperEnd* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	bool		m_bIsVictory;
	bool		m_bIsFail;
};

#define __BUMPEREND_H__
#endif // !__BUMPEREND_H__
