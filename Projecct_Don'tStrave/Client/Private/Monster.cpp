#include "Monster.h"

#include "UserInterface.h"
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

	MONSTER_DATA data = *static_cast<MONSTER_DATA*>(pArg);
	m_iId = data.iId;
	m_iMaxHp = data.iMaxHp;
	m_iAtk = data.iAtk;
	m_fSpeed = data.fSpeed / 4.f;
	m_iAtkDistance = data.iAtkDistance;
	m_iAtkSpeed = data.iAtkSpeed;
	m_iMaxHit = data.iHitMax;

	m_iHp = m_iMaxHp;
	m_iHit = m_iMaxHit;

	return S_OK;
}

HRESULT CMonster::Initialize_Late()
{
	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
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

MONSTER_DATA CMonster::Get_Monster()
{
	return MONSTER_DATA();
}

void CMonster::Free()
{
	__super::Free();

}
