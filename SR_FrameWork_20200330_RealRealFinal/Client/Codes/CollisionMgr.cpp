#include "stdafx.h"
#include "CollisionMgr.h"
#include"Transform.h"
#include"GameObject.h"
#include"ShootingMonster.h"
#include"ShootingBullet.h"
#include "SuperCar.h"

#include "SoundMgr.h"
IMPLEMENT_SINGLETON(CCollisionMgr)


CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_BulletToMonster(list<CShootingBullet*>* pListBullet, list<CShootingMonster*>* pListMonster)
{
	for (auto& Bullet : *pListBullet)
	{
		for (auto& Monster : *pListMonster)
		{
			Vec3 vPos = Bullet->GetPos();
			Vec3 vPos2 = Monster->GetPos();
			Vec3 vSize = dynamic_cast<ENGINE::CTransform*>(Bullet->GetComponent(L"Transform"))->GetSize();
			Vec3 vSize2 = dynamic_cast<ENGINE::CTransform*>(Monster->GetComponent(L"Transform"))->GetSize();

			if (Check_Sphere(Bullet->GetPos(), dynamic_cast<ENGINE::CTransform*>(Bullet->GetComponent(L"Transform"))->GetSize().x,
				Monster->GetPos(), dynamic_cast<ENGINE::CTransform*>(Monster->GetComponent(L"Transform"))->GetSize().x
			))
			{
				dynamic_cast<CShootingMonster*>(Monster)->Set_Dead();
				dynamic_cast<CShootingBullet*>(Bullet)->Set_Dead();
				//cout << "�浹:" << endl;
			}


		}
	}
}

void CCollisionMgr::Collision_BumperCar(CSuperCar* pAttack, CSuperCar* pHit)
{
	if (Check_Sphere(pAttack->Get_Pos(), pAttack->Get_Size(), pHit->GetPos(), pHit->Get_Size()))
	{
		//cout << "�浹" << endl;
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->PlaySound(L"����.mp3", CSoundMgr::CHANNELID::EFFECT);
		CSoundMgr::GetInstance()->SoundControl(CSoundMgr::CHANNELID::EFFECT, 0.3f);

		// �������� �ݵ�
		D3DXVECTOR3 vAttDir = pAttack->GetPos() - pHit->GetPos();
		vAttDir.y = 0.f;	// y�� ���ư��� ������ �ʿ䰡 ����.
		D3DXVec3Normalize(&vAttDir, &vAttDir);

		// fAttSpeed�� ������ ���� �и���.
		float fAttSpeed = pAttack->Get_Speed() - pHit->Get_Speed();
		if (fAttSpeed >= 0.f)
			fAttSpeed = 0.f;
		fAttSpeed *= 0.6f;

		float	fAttDistance = fabsf(fAttSpeed);	// �̹� ������ �����ֱ⶧���� ���밪���� ������ش�.
		if (fAttDistance <= 0)
			fAttDistance = 3;

		pAttack->Set_ReBoundDir(vAttDir);
		pAttack->Set_ReBoundDistance(fAttDistance);

		// �������� �ݵ�
		D3DXVECTOR3 vHitDir = pHit->GetPos() - pAttack->GetPos();
		vAttDir.y = 0.f;	// y�� ���ư��� ������ �ʿ䰡 ����.
		D3DXVec3Normalize(&vHitDir, &vHitDir);

		// fHitSpeed�� ������ ���� �и���.
		float fHitSpeed = pHit->Get_Speed() - pAttack->Get_Speed();
		if (fHitSpeed >= 0.f)
			fHitSpeed = 0.f;

		float	fHitDistance = fabsf(fHitSpeed);	// �̹� ������ �����ֱ⶧���� ���밪���� ������ش�.
		if (fHitDistance <= 0)
			fHitDistance = 5;

		pHit->Set_ReBoundDir(vHitDir);
		pHit->Set_ReBoundDistance(fHitDistance);
	}
}

bool CCollisionMgr::Check_Sphere(D3DXVECTOR3 vPos1, float fRadian1, D3DXVECTOR3 vPos2, float fRadian2)
{
	float fDist;
	fDist = VEC3::Length(vPos1 - vPos2);
	if (fDist <= fRadian1 / 2 + fRadian2 / 2)
		return true;
	return false;

}
