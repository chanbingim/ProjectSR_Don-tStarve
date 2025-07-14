#include "Monster.h"
#include "GameInstance.h"
#include "Mouse.h"
#include "Player.h"
#include "DropItemComponent.h"
#include "QuestManager.h"

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
	if(FAILED(Ready_Components()))
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
	m_pMonsterData->bIsDead = false;
	m_tDamage.Attacker = this;
	m_tDamage.Damage = data.iAtk;
<<<<<<< HEAD
	
=======
	m_bPlayerKill = false;

>>>>>>> origin/0714_kjh
	m_pChar = m_pMonsterData;
	m_pCollision_Com->SetCollisionSize({ m_pMonsterData->iAtkDistance, 0.f ,0.f });

	m_pTransformCom->SetPosition(data.fPos);
	m_bActive = true;

	return S_OK;
}

HRESULT CMonster::Initialize_Late()
{
	__super::Initialize_Late();
	m_pPlayerPos = &(static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), 0))->Get_Player()->fPos);
	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_fAttackTime += fTimeDelta * 2;
	m_fMoveTime += fTimeDelta;
	if (m_fMoveDelay <= m_fMoveTime) {
		SetRandomMove();
	}
	if (!m_pNearTarget) {
		m_bTarget = false;
	}
	if (0 < m_pMonsterData->iHp) {
		m_bPlayerKill = false;
	}
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	_float3 vPickingPos = {};
	if (m_bActive && m_pGameInstance->KeyDown(VK_LBUTTON) && dynamic_cast<CVIBuffer_Rect*>(m_pVIBufferCom)->Picking(m_pTransformCom, &vPickingPos))
	{
		dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Player")))->Get_Player()->pWorkObject = this;
	}
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
	if (m_pNearTarget) {
		if (m_bDir) {
			m_fMoving = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			if (0.0001f < abs(m_fMoving.x) + abs(m_fMoving.z)) {
				m_fAngle = D3DXToDegree(acosf(m_fMoving.x / D3DXVec3Length(&m_fMoving)));
				if (0 < m_fMoving.z) {
					m_fAngle = 360.f - m_fAngle;
				}
			}
		}
		D3DMATRIX view;
		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
		_float3 look = view.m[2];
		look.z *= -1;
		_float lookAngle = D3DXToDegree(acosf(look.x / D3DXVec3Length(&m_fMoving)));
		lookAngle += 180;
		if (0 < look.z) {
			lookAngle = 360.f - lookAngle;
		}
		_float fAngle = lookAngle - m_fAngle;
		if (0 > fAngle) {
			fAngle += 360;
		}
		if ((0.f <= fAngle && fAngle < 44.9f) || (fAngle < 360.f && fAngle >= 314.9f)) {
			m_tMoveDIr = MOVE_DIR::MOVE_UP;
		}
		else if ((fAngle < 134.9f && fAngle >= 44.9f)) {
			m_tMoveDIr = MOVE_DIR::MOVE_LEFT;
		}
		else if (fAngle < 224.9f && fAngle >= 134.9f) {
			m_tMoveDIr = MOVE_DIR::MOVE_DOWN;
		}
		else if (fAngle < 314.9f && fAngle >= 224.9f) {
			m_tMoveDIr = MOVE_DIR::MOVE_RIGHT;
		}
	}
	else {
		__super::SetDir();
	}
}

void CMonster::SetRandomMove()
{
	m_fMoveTime = 0.f;
	m_fMoveStart = (_float)(rand() % 2) + 1;
	m_fMoveDelay = (_float)(rand() % 4) + 8;
	m_fMove = { (_float)(rand() % 10 - rand() % 10), 0.f, (_float)(rand() % 10 - rand() % 10) };
	D3DXVec3Normalize(&m_fMove, &m_fMove);
}

void CMonster::Damage(void* pArg)
{
	__super::Damage(pArg);
	DAMAGE_DATA_BASE DamageBase = {};
	if (nullptr != pArg) {
		DamageBase = *static_cast<DAMAGE_DATA_BASE*>(pArg);
		CCharacter* player = {};
		player = static_cast<CCharacter*>(DamageBase.Attacker);
		if (200 <= player->Get_Char()->iId)
			m_bPlayerKill = true;
	}
}

void CMonster::Death()
{
	if (m_pMonsterData && 0 >= m_pMonsterData->iHp && m_pDropItem_Com)
	{
		m_pDropItem_Com->DropItem(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Material_Item"),
			EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), m_pMonsterData->fPos);
	}
}

MONSTER_DATA* CMonster::Get_Monster()
{
	return m_pMonsterData;
}

_float CMonster::Get_Sound()
{
	_float3 transform = m_pMonsterData->fPos - *m_pPlayerPos;
	return 10 - (D3DXVec3Length(&transform));
}

HRESULT CMonster::Ready_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_DropItem"),
		TEXT("Com_DropItem"), (CComponent**)&m_pDropItem_Com)))
		return E_FAIL;
	return S_OK;
}

void CMonster::Free()
{
<<<<<<< HEAD
	if(m_pMonsterData && 0 >= m_pMonsterData->iHp && m_pDropItem_Com)
	{
		m_pDropItem_Com->DropItem(EnumToInt(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Material_Item"),
			EnumToInt(LEVEL::GAMEPLAY), TEXT("Layer_Item"), m_pMonsterData->fPos);

		CQuestManager::GetInstance()->CallMonsterDeath(m_pMonsterData->iId);
	}
=======
>>>>>>> origin/0714_kjh
	__super::Free();

	Safe_Delete(m_pMonsterData);
	Safe_Release(m_pDropItem_Com);
}
