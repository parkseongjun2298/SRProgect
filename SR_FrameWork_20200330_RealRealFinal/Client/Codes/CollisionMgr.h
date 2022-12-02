#pragma once

#ifndef __COLLISIONMGR_H__

class CSuperCar;
class CShootingBullet;
class CShootingMonster;
class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	CCollisionMgr();
public:
	virtual ~CCollisionMgr();

public:
	void Collision_BulletToMonster(list<CShootingBullet*>* pListBullet, list<CShootingMonster*>* pListMonster);

public:
	void Collision_BumperCar(CSuperCar* pAttack, CSuperCar* pHit);

private:
	bool Check_Sphere(D3DXVECTOR3 vPos1, float fRadian1, D3DXVECTOR3 vPos2, float fRadian2);




};

#define __COLLISIONMGR_H__
#endif
