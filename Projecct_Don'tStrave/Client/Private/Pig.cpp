#include "Pig.h"
#include "GameInstance.h"
#include "XML_Manager.h"
#include "Camera.h"
#include "Player.h"
#include "PigHouse.h"
#include "CharacterManager.h"
#include "DropItemComponent.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{

}

CPig::CPig(const CPig& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CPig::Initialize_Prototype()
{
	CXML_Manager::GetInstance()->AddTexture("../Bin/Resources/Textures/Monster/Pig/pig_basic.scml", L"../Bin/Resources/Textures/Monster/Pig/", &m_tImageVec);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Pig/pig_basic.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Pig/pig_action.scml", &m_tAnimation);
	CXML_Manager::GetInstance()->LoadScml("../Bin/Resources/Textures/Monster/Pig/pig_attack.scml", &m_tAnimation);
	return S_OK;
}

HRESULT CPig::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	LoadImageFile();
	SetAnimation(m_tDir, MOTION::IDLE);

	m_pCollision_Com->BindEnterFunction([&](CGameObject* HitActor, _float3& _Dir) { BeginHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindOverlapFunction([&](CGameObject* HitActor, _float3& _Dir) { OverlapHitActor(HitActor, _Dir); });
	m_pCollision_Com->BindExitFunction([&](CGameObject* HitActor, _float3& _Dir) { EndHitActor(HitActor, _Dir); });
	
	m_pDropItem_Com->ADD_ItemData(43, 1);
	m_pDropItem_Com->ADD_ItemData(44, 1);
	m_pDropItem_Com->SetCreateEffect(1);

	return S_OK;
}

HRESULT CPig::Initialize_Late()
{
	__super::Initialize_Late();
	auto GroundObejcts = m_pGameInstance->GetAllObejctsToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"));
	if (GroundObejcts && !GroundObejcts->empty()) {
		for (auto& object : (*GroundObejcts)) {
			_float3 transform = object->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pTransformCom->GetWorldState(WORLDSTATE::POSITION);
			_float distance = D3DXVec3Length(&transform);
			if (0.1f > distance) {
				if (m_pHouse = dynamic_cast<CPigHouse*>(object)) {
					m_pHouse->EnterPig(this);
					m_bActive = false;
					m_bHouse = true;
					break;
				}
			}
		}
	}
	return S_OK;
}


void CPig::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	if (m_tMotion == ATTACK && m_bAttack && 470 <= (int)m_fAniTime) {
		m_bAttack = false;
	}
	__super::Priority_Update(fTimeDelta);
	ResetTarget(5.f);
	m_bHouse = false;
	if (!m_pNearTarget && m_pHouse && 30 <= *m_pTime) {
		m_pNearTarget = m_pHouse;
		m_bHouse = true;
	}
}

void CPig::Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	__super::Update(fTimeDelta);
	switch (m_tMotion)
	{
	case MOTION::IDLE:
	case MOTION::IDLE_TO_RUN:
	case MOTION::RUN:
	case MOTION::RUN_TO_IDLE:
	case MOTION::ATTACK:
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
	else if (m_pNearTarget) {
		_float3 move = m_pNearTarget->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION) - m_pMonsterData->fPos;
		if (D3DXVec3Length(&move) < 4.5f || m_bHouse) {
			if (m_tMotion != MOTION::RUN && m_tMotion != MOTION::IDLE_TO_RUN) {
				switch (m_tMotion)
				{
				case MOTION::DAMAGE:
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
						SetAnimation(m_tDir, MOTION::IDLE_TO_RUN);
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
					m_pMonsterData->fPos += m_fMove * m_pMonsterData->fSpeed * fTimeDelta;
					m_pTransformCom->SetPosition(m_pMonsterData->fPos);
				}
				break;
			case MOTION::ATTACK:
			case MOTION::RUN_TO_IDLE:
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
			case CPig::IDLE:
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
			case MOTION::ATTACK:
			case MOTION::RUN_TO_IDLE:
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

void CPig::Late_Update(_float fTimeDelta)
{
	if (!m_bActive) {
		return;
	}
	if (!m_isDead && m_pCamera->IsInObject(m_pTransformCom->GetWorldState(WORLDSTATE::POSITION), 10))
	{
		__super::Late_Update(fTimeDelta);
		SetDir();
		m_pGameInstance->Add_RenderGroup(RENDER::ALPHATEST, this);
		m_pCharacterManager->AddObject(this);
	}
}

HRESULT CPig::Render()
{
	__super::Render();
	if (m_bActive && !m_isDead) {
		RenderAnimation(m_sAnim, m_tAnimation, m_tImageVec);
	}
	return S_OK;
}

void CPig::Hit()
{
	SetAnimation(DIR::DIR_END, MOTION::DAMAGE);
}

void CPig::Attack()
{
	m_bAttack = true;
	SetAnimation(m_tDir, MOTION::ATTACK);
}

void CPig::Death()
{
	SetAnimation(DIR::DIR_END, MOTION::DEATH);
}
void CPig::OutHouse()
{
	m_pMonsterData->fPos = m_pHouse->GetTransfrom()->GetWorldState(WORLDSTATE::POSITION);
	m_pTransformCom->SetPosition(m_pMonsterData->fPos);
	m_bActive = true;
	m_bHouse = false;
	SetRandomMove();
}

void CPig::GetTarget(CGameObject* actor, _float distance)
{
	if (5.f > distance && m_fNearDistance / 2 > distance) {
		if (dynamic_cast<CCharacter*>(actor)) {
			if ((m_pMonsterData->iHostile && 200 <= dynamic_cast<CCharacter*>(actor)->Get_Char()->iId && !dynamic_cast<CCharacter*>(actor)->Get_Char()->bIsDead) ||
				(!dynamic_cast<CPig*>(actor) && !dynamic_cast<CHouse*>(actor) && dynamic_cast<CMonster*>(actor) && dynamic_cast<CMonster*>(actor)->Get_Active() && 1 == dynamic_cast<CMonster*>(actor)->Get_Monster()->iHostile)) {
				m_pNearTarget = actor;
				m_fNearDistance = distance;
			}
		}
	}
}

HRESULT CPig::SetAnimation(DIR dir, MOTION motion)
{
	if (DIR::DIR_END == dir) {
		m_tDir = DIR::DOWN;
	}
	if (motion != m_tMotion) {
		m_fAniTime = 0;
	}
	m_tMotion = motion;
	switch (motion)
	{
	case MOTION::IDLE:
		m_sAnim = L"idle_loop";
		break;
	case MOTION::IDLE_TO_WALK:
		m_sAnim = L"walk_pre";
		break;
	case MOTION::WALK:
		m_sAnim = L"walk_loop";
		break;
	case MOTION::WALK_TO_IDLE:
		m_sAnim = L"walk_pst";
		break;
	case MOTION::IDLE_TO_RUN:
		m_sAnim = L"run_pre";
		break;
	case MOTION::RUN:
		m_sAnim = L"run_loop";
		break;
	case MOTION::RUN_TO_IDLE:
		m_sAnim = L"run_pst";
		break;
	case MOTION::ATTACK:
		m_sAnim = L"atk";
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
	case MOTION::EAT:
		m_sAnim = L"eat";
		break;
	case MOTION::DAMAGE:
		m_sAnim = L"hit";
		break;
	case MOTION::ABANDON:
		m_sAnim = L"abandon";
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

void CPig::Damage(void* pArg)
{
	if (0 == m_pMonsterData->iHostile) {
		DAMAGE_DATA_BASE DamageBase = {};
		if (pArg) {
			DamageBase = *static_cast<DAMAGE_DATA_BASE*>(pArg);
			if (DamageBase.Attacker) {
				CCharacter* player = static_cast<CCharacter*>(DamageBase.Attacker);
				if (dynamic_cast<CPlayer*>(player)) {
					m_pMonsterData->iHostile = 1;
				}
			}
		}
	}
	__super::Damage(pArg);
}

void CPig::BeginHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

void CPig::OverlapHitActor(CGameObject* HitActor, _float3& _Dir)
{
	__super::OverlapHitActor(HitActor, _Dir);
	if (m_bHouse && m_pNearTarget == m_pHouse && HitActor == m_pHouse) {
		m_pHouse->EnterPig(this);
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
		if (m_tMotion == ATTACK && m_bAttack && 470 <= (int)m_fAniTime) {
			HitActor->Damage(&m_tDamage);
		}
	}
}

void CPig::EndHitActor(CGameObject* HitActor, _float3& _Dir)
{
}

CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPig* pInstance = new CPig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPig::Clone(void* pArg)
{
	CPig* pInstance = new CPig(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPig");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPig::Free()
{
	__super::Free();
}
