#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCharacter{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CCharacter{ Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	MONSTER_DESC data = *static_cast<MONSTER_DESC*>(pArg);
	m_pMonsterData = new MONSTER_DATA;
	m_pMonsterData->iId = data.iId;
	m_pMonsterData->iMaxHp = data.iMaxHp;
	m_pMonsterData-> iHp = data.iMaxHp;
	m_pMonsterData->iMaxHit = data.iMaxHit;
	m_pMonsterData->iHit = data.iMaxHit;
	m_pMonsterData->iAtk = data.iAtk;
	m_pMonsterData->fSpeed = data.fSpeed / 4.f;
	m_pMonsterData->iHostile = data.iHostile;
	m_pMonsterData->iAtkDistance = data.iAtkDistance / 5.f;
	m_pMonsterData->iAtkSpeed = data.iAtkSpeed;
	m_pMonsterData->fPos = data.fPos;
	m_tDamage.Attacker = this;
	m_tDamage.Damage = data.iAtk;

	m_pChar = m_pMonsterData;
	m_pCollision_Com->SetCollisionSize({ m_pMonsterData->iAtkDistance, 0.f ,0.f });

	m_pTransformCom->SetPosition(data.fPos);

	return S_OK;
}

HRESULT CMonster::Initialize_Late()
{
	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_fAttackTime += fTimeDelta * 2;

}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster::Render()
{
	__super::Render();

	return S_OK;
}

void CMonster::SetDir()
{
	if (m_pTarget) {
		m_fMoving = m_pTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		if (0.01f < abs(m_fMoving.x) + abs(m_fMoving.z)) {
			m_fAngle = D3DXToDegree(acosf(m_fMoving.x / sqrtf(powf(m_fMoving.x, 2) + powf(m_fMoving.z, 2))));
			if (0 < m_fMoving.z) {
				m_fAngle = 360.f - m_fAngle;
			}
		}
		D3DMATRIX view;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
		_float3 look = view.m[2];
		look.z *= -1;
		_float lookAngle = D3DXToDegree(acosf(look.x / sqrtf(powf(look.x, 2) + powf(look.z, 2))));
		lookAngle += 180;
		if (0 < look.z) {
			lookAngle = 360.f - lookAngle;
		}
		_float fAngle = lookAngle - m_fAngle;
		if (0 > fAngle) {
			fAngle += 360;
		}
		if ((0.f <= fAngle && fAngle < 40.f) || (fAngle < 360.f && fAngle >= 310.f)) {
			m_tMoveDIr = MOVE_DIR::MOVE_UP;
		}
		else if ((fAngle < 130.f && fAngle >= 40.f)) {
			m_tMoveDIr = MOVE_DIR::MOVE_LEFT;
		}
		else if (fAngle < 220.f && fAngle >= 130.f) {
			m_tMoveDIr = MOVE_DIR::MOVE_DOWN;
		}
		else if (fAngle < 310.f && fAngle >= 220.f) {
			m_tMoveDIr = MOVE_DIR::MOVE_RIGHT;
		}
	}
	else {
		__super::SetDir();
	}
}

MONSTER_DATA* CMonster::Get_Monster()
{
	return m_pMonsterData;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Delete(m_pMonsterData);
}
