#pragma once
#ifndef __BUMPERSTART_H__

#include "UiPlate.h"
class CBumperStart :
	public CUiPlate
{
public:
	explicit CBumperStart(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperStart();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT AddComponent() override;
	virtual void Release() override;

public:
	void CountSize();
	void StartSize();
	void QuestSize();

public:
	void Set_Count(bool bCheck);
	void Set_Quest(bool bCheck);

public:
	bool Get_GameStart();

public:
	static CBumperStart* Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos, D3DXVECTOR3 vSize, wstring wstrTextureTag, int iIndex = 0);

private:
	bool		m_bIsCount;
	bool		m_bIsStart;
	bool		m_bIsQuest;

	bool		m_bIsGameStart;

	float		m_fTextureIndex;
};

#define __BUMPERSTART_H__
#endif // !__BUMPERSTART_H__
