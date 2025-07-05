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

	MONSTER_DESC data = *static_cast<MONSTER_DESC*>(pArg);
	m_pMonsterData = new MONSTER_DATA;
	m_pMonsterData->iId = data.iId;
	m_pMonsterData->iMaxHp = data.iMaxHp;
	m_pMonsterData->iHp = data.iMaxHp;
	m_pMonsterData->iMaxHit = data.iMaxHit;
	m_pMonsterData->iHit = data.iMaxHit;
	m_pMonsterData->iAtk = data.iAtk;
	m_pMonsterData->fSpeed = data.fSpeed / 4.f;
	m_pMonsterData->iAtkDistance = data.iAtkDistance;
	m_pMonsterData->iAtkSpeed = data.iAtkSpeed;
	m_pMonsterData->fPos = data.fPos;

	m_pChar = m_pMonsterData;


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

MONSTER_DATA* CMonster::Get_Monster()
{
	return m_pMonsterData;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Delete(m_pMonsterData);
}
