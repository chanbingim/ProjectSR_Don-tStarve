#include "SpiderWarrior.h"
#include "SpiderHouse.h"
#include "SpiderQueen.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Camera.h"
#include "Food.h"
#include "Item_Manager.h"
#include "CharacterManager.h"
#include "DropItemComponent.h"

CSpiderWarrior::CSpiderWarrior(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CSpider{ pGraphic_Device }
{

}

CSpiderWarrior::CSpiderWarrior(const CSpiderWarrior& Prototype)
	: CSpider{ Prototype }
{
}

HRESULT CSpiderWarrior::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/SpiderWarrior/spider.scml", L"../Bin/Resources/Textures/Monster/SpiderWarrior/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderWarrior/spider.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/SpiderWarrior/spiderwarrior.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CSpiderWarrior::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	SetAnimation(m_tDir, MOTION::IDLE);
	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });


	m_pDropItem_Com->ADD_ItemData(46, 2);
	m_pDropItem_Com->SetCreateEffect(1);

	return S_OK;
}


void CSpiderWarrior::Priority_Update(_float fTimeDelta)
{
	if (m_tMotion == ATTACK && m_bAttack && 840 <= (int)m_fAniTime) {
		m_bAttack = false;
	}
	if (!m_bActive) {
		return;
	}
	__super::Priority_Update(fTimeDelta);
	if (MOTION::IDLE_TO_EAT == m_tMotion) {
		return;
	}
	ResetTarget(4.f);

	m_bHouse = false;
	if (!m_pNearTarget && m_pHouse && 30 >= *m_pTime) {
		m_pNearTarget = m_pHouse;
		m_bHouse = true;
	}
}

