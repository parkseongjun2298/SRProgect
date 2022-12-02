#pragma once
#ifndef __BUMPERCARUI_H__

#include "GameObject.h"
class CBumperCarUi :
	public ENGINE::CGameObject
{
public:
	explicit CBumperCarUi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBumperCarUi();
};

#define __BUMPERCARUI_H__
#endif // !__BUMPERCARUI_H__