void CSpiderWarrior::Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	__super::Update(fTimeDelta);
	switch (m_tMotion)
	{
	case IDLE:
		switch (m_tMoveDIr)
		{
		case MOVE_DIR::MOVE_UP:
			m_tDir = DIR::UP;
			break;
		default:
			m_tDir = DIR::DOWN;
			break;
		}
		SetAnimation(m_tDir, m_tMotion);
		break;
	case MOTION::IDLE_TO_RUN:
	case MOTION::RUN:
	case MOTION::RUN_TO_IDLE:
	case MOTION::ATTACK:
	case MOTION::DASH_ATTACK:
		switch (m_tMoveDIr)
		{
		case MOVE_DIR::MOVE_DOWN:
			m_tDir = DIR::DOWN;
			break;
		case MOVE_DIR::MOVE_LEFT:
		case MOVE_DIR::MOVE_RIGHT:
			m_tDir = DIR::SIDE;
			break;
		case MOVE_DIR::MOVE_UP:
			m_tDir = DIR::UP;
			break;
		default:
			break;
		}
		SetAnimation(m_tDir, m_tMotion);
		break;
	}
	if (m_tMotion == MOTION::DEATH) {
		if (m_iLength <= m_fAniTime) {
			m_isDead = true;
			return;
		}
	}
	else if (m_tMotion == MOTION::DASH_ATTACK) {
		if (m_iLength <= m_fAniTime) {
			SetAnimation(m_tDir, MOTION::IDLE);
		}
		else if (267 <= m_fAniTime && 600 >= m_fAniTime) {
			m_pMonsterData->fPos += m_fDash * fTimeDelta;
			m_pTransformCom->SetPosition(m_pMonsterData->fPos);
		}
	}
	else if (m_pNearTarget) {
		_float3 move = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;
		if (D3DXVec3Length(&move) < 3.5f) {
			m_fAtkCool -= fTimeDelta;
			if (dynamic_cast<CFood*>(m_pNearTarget) && MOTION::EAT != m_tMotion) {
				m_bTarget = true;
				_float distance = D3DXVec3Length(&move);
				if (0.1f > distance) {
					SetAnimation(m_tDir, MOTION::IDLE_TO_EAT);
				}
				else {
					SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
				}
			}
			if (MOTION::DAMAGE != m_tMotion && 0.f >= m_fAtkCool) {
				if (MOTION::TAUNT == m_tMotion && m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::DASH_ATTACK);
					m_fAtkCool = 5.f;
					D3DXVec3Normalize(&move, &move);
					m_fDash = move * 5;
				}
				else {
					SetAnimation(m_tDir, MOTION::TAUNT);
				}
			}
			else if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
				switch (m_tMotion)
				{
				case MOTION::TAUNT:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
					}
					break;
				case MOTION::IDLE_TO_EAT:
					if (m_iLength <= m_fAniTime) {
						if (!m_pNearTarget->isDead()) {
							SetAnimation(m_tDir, MOTION::EAT);
							m_pMonsterData->iHp = min(m_pMonsterData->iMaxHp, m_pMonsterData->iHp + CItem_Manager::GetInstance()->Get_ItemData(dynamic_cast<CItem*>(m_pNearTarget)->Get_Info().iItemID).iHungerChange);
							m_pNearTarget->SetDead();
							m_pNearTarget = nullptr;
						}
						else {
							SetAnimation(m_tDir, MOTION::IDLE);
						}
					}
					break;
				case MOTION::EAT:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::EAT_TO_IDLE);
					}
					break;
				case MOTION::EAT_TO_IDLE:
				case MOTION::DAMAGE:
					if (m_iLength <= m_fAniTime) {
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					break;
				case MOTION::ATTACK:
					if (m_iLength <= m_fAniTime) {
						m_fAttackTime = 0;
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					break;
				default:
					if (m_bCol) {
						SetAnimation(m_tDir, MOTION::IDLE);
					}
					else {
						if (m_bTarget) {
							SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
						}
						else {
							SetAnimation(m_tDir, MOTION::TAUNT);
							m_bTarget = true;
						}
					}
					break;
				}
			}
			else if (m_bCol) {
				SetAnimation(m_tDir, MOTION::IDLE);
			}
			else {
				if (m_tMotion == MOTION::IDLE_TO_RUN && m_iLength <= m_fAniTime)
				{
					SetAnimation(m_tDir, MOTION::RUN);
				}
				D3DXVec3Normalize(&move, &move);

				m_pMonsterData->fPos += move * m_pMonsterData->fSpeed * fTimeDelta;
				m_pTransformCom->SetPosition(m_pMonsterData->fPos);
			}
		}
		else {
			switch (m_tMotion)
			{
			case MOTION::IDLE_TO_RUN:
			case MOTION::RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
				}
				else {
					D3DXVec3Normalize(&move, &move);
					m_pMonsterData->fPos += move * m_pMonsterData->fSpeed * fTimeDelta;
					m_pTransformCom->SetPosition(m_pMonsterData->fPos);
				}
				break;
			case MOTION::EAT:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::EAT_TO_IDLE);
				}
				break;
			case MOTION::ATTACK:
			case MOTION::EAT_TO_IDLE:
			case MOTION::RUN_TO_IDLE:
			case MOTION::TAUNT:
			case MOTION::DAMAGE:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::IDLE);
				}
				break;
			default:
				break;
			}
		}
	}
	else {
		if (m_fMoveStart > m_fMoveTime) {
			switch (m_tMotion)
			{
			case MOTION::IDLE_TO_EAT:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::IDLE);
				}
				break;
			case MOTION::EAT:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::EAT_TO_IDLE);
				}
				return;
			case MOTION::ATTACK:
			case MOTION::EAT_TO_IDLE:
			case MOTION::RUN_TO_IDLE:
			case MOTION::TAUNT:
			case MOTION::DAMAGE:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
				}
			case MOTION::IDLE:
				SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
				break;
			case MOTION::IDLE_TO_RUN:
				if (m_iLength <= m_fAniTime)
				{
					SetAnimation(m_tDir, MOTION::RUN);
				}
				break;
			}
			if (m_tMotion == MOTION::IDLE_TO_RUN && m_iLength <= m_fAniTime)
			{
				SetAnimation(m_tDir, MOTION::RUN);
			}
			m_pMonsterData->fPos += m_fMove * m_pMonsterData->fSpeed * fTimeDelta;
			m_pTransformCom->SetPosition(m_pMonsterData->fPos);
		}
		else {
			switch (m_tMotion)
			{
			case MOTION::IDLE_TO_RUN:
			case MOTION::RUN:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::RUN_TO_IDLE);
				}
				else {
					m_pMonsterData->fPos += m_fMove * m_pMonsterData->fSpeed * fTimeDelta;
					m_pTransformCom->SetPosition(m_pMonsterData->fPos);
				}
				break;
			case MOTION::EAT:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::EAT_TO_IDLE);
				}
				break;
			case MOTION::ATTACK:
			case MOTION::EAT_TO_IDLE:
			case MOTION::RUN_TO_IDLE:
			case MOTION::TAUNT:
			case MOTION::DAMAGE:
				if (m_iLength <= m_fAniTime) {
					SetAnimation(m_tDir, MOTION::IDLE);
				}
				break;
			default:
				break;
			}
		}
	}
}

void CSpiderWarrior::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	__super::Late_Update(fTimeDelta);
	if (!m_isDead && m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION), 10))
	{
		SetDir();
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
		m_pCharacterManager->AddObject(this);
	}
}

HRESULT CSpiderWarrior::Render()
{
	if (m_bActive && !m_isDead) {
		__super::Render();
		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);
	}
	return S_OK;
}

void CSpiderWarrior::Hit()
{
	SetAnimation(m_tDir, MOTION::DAMAGE);
}

void CSpiderWarrior::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CSpiderWarrior::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}

void CSpiderWarrior::OutHouse()
{
	__super::OutHouse();
	SetAnimation(m_tDir, MOTION::IDLE);
}

void CSpiderWarrior::GetTarget(CGameObject* actor, _float distance)
{
	if (4.f > distance && m_fNearDistance / 2 > distance) {
		if (dynamic_cast<CCharacter*>(actor)) {
			if ((200 <= dynamic_cast<CCharacter*>(actor)->Get_Char()->iId && !dynamic_cast<CCharacter*>(actor)->Get_Char()->bIsDead) ||
				(dynamic_cast<CMonster*>(actor) && dynamic_cast<CMonster*>(actor)->Get_Active() && !dynamic_cast<CSpider*>(actor) && !dynamic_cast<CSpiderQueen*>(actor) && 2 != dynamic_cast<CMonster*>(actor)->Get_Monster()->iHostile && !dynamic_cast<CHouse*>(actor))) {
				m_pNearTarget = actor;
				m_fNearDistance = distance;
			}
		}
		else if (dynamic_cast<CFood*>(actor)) {
			if (FOOD::MEAT == CItem_Manager::GetInstance()->Get_ItemData(dynamic_cast<CItem*>(actor)->Get_Info().iItemID).eFoodtype) {
				m_pNearTarget = actor;
				m_fNearDistance = distance;
			}
		}
	}
}

HRESULT CSpiderWarrior::SetAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir || ((MOTION::IDLE == motion || MOTION::DAMAGE == motion || MOTION::IDLE_TO_EAT == motion || MOTION::EAT == motion || MOTION::EAT_TO_IDLE == motion || MOTION::TAUNT == motion) && DIR::SIDE == dir)) {
		m_tDir = DIR::DOWN;
	}
	if (motion != m_tMotion) {
		m_fAniTime = 0;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::IDLE:
		m_sAnim = L"idle";
		break;
	case MOTION::IDLE_TO_RUN:
		m_sAnim = L"walk_pre";
		break;
	case MOTION::RUN:
		m_sAnim = L"walk_loop";
		break;
	case MOTION::RUN_TO_IDLE:
		m_sAnim = L"walk_pst";
		break;
	case MOTION::ATTACK:
		m_sAnim = L"atk";
		break;
	case MOTION::DASH_ATTACK:
		m_sAnim = L"warrior_atk";
		break;
	case MOTION::IDLE_TO_SLEEP:
		m_sAnim = L"sleep_pre";
		break;
	case MOTION::SLEEP:
		m_sAnim = L"sleep_loop";
		break;
	case MOTION::SLEEP_TO_IDLE:
		m_sAnim = L"sleep_pst";
		break;
	case MOTION::IDLE_TO_EAT:
		m_sAnim = L"eat";
		break;
	case MOTION::EAT:
		m_sAnim = L"eat_loop";
		break;
	case MOTION::EAT_TO_IDLE:
		m_sAnim = L"eat_pst";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::IDLE_TO_COWER:
		m_sAnim = L"cower_pre";
		break;
	case MOTION::COWER:
		m_sAnim = L"cower_loop";
		break;
	case MOTION::COWER_TO_IDLE:
		m_sAnim = L"cower_pst";
		break;
	case MOTION::TAUNT:
		m_sAnim = L"taunt";
		break;
	case MOTION::DEATH:
		m_sAnim = L"death";
		break;
	}
	switch (dir)
	{
	case DIR::DOWN:
		m_sAnim += L"_down";
		break;
	case DIR::SIDE:
		m_sAnim += L"_side";
		break;
	case DIR::UP:
		m_sAnim += L"_up";
		break;
	}
	return S_OK;
}

void CSpiderWarrior::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
	if (dynamic_cast<CCharacter*>(HitActor)) {
		if (!dynamic_cast<CMonster*>(HitActor) && m_tMotion == DASH_ATTACK) {
			m_pNearTarget->Damage(&m_tDamage);
		}
	}
}

void CSpiderWarrior::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	__super::OverlapHitActor(HitActor, _Dir);
	if (m_bHouse && m_pNearTarget == m_pHouse && HitActor == m_pHouse) {
		m_pHouse->EnterSpider(this);
		m_bActive = false;
		m_pNearTarget = nullptr;
		return;
	}
	if (HitActor == m_pNearTarget && m_tMotion != DAMAGE && m_tMotion != DEATH) {
		_float3 transform = HitActor->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
		_float distance = D3DXVec3Length(&transform);
		if ((m_pMonsterData->iAtkDistance / 2.f) >= distance || (dynamic_cast<CMonster*>(HitActor) && (m_pMonsterData->iAtkDistance / 2.f) >= distance - (dynamic_cast<CMonster*>(HitActor)->Get_Monster()->iAtkDistance / 2.f))) {
			m_bCol = true;
			if (dynamic_cast<CCharacter*>(HitActor) && m_tMotion != ATTACK && m_pMonsterData->iAtkSpeed <= m_fAttackTime) {
				Attack();
			}
		}
		if (m_tMotion == ATTACK && m_bAttack && 840 <= (int)m_fAniTime) {
			HitActor->Damage(&m_tDamage);
		}
	}
}

void CSpiderWarrior::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CSpiderWarrior* CSpiderWarrior::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpiderWarrior* pInstance = new CSpiderWarrior(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderWarrior::Clone(void* pArg)
{
	CSpiderWarrior* pInstance = new CSpiderWarrior(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderWarrior");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderWarrior::Free()
{
	__super::Free();
}
